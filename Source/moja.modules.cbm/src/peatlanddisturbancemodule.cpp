#include "moja/modules/cbm/peatlanddisturbancemodule.h"

#include <moja/flint/variable.h>

#include <moja/signals.h>
#include <moja/notificationcenter.h>

#include <boost/algorithm/string.hpp> 
#include <boost/algorithm/string/join.hpp>

#include <moja/logging.h>

namespace moja {
namespace modules {
namespace cbm {

    void PeatlandDisturbanceModule::configure(const DynamicObject& config) { }

    void PeatlandDisturbanceModule::subscribe(NotificationCenter& notificationCenter) {
        notificationCenter.subscribe(signals::LocalDomainInit,  &PeatlandDisturbanceModule::onLocalDomainInit,  *this);
        notificationCenter.subscribe(signals::DisturbanceEvent, &PeatlandDisturbanceModule::onDisturbanceEvent, *this);
		notificationCenter.subscribe(signals::TimingInit,       &PeatlandDisturbanceModule::onTimingInit,       *this);
	}

   
	void PeatlandDisturbanceModule::doLocalDomainInit() { 		
        _spu = _landUnitData->getVariable("spatial_unit_id");
		_run_peatland = _landUnitData->getVariable("run_peatland");

		fetchPeatlandDistMatrices();
		fetchPeatlandDMAssociations();
		fetchPeatlandDistModifiers();
    }

    void PeatlandDisturbanceModule::doTimingInit() {			
		_runPeatland = _run_peatland->value();
        _spuId = _spu->value();		
    }

	void PeatlandDisturbanceModule::doDisturbanceEvent(DynamicVar n) {
		if (!_runPeatland){ return; }

		auto& data = n.extract<const DynamicObject>();

		// Get the disturbance type.
		std::string disturbanceType = data["disturbance"];	
		
		//check if it is fire disturbance containg "fire" in disturbance type name		
		std::string disturbanceType_lower = boost::algorithm::to_lower_copy(disturbanceType);;
		//TODO, future task to determine fire disturbace generally		
		bool isFire = boost::contains(disturbanceType_lower, "fire");

		if (_runPeatland && isFire) {
			auto distMatrix = data["transfers"].extract<std::shared_ptr<std::vector<CBMDistEventTransfer>>>();
			int peatlandId = _landUnitData->getVariable("peatlandId")->value();

			const auto& dmAssociation = _dmAssociations.find(peatlandId);
			const auto& dmIDandWtdModifer = dmAssociation->second;
			
			int dmId = dmIDandWtdModifer.first;
			int wtdModifierId = dmIDandWtdModifer.second;
			modifierVector vec = _modifiers.find(wtdModifierId)->second;
			std::string modifiers = boost::algorithm::join(vec, ";");
			_landUnitData->getVariable("peatland_annual_wtd_modifiers")->set_value(modifiers);

			const auto& it = _matrices.find(dmId);			
			const auto& operations = it->second;
			for (const auto& transfer : operations) {
				distMatrix->push_back(CBMDistEventTransfer(transfer));
			}
		}
    } 

	void PeatlandDisturbanceModule::fetchPeatlandDistMatrices() {
		_matrices.clear();
		const auto& transfers = _landUnitData->getVariable("peatland_disturbance_matrices")->value()
			.extract<const std::vector<DynamicObject>>();

		for (const auto& row : transfers) {
			auto transfer = CBMDistEventTransfer(*_landUnitData, row);
			int dmId = transfer.disturbanceMatrixId();
			const auto& v = _matrices.find(dmId);
			if (v == _matrices.end()) {
				EventVector vec;
				vec.push_back(transfer);
				_matrices.emplace(dmId, vec);
			}
			else {
				auto& vec = v->second;
				vec.push_back(transfer);
			}
		}
	}

	void PeatlandDisturbanceModule::fetchPeatlandDMAssociations() {
		_dmAssociations.clear();
		const auto& dmAssociations = _landUnitData->getVariable("peatland_fire_dm_associations")->value()
			.extract<const std::vector<DynamicObject>>();

		for (const auto& dmAssociation : dmAssociations) {
			int peatlandId = dmAssociation["peatland_id"];			
			int dmId = dmAssociation["peatland_dm_id"];
			int wtdModifierId = dmAssociation["wtd_modifier_id"];
			_dmAssociations.insert (std::make_pair(
				peatlandId, std::make_pair(dmId, wtdModifierId)));
		}
	}

	void PeatlandDisturbanceModule::fetchPeatlandDistModifiers() {
		_modifiers.clear();
		const auto& modifierList = _landUnitData->getVariable("peatland_wtd_modifiers")->value()
			.extract<const std::vector<DynamicObject>>();

		for (const auto& row : modifierList) {
			int modifierId = row["id"];
			int year = row["year"];
			int modifier = row["modifier"];

			const auto& v = _modifiers.find(modifierId);
			if (v == _modifiers.end()) {
				modifierVector vec;
				//each modifier will be recorded as year_modifier
				vec.push_back(std::to_string(year) + "_" + std::to_string(modifier));
				_modifiers.emplace(modifierId, vec);
			}
			else {
				auto& vec = v->second;
				vec.push_back(std::to_string(year) + "_" + std::to_string(modifier));
			}
		}
    } 
}}}
