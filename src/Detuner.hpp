#ifndef __DETUNER_HPP_
#define __DETUNER_HPP_

#include "config.hpp"
#include "commonTypes.hpp"

#include "Modulator.hpp"
#include "Parameter.hpp"
#include "ModulationParameter.hpp"
#include "KeyboardController.hpp"

#include <array>

/**
 * @brief static class to handle detune scaling factors/modulation
*/
class Detuner : public Modulator {
private:
    static std::array<double,CONFIG_DETUNE_MAX_CENTS_2 + 1> scaleFactor ;
    KeyboardController* keyboardController_ ;

public:
    /**
     * @brief constructor necessary when using for modulation
     * 
     * @param keyboardController used to add in pitchbend logic
     * 
    */
    Detuner(KeyboardController* keyboardController);

    Detuner();

    /**
     * @brief activate the module instance by connecting a keyboard controller
     * @param keyboardController pointer to keyboard controller
    */
    void activate(KeyboardController* keyboardController);

    /**
     * @brief returns detune factor for given percent change
    */
    static double getDetuneFactor(int cents);

    /**
     * @brief generate detune scale factor array prior to real time processing
    */
    static void generate();

    /**
     * @brief modulate detune. Also adds in pitchbend from keyboard controller.
     * 
     * @param value value to modulate (the Parameter value)
     * @param modp ModulationParameter map. Must contain DETUNE_CENTS
    */
    double modulate(double value, ParameterModMap* modp) const override;

};

#endif // __DETUNER_HPP_