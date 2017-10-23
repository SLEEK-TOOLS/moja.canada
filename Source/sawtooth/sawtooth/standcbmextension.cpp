#include "standcbmextension.h"
namespace Sawtooth {
	namespace CBMExtension {

		Sawtooth_CBMBiomassPools StandCBMExtension::ComputeLitterFalls(
			const Parameter::CBM::TurnoverParameter& t, 
			const Sawtooth_CBMBiomassPools& biomass) {
			Sawtooth_CBMBiomassPools result;

			result.SWM = biomass.SWM * t.StemAnnualTurnoverRate;
			result.SWO = biomass.SWO * t.SoftwoodBranchTurnoverRate;
			result.SWF = biomass.SWF * t.SoftwoodFoliageFallRate;
			result.SWFR = biomass.SWFR * t.FineRootTurnProp;
			result.SWCR = biomass.SWCR * t.CoarseRootTurnProp;

			result.HWM = biomass.HWM * t.StemAnnualTurnoverRate;
			result.HWO = biomass.HWO * t.HardwoodBranchTurnoverRate;
			result.HWF = biomass.HWF * t.HardwoodFoliageFallRate;
			result.HWFR = biomass.HWFR * t.FineRootTurnProp;
			result.HWCR = biomass.HWCR * t.CoarseRootTurnProp;

			return result;
		}

		Sawtooth_CBMBiomassPools StandCBMExtension::PartitionAboveGroundC(
			C_AG_Source source,
			const Stand& stand,
			const Parameter::CBM::StumpParameter& stump,
			const Parameter::CBM::RootParameter& rootParam) {

			Sawtooth_CBMBiomassPools pools;
			for (auto species : stand.UniqueSpecies()) {
				const auto sp = Parameters.GetSpeciesParameter(species);
				auto deciduous = sp->DeciduousFlag;
				double biomassC_utilizationLevel = Parameters
					.GetBiomassCUtilizationLevel(stand.GetRegionId(), species) / 1000.0;
				switch (source)
				{
				case Sawtooth::CBMExtension::Live:
					for (auto ilive : stand.iLive(species)) {
						double C_ag = stand.C_ag(ilive) / stand.Area() / 1000.0;
						Partition(pools, deciduous, C_ag, sp->Cag2Cf1,
							sp->Cag2Cf2, sp->Cag2Cbk1, sp->Cag2Cbk2,
							sp->Cag2Cbr1, sp->Cag2Cbr2,
							biomassC_utilizationLevel, stump, rootParam);
					}
					break;
				case Sawtooth::CBMExtension::LivePreGrowth:
					for (auto ilive : stand.iLive(species)) {
						double C_ag = (stand.C_ag(ilive)-stand.C_ag_g(ilive))
							/ stand.Area() / 1000.0;
						Partition(pools, deciduous, C_ag, sp->Cag2Cf1,
							sp->Cag2Cf2, sp->Cag2Cbk1, sp->Cag2Cbk2,
							sp->Cag2Cbr1, sp->Cag2Cbr2,
							biomassC_utilizationLevel, stump, rootParam);
					}
					break;
				case Sawtooth::CBMExtension::AnnualMortality:
					for (auto iDead : stand.iDead(species)) {
						double C_ag = stand.Mortality_C_ag(iDead) / stand.Area() / 1000.0;
						Partition(pools, deciduous, C_ag, sp->Cag2Cf1,
							sp->Cag2Cf2, sp->Cag2Cbk1, sp->Cag2Cbk2,
							sp->Cag2Cbr1, sp->Cag2Cbr2,
							biomassC_utilizationLevel, stump, rootParam);
					}
					break;
				case Sawtooth::CBMExtension::DisturbanceMortality:
					for (auto iDead : stand.iDead(species)) {
						double C_ag = stand.Disturbance_C_ag(iDead) / stand.Area() / 1000.0;
						Partition(pools, deciduous, C_ag, sp->Cag2Cf1,
							sp->Cag2Cf2, sp->Cag2Cbk1, sp->Cag2Cbk2,
							sp->Cag2Cbr1, sp->Cag2Cbr2,
							biomassC_utilizationLevel, stump, rootParam);
					}
					break;
				default:
					throw std::invalid_argument("specified source not valid");
				}
			}
			return pools;
		}

		void StandCBMExtension::Partition(Sawtooth_CBMBiomassPools& result,
			int deciduous, double C_ag, double Cag2Cf1, double Cag2Cf2,
			double Cag2Cbk1, double Cag2Cbk2, double Cag2Cbr1, double Cag2Cbr2,
			double biomassC_utilizationLevel,
			const Parameter::CBM::StumpParameter& stump,
			const Parameter::CBM::RootParameter& rootParam) {

			if (C_ag == 0.0) { return; }

			double SWFoliageC = 0.0;
			double SWBarkC = 0.0;
			double SWBranchC = 0.0;
			double SWStemMerchC = 0.0;
			double SWStemNonMerchC = 0.0;
			double SWFineRootC = 0.0;
			double SWCoarseRootC = 0.0;
			double HWFoliageC = 0.0;
			double HWBarkC = 0.0;
			double HWBranchC = 0.0;
			double HWStemMerchC = 0.0;
			double HWStemNonMerchC = 0.0;
			double HWFineRootC = 0.0;
			double HWCoarseRootC = 0.0;

			double C_ag_hw = 0;
			double C_ag_sw = 0;

			if (deciduous) { //group C_ag according to species flag
				C_ag_hw = C_ag;
				HWFoliageC = C_ag * Cag2Cf1 * std::pow(C_ag, Cag2Cf2);
				HWBarkC = C_ag * Cag2Cbk1 * std::pow(C_ag, Cag2Cbk2);
				HWBranchC = C_ag * Cag2Cbr1 * std::pow(C_ag, Cag2Cbr2);
			}
			else {
				C_ag_sw = C_ag;
				SWFoliageC = C_ag * Cag2Cf1 * std::pow(C_ag, Cag2Cf2);
				SWBarkC = C_ag * Cag2Cbk1 * std::pow(C_ag, Cag2Cbk2);
				SWBranchC = C_ag * Cag2Cbr1 * std::pow(C_ag, Cag2Cbr2);
			}

			//group by merch/non-merch
			if (C_ag >= biomassC_utilizationLevel) { 
				//merch stems (less the top and stump) go into the merch C pool
				HWStemMerchC = C_ag_hw - HWFoliageC - HWBarkC - HWBranchC;
				SWStemMerchC = C_ag_sw - SWFoliageC - SWBarkC - SWBranchC;
			}
			else {
				//non-merch stems go into the other pool
				HWStemNonMerchC = C_ag_hw - HWFoliageC - HWBarkC - HWBranchC;
				SWStemNonMerchC = C_ag_sw - SWFoliageC - SWBarkC - SWBranchC;
			}

			double totalRootBioHW = rootParam.hw_a *
				std::pow(C_ag_hw / rootParam.biomass_to_carbon, rootParam.hw_b);
			double totalRootBioSW = rootParam.sw_a * C_ag_sw / rootParam.biomass_to_carbon;
			double fineRootPortion = rootParam.frp_a + rootParam.frp_b *
				std::exp(rootParam.frp_c * (totalRootBioHW + totalRootBioSW));

			SWCoarseRootC = totalRootBioSW * (1 - fineRootPortion) *  rootParam.biomass_to_carbon;
			SWFineRootC = totalRootBioSW * fineRootPortion *  rootParam.biomass_to_carbon;
			HWCoarseRootC = totalRootBioHW * (1 - fineRootPortion) *  rootParam.biomass_to_carbon;
			HWFineRootC = totalRootBioHW * fineRootPortion *  rootParam.biomass_to_carbon;

			//find the top and stump using the stump/proportions versus the merchantable stem
			double swTopAndStump = SWStemMerchC * (stump.sw_stump_proportion + stump.sw_top_proportion) * 0.01;
			double hwTopAndStump = HWStemMerchC * (stump.hw_stump_proportion + stump.hw_top_proportion) * 0.01;

			//accumulate the resulting components
			result.SWM += SWStemMerchC - swTopAndStump; //deduct top and stump from Merch
			result.SWO += SWBarkC + SWBranchC + SWStemNonMerchC + swTopAndStump; //and move the top and stump to other
			result.SWF += SWFoliageC;
			result.SWFR += SWFineRootC;
			result.SWCR += SWCoarseRootC;

			result.HWM += HWStemMerchC - hwTopAndStump;
			result.HWO += HWBarkC + HWBranchC + hwTopAndStump + HWStemNonMerchC;
			result.HWF += HWFoliageC;
			result.HWFR += HWFineRootC;
			result.HWCR += HWCoarseRootC;
		}

		void StandCBMExtension::PerformDisturbance(Stand& stand,
			Rng::Random& r, int disturbanceType) {

			if (disturbanceType > 0) {
				const auto disturbanceLosses = Parameters
					.GetDisturbanceBiomassLossProportions(stand.GetRegionId(),
						disturbanceType);

				//check if the matrix is "stand replacing" ie. it removes all biomass
				bool standReplacing =
					std::abs(1.0 - disturbanceLosses.SWM) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.SWF) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.SWO) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.SWCR) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.SWFR) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.HWM) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.HWF) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.HWO) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.HWCR) < 0.005 &&
					std::abs(1.0 - disturbanceLosses.HWFR) < 0.005;

				if (standReplacing) {
					// the stand replacing case is easy, all trees are taken
					stand.KillAllTrees(Sawtooth_Disturbance);
				}
				else {
					PartialDisturbance1(disturbanceLosses, stand, r);
				}
			}
		}


		// method for applying CBM partial disturbance matrices to Sawtooth
		// applies merch retained proportion from matrix as a probability of
		// disturbance for given trees
		// benefit: less computation, on average matches CBM matrix proportions
		// drawback: chance of having outliers where biomass lost (as a 
		// proportion of total stand biomass) is drastically different 
		// than what CBM disturbance matrices prescribe
		// see: Proposals 1 and 2 in the partial disturbance matrix section in
		// in: M:\Sawtooth\Code\cppVersion\documentation\ApplyingCBMDisturbanceMatrices.docx
		void  StandCBMExtension::PartialDisturbance1(
			const Sawtooth_CBMBiomassPools& disturbanceLosses,
			Sawtooth::Stand & stand, Sawtooth::Rng::Random & r) {
			//find the CBM loss proportions based on the Sawtooth stand's live
			//biomass pools and the matrix loss proportions
			double p_mortality_sw = std::min(1.0, std::max(0.0, 1.0 - disturbanceLosses.SWM));
			double p_mortality_hw = std::min(1.0, std::max(0.0, 1.0 - disturbanceLosses.HWM));

			const auto ilive_SW = stand.iLive(Parameters.GetSoftwoodSpecies());
			const auto ilive_HW = stand.iLive(Parameters.GetHardwoodSpecies());

			//create vectors of uniformly distributed numbers
			std::vector<double> rn_sw = r.rand(ilive_SW.size());
			std::vector<double> rn_hw = r.rand(ilive_HW.size());
			int k = 0;
			for (auto ilive : ilive_SW) {
				if (rn_sw[k++] < p_mortality_sw) {
					stand.KillTree(ilive, Sawtooth_Disturbance);
				}
			}
			k = 0;
			for (auto ilive : ilive_HW) {
				if (rn_hw[k++] < p_mortality_hw) {
					stand.KillTree(ilive, Sawtooth_Disturbance);
				}
			}
		}

		// method for applying CBM partial disturbance matrices to Sawtooth
		// handles partial disturbances by computing the total that would be removed by the matrix first,
		// then killing trees randomly until the total killed C is close to the partial value
		// benefit: reliably close to the CBM disturbance proportion
		// drawback: more computation
		// see: Proposal 3: Use matrix on stand total C 
		// in: M:\Sawtooth\Code\cppVersion\documentation\ApplyingCBMDisturbanceMatrices.docx
		void StandCBMExtension::PartialDisturbance2(
			const Sawtooth_CBMBiomassPools& disturbanceLosses,
			Sawtooth::Stand & stand, Sawtooth::Rng::Random & r)
		{
			//the matrix is not stand replacing, we need to partially
			//disturb the stand by taking out some of the trees

			const auto stump = Parameters.GetStumpParameter(
				stand.GetStumpParameterId());
			const auto rootParam = Parameters.GetRootParameter(
				stand.GetRootParameterId());

			//compute the stand's CBM biomass pools based on live trees
			Sawtooth_CBMBiomassPools liveBiomass = PartitionAboveGroundC(Live,
				stand, *stump, *rootParam);

			//find the CBM loss proportions based on the Sawtooth stand's live
			//biomass pools and the matrix loss proportions
			double SWLoss = 
				liveBiomass.SWM * disturbanceLosses.SWM +
				liveBiomass.SWF * disturbanceLosses.SWF +
				liveBiomass.SWO * disturbanceLosses.SWO +
				liveBiomass.SWCR * disturbanceLosses.SWCR +
				liveBiomass.SWFR * disturbanceLosses.SWFR;

			double HWLoss = 
				liveBiomass.HWM * disturbanceLosses.HWM +
				liveBiomass.HWF * disturbanceLosses.HWF +
				liveBiomass.HWO * disturbanceLosses.HWO +
				liveBiomass.HWCR * disturbanceLosses.HWCR +
				liveBiomass.HWFR * disturbanceLosses.HWFR;

			//now disturb trees until we are close to the CBM loss proportions
			int softwood = 0;
			int hardwood = 1;

			for (auto forestType : { softwood, hardwood }) {
				double lossTarget = 0;
				std::vector<int> ilive;
				if (forestType == softwood) {
					lossTarget = SWLoss;
					//get the indices to live softwood trees
					ilive = stand.iLive(Parameters.GetSoftwoodSpecies());
				}
				else {
					lossTarget = HWLoss;
					//get the indices to live hardwood trees
					ilive = stand.iLive(Parameters.GetHardwoodSpecies());
				}
				if (lossTarget == 0 || ilive.size() == 0) {
					continue;
				}

				r.shuffle(ilive.begin(), ilive.end());
				double lost = 0.0;
				//iterate over the live trees
				for (auto i : ilive) {
					const auto sp = Parameters.GetSpeciesParameter(
						stand.SpeciesId(i));
					double bioUtilRate = Parameters.GetBiomassCUtilizationLevel(
						stand.GetRegionId(), stand.SpeciesId(i));
					double C_ag = stand.C_ag(i) / stand.Area() / 1000.0;
					
					//partition the tree into CBM pools
					Sawtooth_CBMBiomassPools pools;
					Partition(pools, sp->DeciduousFlag, C_ag, sp->Cag2Cf1,
						sp->Cag2Cf2, sp->Cag2Cbk1, sp->Cag2Cbk2,
						sp->Cag2Cbr1, sp->Cag2Cbr2,
						bioUtilRate, *stump, *rootParam);

					//add the tree to the running total
					lost +=
						pools.SWM +
						pools.SWF +
						pools.SWO +
						pools.SWCR +
						pools.SWFR +
						pools.HWM +
						pools.HWF +
						pools.HWO +
						pools.HWCR +
						pools.HWFR;

					//check if the lost value crosses the target value
					if (lost >= lossTarget) {
						break;
					}
					else {
						//otherwise disturb the tree
						stand.KillTree(i, Sawtooth_Disturbance);
					}
				}
			}
		}

		Sawtooth_CBMAnnualProcesses StandCBMExtension::Compute(
			const Stand& stand) {
			Sawtooth_CBMAnnualProcesses result;

			const auto stump = Parameters.GetStumpParameter(
				stand.GetStumpParameterId());
			const auto rootParam = Parameters.GetRootParameter(
				stand.GetRootParameterId());
			const auto turnover = Parameters.GetTurnoverParameter(
				stand.GetTurnoverParameterId());

			result.Mortality = PartitionAboveGroundC(AnnualMortality, stand,
				*stump, *rootParam);
			Sawtooth_CBMBiomassPools liveBiomass = PartitionAboveGroundC(Live, stand,
				*stump, *rootParam);

			Sawtooth_CBMBiomassPools livePreGrowthBiomass = PartitionAboveGroundC(LivePreGrowth, stand,
				*stump, *rootParam);

			result.NetGrowth = liveBiomass - livePreGrowthBiomass;
			result.Litterfall = ComputeLitterFalls(*turnover, liveBiomass);
			result.GrossGrowth = result.NetGrowth + result.Litterfall + result.Mortality;
			result.Disturbance = PartitionAboveGroundC(DisturbanceMortality,
				stand, *stump, *rootParam);

			return result;
		}
	}
}