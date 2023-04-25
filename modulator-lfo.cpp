#include "modulator-lfo.hpp"
#include "config.hpp"

// CONSTRUCTOR/DESTRUCTORS

LFO::LFO(Waveform wf, double f, float rate, double wave_range_min, double wave_range_max, float d):
    depth(d),
    oscillator(new BaseOscillator(wf,f,rate,wave_range_min,wave_range_max))
{
};

LFO::LFO():
    LFO(WAVE_SINE,0.1,DEFAULT_SAMPLING_RATE,0.0,1.0,1.0)
{};

LFO::~LFO(){
    delete(oscillator);
}

ModulatorType LFO::getType() const {
    return MODULATOR_LFO;
}; 

void LFO::set_is_active(bool status){
    if(status != is_active){
        is_active = status;
        oscillator->set_step(0.0);
    }
};

inline void LFO::set_freq(double f){
    oscillator->set_freq(f);
};

inline void LFO::set_waveform(Waveform wf){
    oscillator->set_waveform(wf);
};

inline void LFO::set_depth(float d){
    depth = d;
}

inline void LFO::tick(){
    oscillator->tick();
}


inline float LFO::modulate_frequency(double input){
    return input * oscillator->get_sample() * depth;
};
inline float LFO::modulate_amplitude(double input){
    return input * oscillator->get_sample() * depth;
};
inline float LFO::modulate_phase(double input){
    return input * oscillator->get_sample() * depth;
};