#ifndef sawtooth_es3_parameter_h
#define sawtooth_es3_parameter_h
#include <map>
namespace Sawtooth {
	namespace Parameter {
		struct ParameterGrowthES3 {
			double G_Int;
			double G_LnB;
			double G_B;
			double G_AS;
			double G_BLS;
			double G_BS;
			double G_EL1;
			double G_EL2;
			double G_SL1;
			double G_SL2;
			double G_CASL;
			double G_TWI;
			double G_DAI;
			double G_DAP;
			double G_Tc;
			double G_T;
			double G_E;
			double G_W1;
			double G_W2;
			double G_W3;
			double G_N;
			double G_C;
			double G_BLSxEL;
			double G_BLSxSL;
			double G_BLSxCASL;
			double G_BLSxTWI;
			double G_BLSxDAI;
			double G_BLSxDAP;
			double G_BLSxT;
			double G_BLSxE;
			double G_BLSxW;
			double G_BLSxN;
			double G_BLSxC;
			double G_ELxDAI;
			double G_ELxDAP;
			double G_SLxDAI;
			double G_SLxDAP;
			double G_CASLxDAI;
			double G_CASLxDAP;
			double G_TWIxDAI;
			double G_TWIxDAP;
			double G_ELxT;
			double G_ELxE;
			double G_ELxW;
			double G_ELxN;
			double G_ELxC;
			double G_SLxT;
			double G_SLxE;
			double G_SLxW;
			double G_SLxN;
			double G_SLxC;
			double G_CASLxT;
			double G_CASLxE;
			double G_CASLxW;
			double G_CASLxN;
			double G_CASLxC;
			double G_TWIxT;
			double G_TWIxE;
			double G_TWIxW;
			double G_TWIxN;
			double G_TWIxC;
			double G_DAIxT;
			double G_DAIxE;
			double G_DAIxW;
			double G_DAIxN;
			double G_DAIxC;
			double G_DAPxT;
			double G_DAPxE;
			double G_DAPxW;
			double G_DAPxN;
			double G_DAPxC;
			double G_TxE;
			double G_TxW;
			double G_NxT;
			double G_NxT2;
			double G_NxE;
			double G_NxE2;
			double G_NxW;
			double G_NxW2;
			double G_CxT;
			double G_CxT2;
			double G_CxE;
			double G_CxE2;
			double G_CxW;
			double G_CxW2;
			double G_CxN;
			double G_CxN2;
			double G_BLSxELxDAI;
			double G_BLSxSLxDAI;
			double G_BLSxCASLxDAI;
			double G_BLSxTWIxDAI;
			double G_BLSxELxDAP;
			double G_BLSxSLxDAP;
			double G_BLSxCASLxDAP;
			double G_BLSxTWIxDAP;
			double G_BLSxELxT;
			double G_BLSxSLxT;
			double G_BLSxCASLxT;
			double G_BLSxTWIxT;
			double G_BLSxELxE;
			double G_BLSxSLxE;
			double G_BLSxCASLxE;
			double G_BLSxTWIxE;
			double G_BLSxELxW;
			double G_BLSxSLxW;
			double G_BLSxCASLxW;
			double G_BLSxTWIxW;
			double G_BLSxELxN;
			double G_BLSxSLxN;
			double G_BLSxCASLxN;
			double G_BLSxTWIxN;
			double G_BLSxELxC;
			double G_BLSxSLxC;
			double G_BLSxCASLxC;
			double G_BLSxTWIxC;
			double G_ELxDAIxT;
			double G_ELxDAPxT;
			double G_SLxDAIxT;
			double G_SLxDAPxT;
			double G_CASLxDAIxT;
			double G_CASLxDAPxT;
			double G_TWIxDAIxT;
			double G_TWIxDAPxT;
			double G_ELxDAIxE;
			double G_ELxDAPxE;
			double G_SLxDAIxE;
			double G_SLxDAPxE;
			double G_CASLxDAIxE;
			double G_CASLxDAPxE;
			double G_TWIxDAIxE;
			double G_TWIxDAPxE;
			double G_ELxDAIxW;
			double G_ELxDAPxW;
			double G_SLxDAIxW;
			double G_SLxDAPxW;
			double G_CASLxDAIxW;
			double G_CASLxDAPxW;
			double G_TWIxDAIxW;
			double G_TWIxDAPxW;
			double G_ELxDAIxN;
			double G_ELxDAPxN;
			double G_SLxDAIxN;
			double G_SLxDAPxN;
			double G_CASLxDAIxN;
			double G_CASLxDAPxN;
			double G_TWIxDAIxN;
			double G_TWIxDAPxN;
			double G_ELxDAIxC;
			double G_ELxDAPxC;
			double G_SLxDAIxC;
			double G_SLxDAPxC;
			double G_CASLxDAIxC;
			double G_CASLxDAPxC;
			double G_TWIxDAIxC;
			double G_TWIxDAPxC;
			double G_BLSxELxDAIxT;
			double G_BLSxSLxDAIxT;
			double G_BLSxCASLxDAIxT;
			double G_BLSxTWIxDAIxT;
			double G_BLSxELxDAPxT;
			double G_BLSxSLxDAPxT;
			double G_BLSxCASLxDAPxT;
			double G_BLSxTWIxDAPxT;
			double G_BLSxELxDAIxE;
			double G_BLSxSLxDAIxE;
			double G_BLSxCASLxDAIxE;
			double G_BLSxTWIxDAIxE;
			double G_BLSxELxDAPxE;
			double G_BLSxSLxDAPxE;
			double G_BLSxCASLxDAPxE;
			double G_BLSxTWIxDAPxE;
			double G_BLSxELxDAIxW;
			double G_BLSxSLxDAIxW;
			double G_BLSxCASLxDAIxW;
			double G_BLSxTWIxDAIxW;
			double G_BLSxELxDAPxW;
			double G_BLSxSLxDAPxW;
			double G_BLSxCASLxDAPxW;
			double G_BLSxTWIxDAPxW;
			double G_BLSxELxDAIxN;
			double G_BLSxSLxDAIxN;
			double G_BLSxCASLxDAIxN;
			double G_BLSxTWIxDAIxN;
			double G_BLSxELxDAPxN;
			double G_BLSxSLxDAPxN;
			double G_BLSxCASLxDAPxN;
			double G_BLSxTWIxDAPxN;
			double G_BLSxELxDAIxC;
			double G_BLSxSLxDAIxC;
			double G_BLSxCASLxDAIxC;
			double G_BLSxTWIxDAIxC;
			double G_BLSxELxDAPxC;
			double G_BLSxSLxDAPxC;
			double G_BLSxCASLxDAPxC;
			double G_BLSxTWIxDAPxC;
			double G_LnB_mu;
			double G_B_mu;
			double G_AS_mu;
			double G_BLS_mu;
			double G_BS_mu;
			double G_EL1_mu;
			double G_EL2_mu;
			double G_SL1_mu;
			double G_SL2_mu;
			double G_CASL_mu;
			double G_TWI_mu;
			double G_DAI_mu;
			double G_DAP_mu;
			double G_Tc_mu;
			double G_T_mu;
			double G_E_mu;
			double G_W1_mu;
			double G_W2_mu;
			double G_W3_mu;
			double G_N_mu;
			double G_C_mu;
			double G_LnB_sig;
			double G_B_sig;
			double G_AS_sig;
			double G_BLS_sig;
			double G_BS_sig;
			double G_EL1_sig;
			double G_EL2_sig;
			double G_SL1_sig;
			double G_SL2_sig;
			double G_CASL_sig;
			double G_TWI_sig;
			double G_DAI_sig;
			double G_DAP_sig;
			double G_Tc_sig;
			double G_T_sig;
			double G_E_sig;
			double G_W1_sig;
			double G_W2_sig;
			double G_W3_sig;
			double G_N_sig;
			double G_C_sig;
			double G_LogCorrection;

			ParameterGrowthES3() {}

			ParameterGrowthES3(const EquationSet& values)
			{
				G_Int = values.at("G_Int");
				G_LnB = values.at("G_LnB");
				G_B = values.at("G_B");
				G_AS = values.at("G_AS");
				G_BLS = values.at("G_BLS");
				G_BS = values.at("G_BS");
				G_EL1 = values.at("G_EL1");
				G_EL2 = values.at("G_EL2");
				G_SL1 = values.at("G_SL1");
				G_SL2 = values.at("G_SL2");
				G_CASL = values.at("G_CASL");
				G_TWI = values.at("G_TWI");
				G_DAI = values.at("G_DAI");
				G_DAP = values.at("G_DAP");
				G_Tc = values.at("G_Tc");
				G_T = values.at("G_T");
				G_E = values.at("G_E");
				G_W1 = values.at("G_W1");
				G_W2 = values.at("G_W2");
				G_W3 = values.at("G_W3");
				G_N = values.at("G_N");
				G_C = values.at("G_C");
				G_BLSxEL = values.at("G_BLSxEL");
				G_BLSxSL = values.at("G_BLSxSL");
				G_BLSxCASL = values.at("G_BLSxCASL");
				G_BLSxTWI = values.at("G_BLSxTWI");
				G_BLSxDAI = values.at("G_BLSxDAI");
				G_BLSxDAP = values.at("G_BLSxDAP");
				G_BLSxT = values.at("G_BLSxT");
				G_BLSxE = values.at("G_BLSxE");
				G_BLSxW = values.at("G_BLSxW");
				G_BLSxN = values.at("G_BLSxN");
				G_BLSxC = values.at("G_BLSxC");
				G_ELxDAI = values.at("G_ELxDAI");
				G_ELxDAP = values.at("G_ELxDAP");
				G_SLxDAI = values.at("G_SLxDAI");
				G_SLxDAP = values.at("G_SLxDAP");
				G_CASLxDAI = values.at("G_CASLxDAI");
				G_CASLxDAP = values.at("G_CASLxDAP");
				G_TWIxDAI = values.at("G_TWIxDAI");
				G_TWIxDAP = values.at("G_TWIxDAP");
				G_ELxT = values.at("G_ELxT");
				G_ELxE = values.at("G_ELxE");
				G_ELxW = values.at("G_ELxW");
				G_ELxN = values.at("G_ELxN");
				G_ELxC = values.at("G_ELxC");
				G_SLxT = values.at("G_SLxT");
				G_SLxE = values.at("G_SLxE");
				G_SLxW = values.at("G_SLxW");
				G_SLxN = values.at("G_SLxN");
				G_SLxC = values.at("G_SLxC");
				G_CASLxT = values.at("G_CASLxT");
				G_CASLxE = values.at("G_CASLxE");
				G_CASLxW = values.at("G_CASLxW");
				G_CASLxN = values.at("G_CASLxN");
				G_CASLxC = values.at("G_CASLxC");
				G_TWIxT = values.at("G_TWIxT");
				G_TWIxE = values.at("G_TWIxE");
				G_TWIxW = values.at("G_TWIxW");
				G_TWIxN = values.at("G_TWIxN");
				G_TWIxC = values.at("G_TWIxC");
				G_DAIxT = values.at("G_DAIxT");
				G_DAIxE = values.at("G_DAIxE");
				G_DAIxW = values.at("G_DAIxW");
				G_DAIxN = values.at("G_DAIxN");
				G_DAIxC = values.at("G_DAIxC");
				G_DAPxT = values.at("G_DAPxT");
				G_DAPxE = values.at("G_DAPxE");
				G_DAPxW = values.at("G_DAPxW");
				G_DAPxN = values.at("G_DAPxN");
				G_DAPxC = values.at("G_DAPxC");
				G_TxE = values.at("G_TxE");
				G_TxW = values.at("G_TxW");
				G_NxT = values.at("G_NxT");
				G_NxT2 = values.at("G_NxT2");
				G_NxE = values.at("G_NxE");
				G_NxE2 = values.at("G_NxE2");
				G_NxW = values.at("G_NxW");
				G_NxW2 = values.at("G_NxW2");
				G_CxT = values.at("G_CxT");
				G_CxT2 = values.at("G_CxT2");
				G_CxE = values.at("G_CxE");
				G_CxE2 = values.at("G_CxE2");
				G_CxW = values.at("G_CxW");
				G_CxW2 = values.at("G_CxW2");
				G_CxN = values.at("G_CxN");
				G_CxN2 = values.at("G_CxN2");
				G_BLSxELxDAI = values.at("G_BLSxELxDAI");
				G_BLSxSLxDAI = values.at("G_BLSxSLxDAI");
				G_BLSxCASLxDAI = values.at("G_BLSxCASLxDAI");
				G_BLSxTWIxDAI = values.at("G_BLSxTWIxDAI");
				G_BLSxELxDAP = values.at("G_BLSxELxDAP");
				G_BLSxSLxDAP = values.at("G_BLSxSLxDAP");
				G_BLSxCASLxDAP = values.at("G_BLSxCASLxDAP");
				G_BLSxTWIxDAP = values.at("G_BLSxTWIxDAP");
				G_BLSxELxT = values.at("G_BLSxELxT");
				G_BLSxSLxT = values.at("G_BLSxSLxT");
				G_BLSxCASLxT = values.at("G_BLSxCASLxT");
				G_BLSxTWIxT = values.at("G_BLSxTWIxT");
				G_BLSxELxE = values.at("G_BLSxELxE");
				G_BLSxSLxE = values.at("G_BLSxSLxE");
				G_BLSxCASLxE = values.at("G_BLSxCASLxE");
				G_BLSxTWIxE = values.at("G_BLSxTWIxE");
				G_BLSxELxW = values.at("G_BLSxELxW");
				G_BLSxSLxW = values.at("G_BLSxSLxW");
				G_BLSxCASLxW = values.at("G_BLSxCASLxW");
				G_BLSxTWIxW = values.at("G_BLSxTWIxW");
				G_BLSxELxN = values.at("G_BLSxELxN");
				G_BLSxSLxN = values.at("G_BLSxSLxN");
				G_BLSxCASLxN = values.at("G_BLSxCASLxN");
				G_BLSxTWIxN = values.at("G_BLSxTWIxN");
				G_BLSxELxC = values.at("G_BLSxELxC");
				G_BLSxSLxC = values.at("G_BLSxSLxC");
				G_BLSxCASLxC = values.at("G_BLSxCASLxC");
				G_BLSxTWIxC = values.at("G_BLSxTWIxC");
				G_ELxDAIxT = values.at("G_ELxDAIxT");
				G_ELxDAPxT = values.at("G_ELxDAPxT");
				G_SLxDAIxT = values.at("G_SLxDAIxT");
				G_SLxDAPxT = values.at("G_SLxDAPxT");
				G_CASLxDAIxT = values.at("G_CASLxDAIxT");
				G_CASLxDAPxT = values.at("G_CASLxDAPxT");
				G_TWIxDAIxT = values.at("G_TWIxDAIxT");
				G_TWIxDAPxT = values.at("G_TWIxDAPxT");
				G_ELxDAIxE = values.at("G_ELxDAIxE");
				G_ELxDAPxE = values.at("G_ELxDAPxE");
				G_SLxDAIxE = values.at("G_SLxDAIxE");
				G_SLxDAPxE = values.at("G_SLxDAPxE");
				G_CASLxDAIxE = values.at("G_CASLxDAIxE");
				G_CASLxDAPxE = values.at("G_CASLxDAPxE");
				G_TWIxDAIxE = values.at("G_TWIxDAIxE");
				G_TWIxDAPxE = values.at("G_TWIxDAPxE");
				G_ELxDAIxW = values.at("G_ELxDAIxW");
				G_ELxDAPxW = values.at("G_ELxDAPxW");
				G_SLxDAIxW = values.at("G_SLxDAIxW");
				G_SLxDAPxW = values.at("G_SLxDAPxW");
				G_CASLxDAIxW = values.at("G_CASLxDAIxW");
				G_CASLxDAPxW = values.at("G_CASLxDAPxW");
				G_TWIxDAIxW = values.at("G_TWIxDAIxW");
				G_TWIxDAPxW = values.at("G_TWIxDAPxW");
				G_ELxDAIxN = values.at("G_ELxDAIxN");
				G_ELxDAPxN = values.at("G_ELxDAPxN");
				G_SLxDAIxN = values.at("G_SLxDAIxN");
				G_SLxDAPxN = values.at("G_SLxDAPxN");
				G_CASLxDAIxN = values.at("G_CASLxDAIxN");
				G_CASLxDAPxN = values.at("G_CASLxDAPxN");
				G_TWIxDAIxN = values.at("G_TWIxDAIxN");
				G_TWIxDAPxN = values.at("G_TWIxDAPxN");
				G_ELxDAIxC = values.at("G_ELxDAIxC");
				G_ELxDAPxC = values.at("G_ELxDAPxC");
				G_SLxDAIxC = values.at("G_SLxDAIxC");
				G_SLxDAPxC = values.at("G_SLxDAPxC");
				G_CASLxDAIxC = values.at("G_CASLxDAIxC");
				G_CASLxDAPxC = values.at("G_CASLxDAPxC");
				G_TWIxDAIxC = values.at("G_TWIxDAIxC");
				G_TWIxDAPxC = values.at("G_TWIxDAPxC");
				G_BLSxELxDAIxT = values.at("G_BLSxELxDAIxT");
				G_BLSxSLxDAIxT = values.at("G_BLSxSLxDAIxT");
				G_BLSxCASLxDAIxT = values.at("G_BLSxCASLxDAIxT");
				G_BLSxTWIxDAIxT = values.at("G_BLSxTWIxDAIxT");
				G_BLSxELxDAPxT = values.at("G_BLSxELxDAPxT");
				G_BLSxSLxDAPxT = values.at("G_BLSxSLxDAPxT");
				G_BLSxCASLxDAPxT = values.at("G_BLSxCASLxDAPxT");
				G_BLSxTWIxDAPxT = values.at("G_BLSxTWIxDAPxT");
				G_BLSxELxDAIxE = values.at("G_BLSxELxDAIxE");
				G_BLSxSLxDAIxE = values.at("G_BLSxSLxDAIxE");
				G_BLSxCASLxDAIxE = values.at("G_BLSxCASLxDAIxE");
				G_BLSxTWIxDAIxE = values.at("G_BLSxTWIxDAIxE");
				G_BLSxELxDAPxE = values.at("G_BLSxELxDAPxE");
				G_BLSxSLxDAPxE = values.at("G_BLSxSLxDAPxE");
				G_BLSxCASLxDAPxE = values.at("G_BLSxCASLxDAPxE");
				G_BLSxTWIxDAPxE = values.at("G_BLSxTWIxDAPxE");
				G_BLSxELxDAIxW = values.at("G_BLSxELxDAIxW");
				G_BLSxSLxDAIxW = values.at("G_BLSxSLxDAIxW");
				G_BLSxCASLxDAIxW = values.at("G_BLSxCASLxDAIxW");
				G_BLSxTWIxDAIxW = values.at("G_BLSxTWIxDAIxW");
				G_BLSxELxDAPxW = values.at("G_BLSxELxDAPxW");
				G_BLSxSLxDAPxW = values.at("G_BLSxSLxDAPxW");
				G_BLSxCASLxDAPxW = values.at("G_BLSxCASLxDAPxW");
				G_BLSxTWIxDAPxW = values.at("G_BLSxTWIxDAPxW");
				G_BLSxELxDAIxN = values.at("G_BLSxELxDAIxN");
				G_BLSxSLxDAIxN = values.at("G_BLSxSLxDAIxN");
				G_BLSxCASLxDAIxN = values.at("G_BLSxCASLxDAIxN");
				G_BLSxTWIxDAIxN = values.at("G_BLSxTWIxDAIxN");
				G_BLSxELxDAPxN = values.at("G_BLSxELxDAPxN");
				G_BLSxSLxDAPxN = values.at("G_BLSxSLxDAPxN");
				G_BLSxCASLxDAPxN = values.at("G_BLSxCASLxDAPxN");
				G_BLSxTWIxDAPxN = values.at("G_BLSxTWIxDAPxN");
				G_BLSxELxDAIxC = values.at("G_BLSxELxDAIxC");
				G_BLSxSLxDAIxC = values.at("G_BLSxSLxDAIxC");
				G_BLSxCASLxDAIxC = values.at("G_BLSxCASLxDAIxC");
				G_BLSxTWIxDAIxC = values.at("G_BLSxTWIxDAIxC");
				G_BLSxELxDAPxC = values.at("G_BLSxELxDAPxC");
				G_BLSxSLxDAPxC = values.at("G_BLSxSLxDAPxC");
				G_BLSxCASLxDAPxC = values.at("G_BLSxCASLxDAPxC");
				G_BLSxTWIxDAPxC = values.at("G_BLSxTWIxDAPxC");
				G_LnB_mu = values.at("G_LnB_mu");
				G_B_mu = values.at("G_B_mu");
				G_AS_mu = values.at("G_AS_mu");
				G_BLS_mu = values.at("G_BLS_mu");
				G_BS_mu = values.at("G_BS_mu");
				G_EL1_mu = values.at("G_EL1_mu");
				G_EL2_mu = values.at("G_EL2_mu");
				G_SL1_mu = values.at("G_SL1_mu");
				G_SL2_mu = values.at("G_SL2_mu");
				G_CASL_mu = values.at("G_CASL_mu");
				G_TWI_mu = values.at("G_TWI_mu");
				G_DAI_mu = values.at("G_DAI_mu");
				G_DAP_mu = values.at("G_DAP_mu");
				G_Tc_mu = values.at("G_Tc_mu");
				G_T_mu = values.at("G_T_mu");
				G_E_mu = values.at("G_E_mu");
				G_W1_mu = values.at("G_W1_mu");
				G_W2_mu = values.at("G_W2_mu");
				G_W3_mu = values.at("G_W3_mu");
				G_N_mu = values.at("G_N_mu");
				G_C_mu = values.at("G_C_mu");
				G_LnB_sig = values.at("G_LnB_sig");
				G_B_sig = values.at("G_B_sig");
				G_AS_sig = values.at("G_AS_sig");
				G_BLS_sig = values.at("G_BLS_sig");
				G_BS_sig = values.at("G_BS_sig");
				G_EL1_sig = values.at("G_EL1_sig");
				G_EL2_sig = values.at("G_EL2_sig");
				G_SL1_sig = values.at("G_SL1_sig");
				G_SL2_sig = values.at("G_SL2_sig");
				G_CASL_sig = values.at("G_CASL_sig");
				G_TWI_sig = values.at("G_TWI_sig");
				G_DAI_sig = values.at("G_DAI_sig");
				G_DAP_sig = values.at("G_DAP_sig");
				G_Tc_sig = values.at("G_Tc_sig");
				G_T_sig = values.at("G_T_sig");
				G_E_sig = values.at("G_E_sig");
				G_W1_sig = values.at("G_W1_sig");
				G_W2_sig = values.at("G_W2_sig");
				G_W3_sig = values.at("G_W3_sig");
				G_N_sig = values.at("G_N_sig");
				G_C_sig = values.at("G_C_sig");
				G_LogCorrection = values.at("G_LogCorrection");
			}
		};
	}
}

#endif
