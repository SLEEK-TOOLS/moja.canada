#ifndef default_param_h
#define default_param_h

namespace Sawtooth {
	namespace Parameter {
		struct DefaultGrowthParameter {
			double G_Int;
			double G_LnB;
			double G_B;
			double G_AS;
			double G_BS;
			double G_NS;
			double G_LnBxBS;
			double G_LnB_mu;
			double G_B_mu;
			double G_AS_mu;
			double G_BS_mu;
			double G_NS_mu;
			double G_LnB_sig;
			double G_B_sig;
			double G_AS_sig;
			double G_BS_sig;
			double G_NS_sig;
			double G_LogCorrection;
			DefaultGrowthParameter() {}
			DefaultGrowthParameter(std::map<std::string, double> values) {
				G_Int = values.at("G_Int");
				G_LnB = values.at("G_LnB");
				G_B = values.at("G_B");
				G_AS = values.at("G_AS");
				G_BS = values.at("G_BS");
				G_NS = values.at("G_NS");
				G_LnBxBS = values.at("G_LnBxBS");
				G_LnB_mu = values.at("G_LnB_mu");
				G_B_mu = values.at("G_B_mu");
				G_AS_mu = values.at("G_AS_mu");
				G_BS_mu = values.at("G_BS_mu");
				G_NS_mu = values.at("G_NS_mu");
				G_LnB_sig = values.at("G_LnB_sig");
				G_B_sig = values.at("G_B_sig");
				G_AS_sig = values.at("G_AS_sig");
				G_BS_sig = values.at("G_BS_sig");
				G_NS_sig = values.at("G_NS_sig");
				G_LogCorrection = values.at("G_LogCorrection");
			}
		};
		struct DefaultRecruitmentParameter {
			double R_Int;
			double R_BS;
			double R_AS;
			double R_AS2;
			double R_BS_mu;
			double R_AS_mu;
			double R_AS2_mu;
			double R_BS_sig;
			double R_AS_sig;
			double R_AS2_sig;
			DefaultRecruitmentParameter() {}
			DefaultRecruitmentParameter(std::map<std::string, double> values) {
				R_Int = values.at("R_Int");
				R_BS = values.at("R_BS");
				R_AS = values.at("R_AS");
				R_AS2 = values.at("R_AS2");
				R_BS_mu = values.at("R_BS_mu");
				R_AS_mu = values.at("R_AS_mu");
				R_AS2_mu = values.at("R_AS2_mu");
				R_BS_sig = values.at("R_BS_sig");
				R_AS_sig = values.at("R_AS_sig");
				R_AS2_sig = values.at("R_AS2_sig");
			}
		};

		struct DefaultMortalityParameter {
			double M_Int;
			double M_B;
			double M_B2;
			double M_AS;
			double M_BS;
			double M_BxBS;
			double M_B_mu;
			double M_B2_mu;
			double M_AS_mu;
			double M_BS_mu;
			double M_B_sig;
			double M_B2_sig;
			double M_AS_sig;
			double M_BS_sig;
			DefaultMortalityParameter() {}
			DefaultMortalityParameter(std::map<std::string, double> values) {
				M_Int = values.at("M_Int");
				M_B = values.at("M_B");
				M_B2 = values.at("M_B2");
				M_AS = values.at("M_AS");
				M_BS = values.at("M_BS");
				M_BxBS = values.at("M_BxBS");
				M_B_mu = values.at("M_B_mu");
				M_B2_mu = values.at("M_B2_mu");
				M_AS_mu = values.at("M_AS_mu");
				M_BS_mu = values.at("M_BS_mu");
				M_B_sig = values.at("M_B_sig");
				M_B2_sig = values.at("M_B2_sig");
				M_AS_sig = values.at("M_AS_sig");
				M_BS_sig = values.at("M_BS_sig");
			}
		};
	}
}
#endif // !default_param_h

