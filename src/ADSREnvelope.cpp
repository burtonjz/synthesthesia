#include "ADSREnvelope.hpp"
#include "KeyboardController.hpp"

#include <iostream>

ParameterController ADSREnvelope::params_ ;
double* ADSREnvelope::sample_rate_ = nullptr ;

void ADSREnvelope::activate(double* sample_rate){
    sample_rate_ = sample_rate ;

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

double ADSREnvelope::getSustain(const double value, const double sustain){
    return sustain * value ;
}

double ADSREnvelope::getRelease(const double start_level, const double time_release, const double release){
    if (time_release > release) return 0.0 ;
    return start_level - start_level * time_release  / release ;
}

double ADSREnvelope::modulate(double value, boost::container::flat_map<ModulationParameter,double>* modp){
    // REQUIRED PARAMS
    if ( modp->find(ModulationParameter::MIDI_NOTE) == modp->end() ) return value ;
    if ( modp->find(ModulationParameter::INITIAL_VALUE) == modp->end() ) return value ;
    if ( modp->find(ModulationParameter::LAST_VALUE) == modp->end() ) return value ;

    uint8_t midi_note = static_cast<uint8_t>((*modp)[ModulationParameter::MIDI_NOTE]);
    const boost::container::flat_map<uint8_t, Note>* notes = KeyboardController::get_active_notes() ;

    auto it = notes->find(midi_note);
    if ( it == notes->end() ) return value ;

    double a = params_.getParameterInstantaneousValue<double>(ParameterType::ATTACK);
    double d = params_.getParameterInstantaneousValue<double>(ParameterType::DECAY);
    double s = params_.getParameterInstantaneousValue<double>(ParameterType::SUSTAIN);
    double r = params_.getParameterInstantaneousValue<double>(ParameterType::RELEASE);
    double rt = it->second.getTimeSinceReleased();
    double pt = it->second.getTimeSincePressed();
    double output ;

    if ( it->second.getIsPressed() ){    
        if (pt == 0.0 ){ // if press stage just started, calculate initial_value
            // (*modp)[ModulationParameter::INITIAL_VALUE] = getRelease(value,(*modp)[ModulationParameter::INITIAL_VALUE],rt - 1.0 / *sample_rate_ ,r);
            (*modp)[ModulationParameter::INITIAL_VALUE] = (*modp)[ModulationParameter::LAST_VALUE] ;
        }

        if ( pt < a ) output = getAttack(value,(*modp)[ModulationParameter::INITIAL_VALUE],pt,a);
        else if ( pt < a + d ) output = getDecay(value, s, pt - a, d);
        else output = getSustain(value, s) ;

    } else {
        if (rt == 0.0){ // if release stage just started, calculate initial_value
            // if (pt < a ) (*modp)[ModulationParameter::INITIAL_VALUE] = getAttack(1.0,0.0, pt - 1.0 / *sample_rate_ ,a) ;
            // else if (pt < a + d ) (*modp)[ModulationParameter::INITIAL_VALUE] = getDecay(1.0, s, pt - a - 1.0 / *sample_rate_, d) ;
            // else (*modp)[ModulationParameter::INITIAL_VALUE] = s ;
            (*modp)[ModulationParameter::INITIAL_VALUE] = (*modp)[ModulationParameter::LAST_VALUE] ;
        }

        output = getRelease((*modp)[ModulationParameter::INITIAL_VALUE], rt, r);
    }    

    (*modp)[ModulationParameter::LAST_VALUE] = output ;
    return output ;

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