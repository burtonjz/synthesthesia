#include "ADSREnvelope.hpp"
#include "ModulatorType.hpp"
#include "ModuleType.hpp"
#include "Modulator.hpp"
#include "KeyboardController.hpp"

#include <iostream>

std::array<ParameterType,4> ADSREnvelope::control_params_ = {
    ParameterType::ATTACK,
    ParameterType::DECAY,
    ParameterType::SUSTAIN,
    ParameterType::RELEASE
};

std::array<ParameterType,4> ADSREnvelope::modulatable_params_ = {
    ParameterType::ATTACK,
    ParameterType::DECAY,
    ParameterType::SUSTAIN,
    ParameterType::RELEASE
};

ADSREnvelope::ADSREnvelope():
    Modulator(ModulatorType::ADSREnvelope),
    Module(ModuleType::ADSREnvelope)
{}

std::pair<const ParameterType*, size_t> ADSREnvelope::getControlPorts(){
    return { control_params_.data(), control_params_.size() } ;
}

std::pair<const ParameterType*, size_t> ADSREnvelope::getModulatableParameters(){
    return { modulatable_params_.data(), modulatable_params_.size() } ;
}

void ADSREnvelope::activate(const double* sample_rate, KeyboardController* keyboardController){
    setSampleRate(sample_rate);
    keyboardController_ = keyboardController ;

    parameterController_.addParameter<ParameterType::ATTACK>(
        parameterDefaults[static_cast<int>(ParameterType::ATTACK)],
        true
    );
    parameterController_.addParameter<ParameterType::DECAY>(
        parameterDefaults[static_cast<int>(ParameterType::DECAY)],
        true
    );
    parameterController_.addParameter<ParameterType::SUSTAIN>(
        parameterDefaults[static_cast<int>(ParameterType::SUSTAIN)],
        true
    );
    parameterController_.addParameter<ParameterType::RELEASE>(
        parameterDefaults[static_cast<int>(ParameterType::RELEASE)],
        true
    );

}

double ADSREnvelope::getAttack(const double value, const double start_level, const double time_pressed, const double attack) const {
    return start_level + ( value - start_level ) * time_pressed / attack ;
}

double ADSREnvelope::getDecay(const double value, const double sustain, const double time_decay, const double decay) const {
    return value - (value - sustain * value) * ( time_decay / decay ) ;
}

double ADSREnvelope::getSustain(const double value, const double sustain) const {
    return sustain * value ;
}

double ADSREnvelope::getRelease(const double start_level, const double time_release, const double release) const {
    if (time_release > release) return 0.0 ;
    return start_level - start_level * time_release  / release ;
}

double ADSREnvelope::modulate(double value, ParameterModMap* modp) const {
    // REQUIRED PARAMS
    if ( modp->find(ModulationParameter::MIDI_NOTE) == modp->end() ) return value ;
    if ( modp->find(ModulationParameter::INITIAL_VALUE) == modp->end() ) return value ;
    if ( modp->find(ModulationParameter::LAST_VALUE) == modp->end() ) return value ;

    uint8_t midi_note = static_cast<uint8_t>((*modp)[ModulationParameter::MIDI_NOTE]);
    const KeyboardMap* notes = keyboardController_->get_active_notes() ;

    auto it = notes->find(midi_note);
    if ( it == notes->end() ) return value ;

    TYPE_TRAIT(ParameterType::ATTACK) a = parameterController_.getParameterInstantaneousValue<ParameterType::ATTACK>();
    TYPE_TRAIT(ParameterType::DECAY) d = parameterController_.getParameterInstantaneousValue<ParameterType::DECAY>();
    TYPE_TRAIT(ParameterType::SUSTAIN) s = parameterController_.getParameterInstantaneousValue<ParameterType::SUSTAIN>();
    TYPE_TRAIT(ParameterType::RELEASE) r = parameterController_.getParameterInstantaneousValue<ParameterType::RELEASE>();
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
            (*modp)[ModulationParameter::INITIAL_VALUE] = (*modp)[ModulationParameter::LAST_VALUE] ;
        }

        output = getRelease((*modp)[ModulationParameter::INITIAL_VALUE], rt, r);
    }    

    (*modp)[ModulationParameter::LAST_VALUE] = output ;
    return output ;

}

ParameterController* ADSREnvelope::getParameterController(){
    return &parameterController_ ;
}

void ADSREnvelope::tick(){
    parameterController_.modulate();
}

double ADSREnvelope::getReleaseTime(){
    if(parameterController_.parameterExists(ParameterType::RELEASE)){
        return parameterController_.getParameterInstantaneousValue<ParameterType::RELEASE>();
    }

    return parameterLimits[static_cast<int>(ParameterType::RELEASE)].second ;
}