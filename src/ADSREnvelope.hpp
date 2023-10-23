#ifndef __ADSR_ENVELOPE_HPP_
#define __ADSR_ENVELOPE_HPP_

#include "config.hpp"
#include "ParameterController.hpp"
#include "ParameterType.hpp"
#include "ModulationParameter.hpp"

#include <boost/container/flat_map.hpp>

class ADSREnvelope {
private:
    static ParameterController params_ ;


    /**
     * @brief get modulated value for attack stage
     * 
     * @param value input value
     * @param start_value initial value when attack stage triggered
     * @param time_pressed time in seconds since key press
     * @param attack the time duration in seconds for the attack stage
    */
    static double getAttack(const double value, const double start_level, const double time_pressed, const double attack);

    /**
     * @brief get modulated value for decay stage
     * 
     * @param value input value
     * @param sustain sustain level (decay stage will end here)
     * @param time_decay the time since decay stage triggered
     * @param decay the time duration in seconds for the decay stage
    */
    static double getDecay(const double value, const double sustain, const double time_decay, const double decay);

    /**
     * @brief get modulated value for decay stage
     * 
     * @param value input value
     * @param sustain sustain level (release stage will start here)
     * @param time_release the time since release stage triggered
     * @param release the time duration in seconds for the release stage
    */
    static double getRelease(const double value, const double sustain, const double time_release, const double release); 

public:
    /**
     * @brief activates the ADSR envelope
     * 
    */
    static void activate();

    /**
     * @brief modulate function
     * 
     * This function is given to a Parameter to modulate its value and store the result in the
     * parameter's instantaneousValue.
     * 
     * @param value value to modulate (the Parameter value)
     * @param modp ModulationParameter map. Must contain MIDI_NOTE.
    */
    static double modulate(double value, boost::container::flat_map<ModulationParameter,double> modp);

    /**
     * @brief returns the value for an envelope parameter
     * 
     * @param param must be one of { ATTACK, DECAY, SUSTAIN, RELEASE }
    */
    static double getValue(ParameterType param);

    /**
     * @brief sets the value for an envelope parameter
     * 
     * @param param must be one of { ATTACK, DECAY, SUSTAIN, RELEASE }
     * @param value value to set. Note -- will be clamped within ParameterLimit
    */
    static void setParameter(ParameterType param, double value);

    /**
     * @brief tick envelope to next sample
     * 
     * Ticks envelope to next sample, which will modulate the ADSR values if modulation is set
    */
    static void tick();
};

#endif // __ADSR_ENVELOPE_HPP_