#ifndef MOJA_MODULES_CBM_MOSSDECAY_H_
#define MOJA_MODULES_CBM_MOSSDECAY_H_

#include "moja/modules/cbm/_modules.cbm_exports.h"
#include "moja/flint/modulebase.h"
#include "moja/modules/cbm/standgrowthcurvefactory.h"

namespace moja {
namespace modules {
namespace cbm {

	/// <summary>
	/// Parameters for moss related computing.
	/// </summary>	
	class CBM_API MossDecayModule : public moja::flint::ModuleBase {
	public:    
		MossDecayModule(std::shared_ptr<StandGrowthCurveFactory> gcFactory)
			: _gcFactory(gcFactory) {};

		virtual ~MossDecayModule() = default;

		void configure(const DynamicObject& config) override;
		void subscribe(NotificationCenter& notificationCenter) override;

		flint::ModuleTypes ModuleType() override { return flint::ModuleTypes::Model; };

		void onLocalDomainInit() override;
		void onTimingInit() override;
		void onTimingStep() override;


	private:
		std::shared_ptr<StandGrowthCurveFactory> _gcFactory;

		flint::IVariable* _mossParameters;

		flint::IPool::ConstPtr _featherMossFast;
		flint::IPool::ConstPtr _sphagnumMossFast;
		flint::IPool::ConstPtr _featherMossSlow;
		flint::IPool::ConstPtr _sphagnumMossSlow;
		flint::IPool::ConstPtr _CO2;	

		bool runMoss;

		double kff;		//base decay rate feather moss fast pool          
		double ksf;		//base decay rate sphagnum fast pool              
		double kfs;		//base decay rate feather moss slow pool          
		double kss;		//sphagnum slow pool base decay rate   

		double q10;		//Q10 temperature coefficient
		double tref;	//reference temperature     

		double akff;	//applied feather moss fast pool applied decay rate     
		double akfs;	//applied feather moss slow pool applied decay rate     
		double aksf;	//applied sphagnum fast pool applied decay rate         
		double akss;	//applied sphagnum slow pool applied decay rate    

		double m;	 //parameter for F6                                  
		double n;	 //parameter for F6  

		double fastToSlowTurnoverRate;	//fast moss pool to slow moss pool turnover rate
		double fastToAirDecayRate;		//fast moss pool to CO2 air pool rate 

		double meanAnnualTemperature;
		Int64 currentStandGCId;

		//Sphagnum slow pool base decay rate, kss = m*ln(maxVolume) + n
		double F6(double m, double n, double maxVolume);

		//Applied decay rate to all moss pools: kff, kfs, ksf, kss
		//kff - feather fast decay rate
		//kfs - feather slow decay rate
		//ksf - sphagnum fast decay rate
		//kss - sphagnum slow decay rate
		double F7(double baseDecayRate, double meanAnnualTemperature, double q10);

		void updateMossAppliedDecayParameters(double standMaximumVolume, double meanAnnualTemperature);

		void doMossFastPoolDecay();

		void doMossSlowPoolDecay();
	};

}}}
#endif
