#include "moja/modules/cbm/cbmdisturbancelistener.h"

#include <moja/flint/ivariable.h>
#include <moja/flint/ioperation.h>
#include <moja/flint/itiming.h>

#include <moja/logging.h>
#include <moja/signals.h>
#include <moja/notificationcenter.h>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

namespace moja {
namespace modules {
namespace cbm {

    void CBMDisturbanceListener::configure(const DynamicObject& config) {
        auto layerNames = config["vars"];
        if (layerNames.size() == 0 || layerNames.isEmpty()) {
            return;
        }

        for (const auto& layerName : layerNames) {
            _layerNames.push_back(layerName);
        }
    }

    void CBMDisturbanceListener::subscribe(NotificationCenter& notificationCenter) {
		_notificationCenter = &notificationCenter;
		notificationCenter.subscribe(signals::LocalDomainInit,	&CBMDisturbanceListener::onLocalDomainInit,	*this);
		notificationCenter.subscribe(signals::SystemShutdown,	&CBMDisturbanceListener::onSystemShutdown,	*this);
		notificationCenter.subscribe(signals::TimingInit,		&CBMDisturbanceListener::onTimingInit,		*this);
		notificationCenter.subscribe(signals::TimingStep,		&CBMDisturbanceListener::onTimingStep,		*this);
	}

    void CBMDisturbanceListener::doLocalDomainInit() {
        for (const auto& layerName : _layerNames) {
            _layers.push_back(_landUnitData->getVariable(layerName));
        }

        fetchMatrices();
        fetchDMAssociations();
        fetchLandClassTransitions();
		fetchDistTypeCodes();

        _landClass = _landUnitData->getVariable("current_land_class");
        _spu = _landUnitData->getVariable("spatial_unit_id");
    }

	void CBMDisturbanceListener::doSystemShutdown() {
		for (const auto& layerName : _errorLayers) {
			MOJA_LOG_DEBUG << (boost::format(
				"Disturbance layer '%1%' is not in the expected format. "
                "Check if the layer is empty or missing its attribute table."
			) % layerName).str();
		}
	}

    void CBMDisturbanceListener::doTimingInit() {
        _landUnitEvents.clear();
        // Pre-load every disturbance event for this land unit.
		for (const auto layer : _layers) {
			const auto& events = layer->value();
			if (events.isEmpty()) {
				continue;
			}

			bool success = true;
			if (events.isVector()) {
				for (const auto& event : events.extract<const std::vector<DynamicObject>>()) {
					success = addLandUnitEvent(event);
				}
			}
			else {
				success = addLandUnitEvent(events);
			}

			if (!success) {
				_errorLayers.insert(layer->info().name);
			}
        }
    }

	std::string CBMDisturbanceListener::getDisturbanceTypeName(const DynamicObject& eventData) {
		if (eventData.contains("disturbance_type")) {
			std::string name = eventData["disturbance_type"].extract<std::string>();
			if (eventData.contains("disturbance_type_id")) {
				// Both id and name have been specified, better check it just in case.
				int id = eventData["disturbance_type_id"].extract<int>();
				auto match = _distTypeNames.find(id);
				if (match == _distTypeNames.end()) {
					MOJA_LOG_FATAL << (boost::format(
						"specified disturbance type id (%1%) not found")
						% id).str();
				}
				if (match->second != name) {
					MOJA_LOG_FATAL << (boost::format(
						"specified disturbance type id (%1%) does not correspond to specified disturbance type name (%2%)")
						% id % name).str();
				}
			}
			return name;
		}
		else if (eventData.contains("disturbance_type_id")) {
			int id = eventData["disturbance_type_id"];
			auto match = _distTypeNames.find(id);
			if (match == _distTypeNames.end()) {
				MOJA_LOG_FATAL << (boost::format(
					"specified disturbance type id (%1%) not found")
					% id).str();
			}
			return match->second;
		}

        MOJA_LOG_FATAL << "disturbance event must specify either name or disturbance id";
		return "";
	}

	bool CBMDisturbanceListener::addLandUnitEvent(const DynamicVar& eventData) {
		if (!eventData.isStruct()) {
			return false;
		}

		const auto& event = eventData.extract<DynamicObject>();

		auto disturbanceType = getDisturbanceTypeName(event);
		int year = event["year"];

		int spu = _spu->value();
		auto key = std::make_pair(disturbanceType, spu);
		const auto& dm = _dmAssociations.find(key);
		if (dm == _dmAssociations.end()) {
			MOJA_LOG_FATAL << (boost::format(
				"Missing DM association for dist type %1% in SPU %2%")
				% disturbanceType % spu).str();
            return false;
		}

		auto dmId = dm->second;

		const auto& it = _landClassTransitions.find(disturbanceType);
		std::string landClass = it != _landClassTransitions.end() ? (*it).second : "";

		int transitionId = -1;
		if (event.contains("transition") && !event["transition"].isEmpty()) {
			transitionId = event["transition"];
		}

        std::vector<std::shared_ptr<IDisturbanceCondition>> conditions;
        if (event.contains("conditions") && !event["conditions"].isEmpty()) {
            for (const auto& condition : event["conditions"]) {
                std::string varName = condition[0];
                auto targetType = condition[1] == "<" ? DisturbanceConditionType::LessThan
                                : condition[1] == ">=" ? DisturbanceConditionType::AtLeast
                                : DisturbanceConditionType::EqualTo;
                DynamicVar target = condition[2];
                conditions.push_back(std::make_shared<VariableDisturbanceCondition>(
                    _landUnitData->getVariable(varName), targetType, target));
            }
        }

		_landUnitEvents[year].push_back(CBMDistEventRef(
            disturbanceType, dmId, year, transitionId, landClass, conditions, event));

		return true;
	}
    
    void CBMDisturbanceListener::doTimingStep() {
        // Load the LU disturbance event for this time/location and apply the moves defined.
        const auto& timing = _landUnitData->timing();
        auto currentYear = timing->curStartDate().year();

        for (auto& e : _landUnitEvents[currentYear]) {
            if (!e.checkConditions()) {
                MOJA_LOG_DEBUG << (boost::format("Conditions not met for %1% in %2% - skipped")
                    % e.disturbanceType() % currentYear).str();

                continue;
            }

			if (e.hasLandClassTransition()) {
				_landClass->set_value(e.landClassTransition());
			}
								
			int disturbanceTypeCode = -1;
			const auto& code = _distTypeCodes.find(e.disturbanceType());
			if (code != _distTypeCodes.end()) {
				disturbanceTypeCode = code->second;
			}
				
			// Check if event is fire disturbance.
			std::string eventType = code->first;
			std::string eventType_lower = boost::algorithm::to_lower_copy(eventType);;
			bool isFire = boost::contains(eventType_lower, "fire");
			
			// Check if running on peatland.
			bool runPeatland = _landUnitData->getVariable("run_peatland")->value();

			// Create a vector to store all of the transfers for this event.
			auto distMatrix = std::make_shared<std::vector<CBMDistEventTransfer>>();
			
			if (!runPeatland || !isFire) {
				// Add CBM DM for all non-fire events.
				// Add CBM fire DM for non-peatland event.
				int dmId = e.disturbanceMatrixId();
				const auto& it = _matrices.find(dmId);
				const auto& operations = it->second;
				for (const auto& transfer : operations) {
					distMatrix->push_back(CBMDistEventTransfer(transfer));
				}
			}

			auto data = DynamicObject({
				{ "disturbance", e.disturbanceType() },
				{ "disturbance_type_code", disturbanceTypeCode },
				{ "transfers", distMatrix },
				{ "transition", e.transitionRuleId() }
			});

            // Merge any additional metadata into disturbance data.
            for (const auto& item : e.metadata()) {
                if (!data.contains(item.first)) {
                    data[item.first] = item.second;
                }
            }

			// Now fire the disturbance events.
			_notificationCenter->postNotificationWithPostNotification(
				moja::signals::DisturbanceEvent, (DynamicVar)data);
        }
    }    

    void CBMDisturbanceListener::fetchMatrices() {
        _matrices.clear();
        const auto& transfers = _landUnitData->getVariable("disturbance_matrices")->value()
            .extract<const std::vector<DynamicObject>>();

        for (const auto& row : transfers) {
            auto transfer = CBMDistEventTransfer(*_landUnitData, row);
            int dmId = transfer.disturbanceMatrixId();
            const auto& v = _matrices.find(dmId);
            if (v == _matrices.end()) {
                EventVector vec;
                vec.push_back(transfer);
                _matrices.emplace(dmId, vec);
            } else {
                auto& vec = v->second;
                vec.push_back(transfer);
            }
        }
    }

    void CBMDisturbanceListener::fetchDMAssociations() {
        _dmAssociations.clear();
        const auto& dmAssociations = _landUnitData->getVariable("disturbance_matrix_associations")->value()
            .extract<const std::vector<DynamicObject>>();

        for (const auto& dmAssociation : dmAssociations) {
            std::string disturbanceType = dmAssociation["disturbance_type"];
            int spu = dmAssociation["spatial_unit_id"];
            int dmId = dmAssociation["disturbance_matrix_id"];
            _dmAssociations.insert(std::make_pair(
                std::make_pair(disturbanceType, spu),
                dmId));
        }
    }

    void CBMDisturbanceListener::fetchLandClassTransitions() {
        const auto& transitions = _landUnitData->getVariable("land_class_transitions")->value();
        if (transitions.isVector()) {
            for (const auto& transition : transitions.extract<const std::vector<DynamicObject>>()) {
                std::string disturbanceType = transition["disturbance_type"];
                std::string landClass = transition["land_class_transition"];
                _landClassTransitions.insert(std::make_pair(disturbanceType, landClass));
            }
        } else {
            std::string disturbanceType = transitions["disturbance_type"];
            std::string landClass = transitions["land_class_transition"];
            _landClassTransitions.insert(std::make_pair(disturbanceType, landClass));
        }
    }

	void CBMDisturbanceListener::fetchDistTypeCodes() {
		if (!_landUnitData->hasVariable("disturbance_type_codes")) {
			return;
		}

		const auto& distTypeCodes = _landUnitData->getVariable("disturbance_type_codes")->value();
		if (distTypeCodes.isVector()) {
			for (const auto& code : distTypeCodes.extract<const std::vector<DynamicObject>>()) {
				std::string distType = code["disturbance_type"];
				int distTypeCode = code["disturbance_type_code"];
				_distTypeCodes[distType] = distTypeCode;
				_distTypeNames[distTypeCode] = distType;
			}
		} else {
			std::string distType = distTypeCodes["disturbance_type"];
			int distTypeCode = distTypeCodes["disturbance_type_code"];
			_distTypeCodes[distType] = distTypeCode;
			_distTypeNames[distTypeCode] = distType;
		}
	}

}}} // namespace moja::modules::cbm
