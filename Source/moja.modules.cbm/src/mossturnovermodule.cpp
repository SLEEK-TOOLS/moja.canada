#include "moja/flint/variable.h"

#include "moja/modules/cbm/mossturnovermodule.h"

namespace moja {
namespace modules {
namespace cbm {
    
	MossTurnoverModule::MossTurnoverModule(){}   

	void MossTurnoverModule::configure(const DynamicObject& config) { }

	void MossTurnoverModule::subscribe(NotificationCenter& notificationCenter) {
		notificationCenter.subscribe(signals::LocalDomainInit	, &MossTurnoverModule::onLocalDomainInit, *this);
		notificationCenter.subscribe(signals::TimingInit		, &MossTurnoverModule::onTimingInit, *this);
		notificationCenter.subscribe(signals::TimingStep		, &MossTurnoverModule::onTimingStep, *this);
	}

	void MossTurnoverModule::doLocalDomainInit() {		
		_featherMossLive = _landUnitData->getPool("FeatherMossLive");
		_sphagnumMossLive = _landUnitData->getPool("SphagnumMossLive");
		_featherMossFast = _landUnitData->getPool("FeatherMossFast");
		_sphagnumMossFast = _landUnitData->getPool("SphagnumMossFast");

		_mossParameters = _landUnitData->getVariable("Moss_Parameters");
		const auto& mossGrowthParameters = _mossParameters->value().extract<DynamicObject>();

		fmlTurnoverRate = mossGrowthParameters["fmlTurnoverRate"];
		smlTurnoverRate = mossGrowthParameters["smlTurnoverRate"];			
	};


	void MossTurnoverModule::doTimingInit() {
		runMoss = _landUnitData->getVariable("run_moss")->value();	
		bool isPeatland = _landUnitData->getVariable("run_peatland")->value();
		int peatlandId = _landUnitData->getVariable("peatlandId")->value();
		_skipForPeatland = (isPeatland && (peatlandId == 1 || peatlandId == 2 || peatlandId == 3));
	};

	void MossTurnoverModule::doTimingStep() {
		if (_skipForPeatland){
			return;
		}
		if (runMoss){			
			doLiveMossTurnover();			
		}
	};

	//Moss turnover (moss live pool to moss fast pool)
	//FeatherMossLive -> FeatherMossFast
	//SphagnumMossLive -> SphagnumMossFast
	void MossTurnoverModule::doLiveMossTurnover() {		
		auto MossTurnoverModule = _landUnitData->createStockOperation();

		double featherMossTurnoverModuleAmount = _featherMossLive->value() * fmlTurnoverRate;
		double sphagnumMossTurnoverModuleAmount = _sphagnumMossLive->value() * smlTurnoverRate;		

		MossTurnoverModule->addTransfer(_featherMossLive, _featherMossFast, featherMossTurnoverModuleAmount);
		MossTurnoverModule->addTransfer(_sphagnumMossLive, _sphagnumMossFast, sphagnumMossTurnoverModuleAmount);

		_landUnitData->submitOperation(MossTurnoverModule);	
	}
}}}