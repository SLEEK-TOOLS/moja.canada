#include "moja/modules/cbm/cbmaggregatorlandunitdata.h"

#include <moja/flint/recordaccumulatorwithmutex.h>
#include <moja/flint/ipool.h>
#include <moja/flint/ivariable.h>
#include <moja/flint/ioperationresult.h>
#include <moja/flint/ioperationresultflux.h>

#include <moja/itiming.h>
#include <moja/signals.h>
#include <moja/notificationcenter.h>

namespace moja {
namespace modules {
namespace cbm {

	void CBMAggregatorLandUnitData::configure(const DynamicObject& config) {
		if (config.contains("reporting_classifier_set")) {
			_classifierSetVar = config["reporting_classifier_set"].extract<std::string>();
		} else {
			_classifierSetVar = "classifier_set";
		}
	}

	void CBMAggregatorLandUnitData::subscribe(NotificationCenter& notificationCenter) {
        notificationCenter.subscribe(signals::LocalDomainInit, &CBMAggregatorLandUnitData::onLocalDomainInit, *this);
        notificationCenter.subscribe(signals::TimingInit	 , &CBMAggregatorLandUnitData::onTimingInit		, *this);
        notificationCenter.subscribe(signals::OutputStep	 , &CBMAggregatorLandUnitData::onOutputStep		, *this);
		notificationCenter.subscribe(signals::Error			 , &CBMAggregatorLandUnitData::onError			, *this);
    }

    Int64 CBMAggregatorLandUnitData::getPoolId(const flint::IPool* pool) {
        PoolInfoRecord poolInfo(pool->name());
        return _poolInfoDimension->search(poolInfo)->getId();
    }

    void CBMAggregatorLandUnitData::recordLandUnitData(bool isSpinup) {
        auto locationId = recordLocation(isSpinup);
        recordPoolsSet(locationId, isSpinup);
        recordFluxSet(locationId);
		recordAgeArea(locationId, isSpinup);
    }

	void CBMAggregatorLandUnitData::recordClassifierNames(const DynamicObject& classifierSet) {
		Poco::Mutex::ScopedLock lock(*_classifierNamesLock);
		if (!_classifierNames->empty()) {
			return;
		}

		for (const auto& classifier : classifierSet) {
			std::string name = classifier.first;
			std::replace(name.begin(), name.end(), '.', '_');
			std::replace(name.begin(), name.end(), ' ', '_');
			_classifierNames->push_back(name);
		}
	}

    Int64 CBMAggregatorLandUnitData::recordLocation(bool isSpinup) {
        Int64 dateRecordId = -1;
		auto testLocationID = this->_landUnitData->getVariable("LandUnitId");
        if (!isSpinup) {
            // Find the date dimension record.			
            const auto timing = _landUnitData->timing();
            DateRecord dateRecord(
                timing->step(), timing->curStartDate().year(),
                timing->curStartDate().month(), timing->curStartDate().day(),
                timing->fractionOfStep(), timing->stepLengthInYears());

            auto storedDateRecord = _dateDimension->accumulate(dateRecord);
            dateRecordId = storedDateRecord->getId();
        }

        // Classifier set information.
        const auto& landUnitClassifierSet = _classifierSet->value().extract<DynamicObject>();
        std::vector<Poco::Nullable<std::string>> classifierSet;
        bool firstPass = _classifierNames->empty();
		if (firstPass) {
			recordClassifierNames(landUnitClassifierSet);
		}

        for (const auto& classifier : landUnitClassifierSet) {
			Poco::Nullable<std::string> classifierValue;
			if (!classifier.second.isEmpty()) {
				classifierValue = classifier.second.convert<std::string>();
			}

            classifierSet.push_back(classifierValue);
        }

        ClassifierSetRecord cSetRecord(classifierSet);
        auto storedCSetRecord = _classifierSetDimension->accumulate(cSetRecord);
        auto classifierSetRecordId = storedCSetRecord->getId();

        std::string landClass = _landClass->value().extract<std::string>();
		LandClassRecord landClassRecord(landClass);
        auto storedLandClassRecord = _landClassDimension->accumulate(landClassRecord);
        auto landClassRecordId = storedLandClassRecord->getId();

		TemporalLocationRecord locationRecord(
            classifierSetRecordId, dateRecordId, landClassRecordId, _landUnitArea);

        auto storedLocationRecord = _locationDimension->accumulate(locationRecord);
        return storedLocationRecord->getId();
    }

    void CBMAggregatorLandUnitData::recordPoolsSet(Int64 locationId, bool isSpinup) {
        auto pools = _landUnitData->poolCollection();
        for (auto& pool : _landUnitData->poolCollection()) {
			PoolInfoRecord poolInfo(pool->name());
            auto poolId = _poolInfoDimension->search(poolInfo)->getId();
            double poolValue = pool->value() * _landUnitArea;
			PoolRecord poolRecord(locationId, poolId, poolValue);
            _poolDimension->accumulate(poolRecord);
        }
    }

	int CBMAggregatorLandUnitData::toAgeClass(int standAge) {		
		int first_end_point = age_class_range - 1;	// The endpoint age of the first age class.
		double offset;					// An offset of the age to ensure that the first age class will have the endpoint FIRSTENDPOINT.
		double classNum;				// The age class as an double.
		double temp;					// The integral part of the age class as a double.									 
		if (standAge < 0) { 
			return 0;
		}
		/* Calculate the age class as an integer.  First determine the offset to ensure the correct endpoint of the first
		* age class and use this value in calculating the age class. */
		offset = first_end_point - (age_class_range / 2.0) + 0.5;
		classNum = ((standAge - offset) / age_class_range) + 1.0;
		if (modf(classNum, &temp) >= 0.5)
			classNum = ceil(classNum);
		else
			classNum = floor(classNum);

		/* If the calculated age class is too great, use the oldest age class. */
		if ((int)classNum >= number_of_age_classes)
			classNum = (double)(number_of_age_classes - 1);

		/* Convert the age class as an integer into an age class. */
		return ((int)classNum);
	}

	void CBMAggregatorLandUnitData::recordAgeArea(Int64 locationId, bool isSpinup) {
		int standAge = _landUnitData->getVariable("age")->value();
		int ageClass = toAgeClass(standAge);
		double area = this->_landUnitArea;

		AgeAreaRecord ageAreaRecord(locationId, ageClass, area);
		_AgeAreaDimension->accumulate(ageAreaRecord);		
	}

    void CBMAggregatorLandUnitData::recordFluxSet(Int64 locationId) {
        // If Flux set is empty, return immediately.
        if (_landUnitData->getOperationLastAppliedIterator().empty()) {
            return;
        }

        for (auto operationResult : _landUnitData->getOperationLastAppliedIterator()) {
            const auto& metaData = operationResult->metaData();
			
			// Find the module info dimension record.
			auto disturbanceTypeName = metaData->disturbanceTypeName == ""
        		? "Annual Process"
        		: metaData->disturbanceTypeName;

			auto disturbanceType = disturbanceTypeName == "Annual Process"
        		? 0
        		: metaData->disturbanceType;

			ModuleInfoRecord moduleInfoRecord(
				metaData->libraryType, metaData->libraryInfoId,
				metaData->moduleType, metaData->moduleId, metaData->moduleName,
				disturbanceTypeName, disturbanceType);

			auto storedModuleInfoRecord = _moduleInfoDimension->accumulate(moduleInfoRecord);
			auto moduleInfoRecordId = storedModuleInfoRecord->getId();

			DisturbanceRecord disturbanceRecord(locationId, moduleInfoRecordId, _landUnitArea);
			_disturbanceDimension->accumulate(disturbanceRecord);

			for (auto it : operationResult->operationResultFluxCollection()) {
                auto srcIx = it->source();
                auto dstIx = it->sink();
                if (srcIx == dstIx) {
                    continue; // don't process diagonal - flux to & from same pool is ignored
                }

                auto fluxValue = it->value() * _landUnitArea;
                auto srcPool = _landUnitData->getPool(srcIx);
                auto dstPool = _landUnitData->getPool(dstIx);

                // Now have the required dimensions - look for the flux record.
				FluxRecord fluxRecord(
                    locationId, moduleInfoRecordId, getPoolId(srcPool),
                    getPoolId(dstPool), fluxValue);

                _fluxDimension->accumulate(fluxRecord);
            }
        }

        _landUnitData->clearLastAppliedOperationResults();
    }

	void CBMAggregatorLandUnitData::doError(std::string msg) {
		bool detailsAvailable = _spatialLocationInfo != nullptr;

		auto module = detailsAvailable ? _spatialLocationInfo->getProperty("module").convert<std::string>() : "unknown";
		ErrorRecord errorRec(module, msg);
		const auto storedError = _errorDimension->accumulate(errorRec);

		auto locationId = detailsAvailable ? recordLocation(true) : -1;
		LocationErrorRecord locErrRec(locationId, storedError->getId());
		_locationErrorDimension->accumulate(locErrRec);
	}

    void CBMAggregatorLandUnitData::doTimingInit() {
        _landUnitArea = _spatialLocationInfo->getProperty("landUnitArea");

        // Record post-spinup pool values.
        recordLandUnitData(true);
    }

    void CBMAggregatorLandUnitData::doLocalDomainInit() {
		for (auto& pool : _landUnitData->poolCollection()) {
			PoolInfoRecord poolInfoRecord(pool->name());
			_poolInfoDimension->accumulate(poolInfoRecord);
		}

        _spatialLocationInfo = std::static_pointer_cast<flint::SpatialLocationInfo>(
            _landUnitData->getVariable("spatialLocationInfo")->value()
            .extract<std::shared_ptr<flint::IFlintData>>());

        _classifierSet = _landUnitData->getVariable(_classifierSetVar);
        _landClass = _landUnitData->getVariable("unfccc_land_class");

		age_class_range = 20; //default age class range
		if (_landUnitData->hasVariable("age_class_range")) {
			age_class_range = _landUnitData->getVariable("age_class_range")->value();
		}

		int age_maximum = 300; //default maximum age
		if (_landUnitData->hasVariable("age_maximum")) {
			age_maximum = _landUnitData->getVariable("age_maximum")->value();
		}

		number_of_age_classes = age_maximum / age_class_range;
    }

    void CBMAggregatorLandUnitData::doOutputStep() {
        recordLandUnitData(false);
    }

}}} // namespace moja::modules::cbm
