#include "ADSREnvelope.hpp"
#include "KeyboardController.hpp"

#ifdef DEBUG
#include <iostream>
#endif 

ParameterController ADSREnvelope::params_ ;


void ADSREnvelope::activate(){
    params_.addParameter<double>(
        ParameterType::ATTACK,
        parameterDefaults[static_cast<int>(ParameterType::ATTACK)],
        true
    );
    params_.addParameter<double>(
        ParameterType::DECAY,
        parameterDefaults[static_cast<int>(ParameterType::DECAY)],
        true
    );
    params_.addParameter<double>(
        ParameterType::SUSTAIN,
        parameterDefaults[static_cast<int>(ParameterType::SUSTAIN)],
        true
    );
    params_.addParameter<double>(
        ParameterType::RELEASE,
        parameterDefaults[static_cast<int>(ParameterType::RELEASE)],
        true
    );
}

double ADSREnvelope::getAttack(const double value, const double start_level, const double time_pressed, const double attack){
    return start_level + ( value - start_level ) * time_pressed / attack ;
}

double ADSREnvelope::getDecay(const double value, const double sustain, const double time_decay, const double decay){
    return value - (value - sustain * value) * ( time_decay / decay ) ;
}

double ADSREnvelope::getRelease(const double value, const double sustain, const double time_release, const double release){
    return sustain * value - ( sustain * value * time_release ) / release ;
}

double ADSREnvelope::modulate(double value, boost::container::flat_map<ModulationParameter,double> modp){
    // REQUIRED PARAMS
    if ( modp.find(ModulationParameter::MIDI_NOTE) == modp.end() ) return value ;
    if ( modp.find(ModulationParameter::LAST_SAMPLE) == modp.end() ) return value ;

    uint8_t midi_note = static_cast<uint8_t>(modp[ModulationParameter::MIDI_NOTE]);
    const boost::container::flat_map<uint8_t, Note>* notes = KeyboardController::get_active_notes() ;

    auto it = notes->find(midi_note);
    if ( it == notes->end() ) return value ;

    double a = params_.getParameterInstantaneousValue<double>(ParameterType::ATTACK);
    double d = params_.getParameterInstantaneousValue<double>(ParameterType::DECAY);
    double s = params_.getParameterInstantaneousValue<double>(ParameterType::SUSTAIN);
    double r = params_.getParameterInstantaneousValue<double>(ParameterType::RELEASE);
    double rt = it->second.getTimeSinceReleased();
    double pt = it->second.getTimeSincePressed();

    if ( it->second.getIsPressed() ){    
        if ( pt < a ){
            // if ( rt > 0 && rt < r) return getAttack(value, getRelease(1.0, s, rt, r), pt, a);
            return getAttack(value,0.0,pt,a);
        }

        if ( pt < a + d ){
            return getDecay(value, s, pt - a, d);
        }

        return value * s ;

    } else {
        if (rt > r) return 0.0 ;
        if ( pt < a ) return getRelease(value, getAttack(1.0,0.0,pt,a), rt, r);
        if ( pt < a + d ) return getRelease(value, getDecay(1.0, s, pt - a, d), rt, r);

        return getRelease(value, s, rt, r);
    }    
}

double ADSREnvelope::getValue(ParameterType param){
    return params_.getParameterValue<double>(param);
}

void ADSREnvelope::setParameter(ParameterType param, double value){
    params_.setParameterValue<double>(param,value);
}

void ADSREnvelope::tick(){
    params_.modulateParameter<double>(ParameterType::ATTACK);
    params_.modulateParameter<double>(ParameterType::DECAY);
    params_.modulateParameter<double>(ParameterType::SUSTAIN);
    params_.modulateParameter<double>(ParameterType::RELEASE);
}