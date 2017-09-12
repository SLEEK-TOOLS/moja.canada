#ifndef MOJA_MODULES_CBM_CBMDISTURBANCEEVENTMODULE_H_
#define MOJA_MODULES_CBM_CBMDISTURBANCEEVENTMODULE_H_

#include "moja/modules/cbm/cbmmodulebase.h"
#include "moja/hash.h"

#include <unordered_map>
#include <unordered_set>

namespace moja {
namespace modules {
namespace cbm {
	
	class CBMDistEventRef {
	public:
		CBMDistEventRef() = default;
		explicit CBMDistEventRef(
			std::string& disturbanceType, int dmId, int year,
			int transitionId, const std::string& landClassTransition = "") :
				_disturbanceType(disturbanceType), _disturbanceMatrixId(dmId), _year(year),
				_transitionRuleId(transitionId), _landClassTransition(landClassTransition) {

			if (landClassTransition != "") {
				_hasLandClassTransition = true;
			}
		}

		std::string disturbanceType() const { return _disturbanceType; }
        int transitionRuleId() const { return _transitionRuleId; }
		int disturbanceMatrixId() const { return _disturbanceMatrixId; }
		double year() const { return _year; }
		std::string landClassTransition() const { return _landClassTransition; }
		bool hasLandClassTransition() const { return _hasLandClassTransition; }

	private:
		std::string _disturbanceType;
		int _disturbanceMatrixId;
        int _transitionRuleId;
		int	_year;
		bool _hasLandClassTransition = false;
		std::string _landClassTransition;
	};

	class CBMDistEventTransfer {
	public:
		typedef std::unique_ptr<CBMDistEventTransfer> UniquePtr;
		typedef std::shared_ptr<CBMDistEventTransfer> Ptr;

		CBMDistEventTransfer() = default;

		CBMDistEventTransfer(flint::ILandUnitDataWrapper& landUnitData, const DynamicObject& data) :			
			_disturbanceMatrixId(data["disturbance_matrix_id"]),
			_sourcePool(landUnitData.getPool(data["source_pool_name"].convert<std::string>())), 
			_destPool(landUnitData.getPool(data["dest_pool_name"].convert<std::string>())),
			_proportion(data["proportion"]) { }

		CBMDistEventTransfer(flint::ILandUnitDataWrapper& landUnitData, const std::string& sourcePool, 
			                 const std::string& destPool, double proportion) :			
			_sourcePool(landUnitData.getPool(sourcePool)),
			_destPool(landUnitData.getPool(destPool)),
			_proportion(proportion) { }

		int disturbanceMatrixId() const { return _disturbanceMatrixId; }
		flint::IPool::ConstPtr sourcePool() const { return _sourcePool; }
		flint::IPool::ConstPtr destPool() const { return _destPool; }
		double proportion() const { return _proportion; }

	private:		
		int _disturbanceMatrixId;
		flint::IPool::ConstPtr _sourcePool;
		flint::IPool::ConstPtr _destPool;
		double _proportion;
	};

	class CBMDisturbanceEventModule : public CBMModuleBase {
	public:
		CBMDisturbanceEventModule() : CBMModuleBase() {}
		virtual ~CBMDisturbanceEventModule() = default;

		void configure(const DynamicObject& config) override;
		void subscribe(NotificationCenter& notificationCenter) override;

		flint::ModuleTypes moduleType() { return flint::ModuleTypes::DisturbanceEvent; };

		virtual void doDisturbanceEvent(Dynamic) override;
		virtual void doLocalDomainInit() override;
		virtual void doSystemShutdown() override;
		virtual void doTimingInit() override;
		virtual void doTimingStep() override;

	private:
		typedef std::vector<CBMDistEventTransfer::Ptr> EventVector;
		typedef std::unordered_map<int, EventVector> EventMap;

		NotificationCenter* _notificationCenter;
		std::vector<std::string> _layerNames;
		std::vector<const flint::IVariable*> _layers;
		flint::IVariable* _landClass;
		flint::IVariable* _spu;
		EventMap _matrices;
		std::unordered_map<std::pair<std::string, int>, int> _dmAssociations;
		std::unordered_map<std::string, std::string> _landClassTransitions;
		std::vector<CBMDistEventRef> _landUnitEvents;
		std::unordered_map<std::string, int> _distTypeCodes;
		std::unordered_set<std::string> _errorLayers;

		flint::IPool::ConstPtr _softwoodMerch;
		flint::IPool::ConstPtr _softwoodOther;
		flint::IPool::ConstPtr _softwoodFoliage;
		flint::IPool::ConstPtr _softwoodCoarseRoots;
		flint::IPool::ConstPtr _softwoodFineRoots;

		flint::IPool::ConstPtr _hardwoodMerch;
		flint::IPool::ConstPtr _hardwoodOther;
		flint::IPool::ConstPtr _hardwoodFoliage;
		flint::IPool::ConstPtr _hardwoodCoarseRoots;
		flint::IPool::ConstPtr _hardwoodFineRoots;

		void fetchMatrices();
		void fetchDMAssociations();
		void fetchLandClassTransitions();
		void fetchDistTypeCodes();
		bool addLandUnitEvent(const Dynamic& ev);
	};

}}} // namespace moja::modules::cbm
#endif // MOJA_MODULES_CBM_CBMDISTURBANCEEVENTMODULE_H_
