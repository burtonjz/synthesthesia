#ifndef __LFO_HPP_
#define __LFO_HPP_

#include "commonTypes.hpp"
#include "ParameterController.hpp"
#include "Modulator.hpp"
#include "Oscillator.hpp"

class LFO : public Modulator {
private:
    static std::array<ParameterType,4> control_params_ ;

    ParameterController parameterController_ ;
    Oscillator oscillator_ ;
    
public:
    /**
     * @brief returns Parameter controlPorts
    */
    static std::pair<const ParameterType*, size_t> getControlPorts();

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
     * @brief returns a pointer to the ParameterController
    */
    ParameterController* getParameterController();

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