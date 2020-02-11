#include "moja/modules/cbm/standbiomasscarboncurve.h"

#include <moja/logging.h>
#include <algorithm>

namespace moja {
namespace modules {
namespace cbm {
    
    std::unordered_map<std::string, double> StandBiomassCarbonCurve::getIncrements() {
        double standRootBiomass = 0.0;
        for (const auto& component : _components) {
            standRootBiomass += component.calculateRootBiomass();
        }

        std::unordered_map<std::string, double> increments;
        for (const auto& component : _components) {
            const auto componentIncrements = component.getIncrements(standRootBiomass);
            increments.insert(componentIncrements.begin(), componentIncrements.end());
        }

        return increments;
    }

    std::vector<double> StandBiomassCarbonCurve::getAboveGroundCarbonCurve()
    {
        std::vector<double> curve;
        for (const auto& component : _components) {
            const auto& componentCurve = component.getAboveGroundCarbonCurve();
            auto maxComponentAge = componentCurve.size();

            if (curve.size() < maxComponentAge) {
                curve.resize(maxComponentAge, 0.0);
            }

            for (int i = 0; i < maxComponentAge; i++) {
                curve[i] += componentCurve[i];
            }
        }

        return curve;
    }

    std::vector<double> StandBiomassCarbonCurve::getFoliageCarbonCurve()
    {
        std::vector<double> curve;
        for (const auto& component : _components) {
            const auto& componentCurve = component.getFoliageCarbonCurve();
            auto maxComponentAge = componentCurve.size();

            if (curve.size() < maxComponentAge) {
                curve.resize(maxComponentAge, 0.0);
            }

            for (int i = 0; i < maxComponentAge; i++) {
                curve[i] += componentCurve[i];
            }
        }

        return curve;
    }

}}}