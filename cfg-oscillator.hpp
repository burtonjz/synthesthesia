// Arguments required to create an oscillator

#ifndef OSCILLATOR_CFG_HPP_
#define OSCILLATOR_CFG_HPP_

#include "waveform.hpp"
#include "modulator.hpp"
#include "linear-fader.hpp"

struct OscillatorConfig{
    bool status; 
    Waveform wf;
    Modulator* freq_mod;
    Modulator* amp_mod;
    Modulator* phase_mod;
    LinearFader<float>* gain;
    double detune;
};

#endif // OSCILLATOR_CFG_HPP_