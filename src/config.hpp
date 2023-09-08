#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <cstdint>
#include "waveform.hpp"

#define SYNTHESTHESIA_URI "https://github.com/burtonjz/synthesthesia"

// INPUTS, OUTPUTS, MODULES

constexpr int N_CHANNELS     = 2; // number of audio out channels. Currently must be 1 or 2 (mono/stereo)
constexpr int N_OSCILLATORS  = 3;
constexpr int N_ENVELOPES    = 1;
constexpr int N_LFOS         = 1;
constexpr int N_MODULATORS   = N_ENVELOPES + N_LFOS;
constexpr int N_FILTERS      = 1;

constexpr int N_MODULES      = N_OSCILLATORS + N_ENVELOPES + N_LFOS + N_FILTERS;

// WAVEFORM GENERATION CONFIGS

constexpr int DEFAULT_SAMPLING_RATE = 48000;
constexpr Waveform DEFAULT_WAVEFORM = WAVE_SINE;
constexpr float DEFAULT_WAVE_RANGE_MIN = -1.0f;
constexpr float DEFAULT_WAVE_RANGE_MAX = 1.0f;
constexpr float DEFAULT_AMPLITUDE = 1.0f;
constexpr float DEFAULT_PHASE = 0.0f;

constexpr float CTRL_FADER_WEIGHT = 0.01; // What percentage of SAMPLING_RATE to use to fade values for control ports
constexpr float KEY_FADER_WEIGHT = 0.01; // What percentage of SAMPLING_RATE to use to fade values for Key class (if amplitude modulator envelope is not connected)

#endif // CONFIG_HPP_