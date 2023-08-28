// Arguments required to create an oscillator

#ifndef OSCILLATOR_CFG_HPP_
#define OSCILLATOR_CFG_HPP_

#include <array>
#include "waveform.hpp"
#include "modulator.hpp"
#include "linear-fader.hpp"
#include "cfg-connection.hpp"

struct OscillatorConfig{
    bool status; 
    Waveform wf;
    LinearFader<float>* gain;
    double detune;
    std::array<Modulator*,OSC_CONNECT_N> modulators;
};

#endif // OSCILLATOR_CFG_HPP_