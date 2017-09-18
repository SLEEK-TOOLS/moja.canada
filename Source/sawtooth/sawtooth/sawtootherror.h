#ifndef sawtooth_error_codes_h
#define sawtooth_error_codes_h

namespace Sawtooth {

	enum Sawtooth_Error_Code {
		Sawtooth_NoError = 0,
		Sawtooth_UnknownError = 1,
		Sawtooth_DBOpenError = 2,
		Sawtooth_DBQueryError = 3,
		Sawtooth_ParameterNameError = 4,
		Sawtooth_ModelMetaError = 5,
		Sawtooth_StandStateError = 6,
		Sawtooth_StandArgumentError = 7
	};

	const size_t maxErrLen = 1000;
	struct Sawtooth_Error {
		char Message[maxErrLen];
		Sawtooth_Error_Code Code;
	};

	//void SetError(std::string errorMsg, )
}

#endif