#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <cstdint>
#include "waveform.hpp"

#ifndef TEST_MODE_
constexpr int N_OSCILLATORS  = 3;
constexpr int N_CHANNELS     = 2;

constexpr int N_ENVELOPES    = 1;
constexpr int N_LFOS         = 1;
constexpr int N_MODULATORS   = N_ENVELOPES + N_LFOS;

#else
constexpr int N_OSCILLATORS  = 1;
constexpr int N_CHANNELS     = 1;

constexpr int N_ENVELOPES    = 1;
constexpr int N_LFOS         = 1;
constexpr int N_MODULATORS   = N_ENVELOPES + N_LFOS;
#endif

// WAVEFORM GENERATION CONFIGS

constexpr int DEFAULT_SAMPLING_RATE = 48000;
constexpr Waveform DEFAULT_WAVEFORM = WAVE_SINE;
constexpr float DEFAULT_WAVE_RANGE_MIN = -1.0f;
constexpr float DEFAULT_WAVE_RANGE_MAX = 1.0f;
constexpr float DEFAULT_AMPLITUDE = 1.0f;
constexpr float DEFAULT_PHASE = 0.0f;

constexpr float CTRL_FADER_WEIGHT = 0.01; // What percentage of SAMPLING_RATE to use to fade values for control ports
constexpr float KEY_FADER_WEIGHT = 0.01; // What percentage of SAMPLING_RATE to use to fade values for Key class (if amplitude modulator envelope is not connected)

// MODULATOR CONNECTION LOGIC

enum ModConnection : uint16_t {
    MOD_CONNECT_NONE           = 0x0000,
    MOD_CONNECT_OSC1_AMP       = 0x0001,
    MOD_CONNECT_OSC1_FREQ      = 0x0002,
    MOD_CONNECT_OSC1_PHASE     = 0x0004,
    MOD_CONNECT_OSC2_AMP       = 0x0008,
    MOD_CONNECT_OSC2_FREQ      = 0x0010,
    MOD_CONNECT_OSC2_PHASE     = 0x0020,
    MOD_CONNECT_OSC3_AMP       = 0x0040,
    MOD_CONNECT_OSC3_FREQ      = 0x0080,
    MOD_CONNECT_OSC3_PHASE     = 0x0100,
    MOD_CONNECT_FILTER1_CUTOFF = 0x0200,
    MOD_CONNECT_FILTER1_Q      = 0x0400
};

constexpr bool MOD_IS_CONNECTED(uint16_t cval, ModConnection c ){
    return (cval & static_cast<uint16_t> (c)) != 0.0;
}

#endif // CONFIG_HPP_