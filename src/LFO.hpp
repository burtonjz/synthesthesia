#ifndef __LFO_HPP_
#define __LFO_HPP_

#include "commonTypes.hpp"
#include "config.hpp"

#include "Modulator.hpp"
#include "Module.hpp"

#include "ParameterController.hpp"
#include "Oscillator.hpp"

class LFO : public Modulator, public Module {
private:
    static std::array<ParameterType,4> control_params_ ;
    static std::array<ParameterType,2> modulatable_params_ ;
    Oscillator oscillator_ ;
    
public:
    /**
     * @brief returns Parameter controlPorts
    */
    static std::pair<const ParameterType*, size_t> getControlPorts();

    /**
     * @brief returns modulatable parameters
    */
    static std::pair<const ParameterType*, size_t> getModulatableParameters();

    /**
     * @brief LFO constructor
     * 
    */
    LFO();

    /**
     * @brief activate the LFO
     * 
     * @param sampleRate synth sample rate
    */
    void activate(const double* sampleRate);

    /**
     * @brief tick the LFO
    */
    void tick();
    
    /**
     * @brief modulation function
     * 
     * @param value input value
     * @param modp modulation parameter map. No parameters are required for this modulation.
    */
    double modulate(double value, ParameterModMap* modp ) const override ;
};

#endif // __LFO_HPP_