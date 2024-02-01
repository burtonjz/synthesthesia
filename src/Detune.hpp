#ifndef __DETUNE_HPP_
#define __DETUNE_HPP_

#include "config.hpp"
#include "Parameter.hpp"
#include "ModulationParameter.hpp"

#include <array>

/**
 * @brief static class to handle detune scaling factors/modulation
*/
class Detune {
private:
    static std::array<double,CONFIG_DETUNE_MAX_CENTS_2 + 1> scaleFactor ;

public:
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
    static double modulate(double value, ParameterModMap* modp);

};

#endif // __DETUNE_HPP_