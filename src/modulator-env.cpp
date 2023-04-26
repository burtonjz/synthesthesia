#include "modulator-env.hpp"

// CLASS CONSTRUCTORS

ADSREnvelope::ADSREnvelope(ADSR a):
    Modulator(true), // connections handled elsewhere for Envelopes
    adsr(a)
{}

ADSREnvelope::ADSREnvelope():
    ADSREnvelope({0.0,0.0,0.0,0.0})
{}

// GETTERS/SETTERS

ModulatorType ADSREnvelope::getType() const {
    return MODULATOR_ENV;
}

uint16_t ADSREnvelope::get_connections() const{
    return connections;
}

void ADSREnvelope::set_connections(uint16_t c){
    connections = c;
}

ADSR ADSREnvelope::get_adsr() const{
    return adsr;
}
void ADSREnvelope::set_adsr(ADSR a){
    adsr = a;
}

float ADSREnvelope::get_level(KeyStatus status, double time, float start_level){
    return adsr.get_level(status,time,start_level);
}


// MODULATION FUNCTIONS

float ADSREnvelope::modulate_amplitude(double input, KeyStatus status, double time, float start_level){
    return input * adsr.get_level(status,time,start_level);
}

float ADSREnvelope::modulate_frequency(double input, KeyStatus status, double time, float start_level){
    return input * adsr.get_level(status,time,start_level);
}

float ADSREnvelope::modulate_phase(double input, KeyStatus status, double time, float start_level){
    return input * adsr.get_level(status,time,start_level);
}

float ADSREnvelope::modulate_amplitude(double input){
    return modulate_amplitude(input,KEY_OFF,0.0,0.0);
}

float ADSREnvelope::modulate_frequency(double input){
    return modulate_frequency(input,KEY_OFF,0.0,0.0);
}

float ADSREnvelope::modulate_phase(double input){
    return modulate_phase(input,KEY_OFF,0.0,0.0);
}