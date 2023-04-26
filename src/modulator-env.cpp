#include "modulator-env.hpp"

// CLASS CONSTRUCTORS

ADSREnvelope::ADSREnvelope(ADSR a):
    adsr(a)
{}

ADSREnvelope::ADSREnvelope():
    ADSREnvelope({0.0,0.0,0.0,0.0})
{}

// GETTERS/SETTERS

inline ModulatorType ADSREnvelope::getType() const {
    return MODULATOR_ENV;
}

inline uint16_t ADSREnvelope::get_connections() const{
    return connections;
}

inline void ADSREnvelope::set_connections(uint16_t c){
    connections = c;
}

inline ADSR ADSREnvelope::get_adsr() const{
    return adsr;
}
inline void ADSREnvelope::set_adsr(ADSR a){
    adsr = a;
}

inline float ADSREnvelope::get_level(KeyStatus status, double time, float start_level){
    return adsr.get_level(status,time,start_level);
}


// MODULATION FUNCTIONS

inline float ADSREnvelope::modulate_amplitude(double input, KeyStatus status, double time, float start_level){
    return input * adsr.get_level(status,time,start_level);
}

inline float ADSREnvelope::modulate_frequency(double input, KeyStatus status, double time, float start_level){
    return input * adsr.get_level(status,time,start_level);
}

inline float ADSREnvelope::modulate_phase(double input, KeyStatus status, double time, float start_level){
    return input * adsr.get_level(status,time,start_level);
}

inline float ADSREnvelope::modulate_amplitude(double input){
    return modulate_amplitude(input,KEY_OFF,0.0,0.0);
}

inline float ADSREnvelope::modulate_frequency(double input){
    return modulate_frequency(input,KEY_OFF,0.0,0.0);
}

inline float ADSREnvelope::modulate_phase(double input){
    return modulate_phase(input,KEY_OFF,0.0,0.0);
}