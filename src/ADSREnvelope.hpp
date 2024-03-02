#ifndef __ADSR_ENVELOPE_HPP_
#define __ADSR_ENVELOPE_HPP_

#include "config.hpp"
#include "commonTypes.hpp"

#include "Module.hpp"
#include "Modulator.hpp"
#include "ParameterController.hpp"
#include "KeyboardController.hpp"
#include "ParameterType.hpp"
#include "ModulationParameter.hpp"

#include <array>
#include <utility>
#include <cstdint>

class ADSREnvelope : public Modulator, public Module {
private:
    KeyboardController* keyboardController_ ;
    static std::array<ParameterType,4> control_params_ ;

public:
    /**
     * @brief returns Parameter controlPorts
    */
    static std::pair<const ParameterType*, size_t> getControlPorts();

    /**
     * @brief default constructor
     * 
    */
    ADSREnvelope();

    /**
     * @brief activate modulator.
    */
    void activate(const double* sample_rate, KeyboardController* keyboardController);

    /**
     * @brief modulate function
     * 
     * This function is given to a Parameter to modulate its value and store the result in the
     * parameter's instantaneousValue.
     * 
     * @param value value to modulate (the Parameter value)
     * @param modp ModulationParameter map. Must contain MIDI_NOTE.
    */
    double modulate(double value, ParameterModMap* modp) const override;

    /**
     * @brief returns a pointer to the ParameterController
    */
    ParameterController* getParameterController();

    /**
     * @brief tick envelope to next sample
     * 
     * Ticks envelope to next sample, which will modulate the ADSR values if modulation is set
    */
    void tick();

    /**
     * @brief returns the currently set release time
    */
    double getReleaseTime();

private:
    /**
     * @brief get modulated value for attack stage
     * 
     * @param value input value
     * @param start_value initial value when attack stage triggered
     * @param time_pressed time in seconds since key press
     * @param attack the time duration in seconds for the attack stage
    */
    double getAttack(const double value, const double start_level, const double time_pressed, const double attack) const ;

    /**
     * @brief get modulated value for decay stage
     * 
     * @param value input value
     * @param sustain sustain level (decay stage will end here)
     * @param time_decay the time since decay stage triggered
     * @param decay the time duration in seconds for the decay stage
    */
    double getDecay(const double value, const double sustain, const double time_decay, const double decay) const ;

    /**
     * @brief get modulated value for decay stage
     * 
     * @param value input value
     * @param sustain sustain level
    */
    double getSustain(const double value, const double sustain) const ;

    /**
     * @brief get modulated value for decay stage
     * 
     * @param start_level
     * @param time_release the time since release stage triggered
     * @param release the time duration in seconds for the release stage
    */
    double getRelease(const double start_level, const double time_release, const double release) const ; 
};

#endif // __ADSR_ENVELOPE_HPP_