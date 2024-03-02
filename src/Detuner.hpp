#ifndef __DETUNER_HPP_
#define __DETUNER_HPP_

#include "config.hpp"
#include "commonTypes.hpp"

#include "Modulator.hpp"
#include "Parameter.hpp"
#include "ModulationParameter.hpp"

#include <array>

/**
 * @brief static class to handle detune scaling factors/modulation
 * 
 * TODO: remove the detune parameter from the oscillator and give it to this class.
*/
class Detuner : public Modulator { // TODO: make this a module as well. I think it should have a parameterController with the detune param.
private:
    static std::array<double,CONFIG_DETUNE_MAX_CENTS_2 + 1> scaleFactor ;

public:
    /**
     * @brief constructor necessary when using for modulation
     * 
     * @param keyboardController used to add in pitchbend logic
     * 
    */
    Detuner();

    /**
     * @brief returns detune factor for given percent change
    */
    static double getDetuneFactor(int cents);

    /**
     * @brief generate detune scale factor array prior to real time processing
    */
    static void generate();

    /**
     * @brief Detune modulation. Should be added to frequency parameters.
     * 
     * @param value value to modulate (the Parameter value)
     * @param modp ModulationParameter map. Must contain DETUNE_CENTS
    */
    double modulate(double value, ParameterModMap* modp) const override;

};

#endif // __DETUNER_HPP_