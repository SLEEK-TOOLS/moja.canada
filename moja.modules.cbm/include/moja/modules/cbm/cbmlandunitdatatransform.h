#ifndef MOJA_MODULES_CBM_CBMLANDUNITDATATRANSFORM_H_
#define MOJA_MODULES_CBM_CBMLANDUNITDATATRANSFORM_H_

#include "moja/flint/itransform.h"
#include "moja/datarepository/iproviderrelationalinterface.h"
#include "moja/flint/ilandunitcontroller.h"

namespace moja {
namespace modules {
namespace cbm {

class CBMLandUnitDataTransform : public flint::ITransform {
public:
	void configure(DynamicObject config,
		const flint::ILandUnitController& landUnitController,
		datarepository::DataRepository& dataRepository) override;

	void controllerChanged(const flint::ILandUnitController& controller) override;
	const Dynamic& value() const override;

private:
	const flint::ILandUnitController* _landUnitController;
	datarepository::DataRepository* _dataRepository;
	datarepository::IProviderRelationalInterface::Ptr _provider;
	const flint::IVariable* _varToUse;
	std::string _varName;

	mutable Dynamic _results;
	mutable DynamicObject _resultsObject;
};

}
}
}
#endif // MOJA_MODULES_CBM_CBMLANDUNITDATATRANSFORM_H_
