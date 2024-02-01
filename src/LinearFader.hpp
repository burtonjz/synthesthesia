#ifndef __LINEAR_FADER_HPP_
#define __LINEAR_FADER_HPP_

#include "ModulationParameter.hpp"
#include "KeyboardController.hpp"
#include "Note.hpp"

#include <boost/container/flat_map.hpp>

#ifdef DEBUG
#include <iostream>
#endif 

class LinearFader {
public:
    static double modulate(double value, boost::container::flat_map<ModulationParameter,double> params){
        // REQUIRED PARAMS
        if ( params.find(ModulationParameter::MIDI_NOTE) == params.end() ) return value ;

        uint8_t midi_note = static_cast<uint8_t>(params[ModulationParameter::MIDI_NOTE]);
        const KeyboardMap* notes = KeyboardController::get_active_notes() ;

        auto it = notes->find(midi_note);
        if ( it == notes->end() ) return value ;

        if ( it->second.getIsPressed() ){
            double t = it->second.getTimeSincePressed();
            if ( t > CONFIG_LINEAR_FADER_FADE_TIME ) return value ;
            return ( value / CONFIG_LINEAR_FADER_FADE_TIME ) * t ;
        } else {
            double t = it->second.getTimeSinceReleased();
            if ( t > CONFIG_LINEAR_FADER_FADE_TIME ) return 0.0 ;
            return value - ( value / CONFIG_LINEAR_FADER_FADE_TIME ) * t ; 
        }      
    }
};

#endif // __LINEAR_FADER_HPP_