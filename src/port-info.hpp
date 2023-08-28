#ifndef PORT_INFO_HPP_
#define PORT_INFO_HPP_

#include <array>
#include <utility>
#include "config.hpp"

enum MidiPorts {
    MIDI_IN = 0,
    MIDI_N  = 1
};

enum AudioPorts {
    AUDIO_OUT_L  = 0,
    AUDIO_OUT_R  = 1,
    AUDIO_OUT_N  = 2
};

enum OscPorts {
    CTRL_OSC_STATUS   = 0,
    CTRL_OSC_WAVEFORM = 1,
    CTRL_OSC_GAIN     = 2,
    CTRL_OSC_DETUNE   = 3,
    CTRL_OSC_PAN      = 4,
    CTRL_OSC_N        = 5
};

constexpr std::array<std::pair<float, float>, CTRL_OSC_N> OscLimits {{
    {0.0f   , 1.0f},
    {0.0f   , 5.0f},
    {0.0f   , 1.0f},
    {-24.5  , 24.5}, 
    {-1.0f  , 1.0f}
}};

enum EnvPorts {
    CTRL_ENV_CONNECTIONS = 0,
    CTRL_ENV_ATTACK      = 1,
    CTRL_ENV_DECAY       = 2,
    CTRL_ENV_SUSTAIN     = 3,
    CTRL_ENV_RELEASE     = 4,
    CTRL_ENV_N           = 5
};
constexpr std::array<std::pair<float, float>, CTRL_OSC_N> EnvLimits {{
    {-__FLT_MAX__   , __FLT_MAX__}, 
    {0.001f , 4.0f},    
    {0.001f , 4.0f},    
    {0.0f   , 1.0f},
    {0.001f , 4.0f}
}};

enum LfoPorts {
    CTRL_LFO_CONNECTIONS = 0,
    CTRL_LFO_WAVEFORM    = 1,
    CTRL_LFO_FREQ        = 2,
    CTRL_LFO_DEPTH       = 3,
    CTRL_LFO_N           = 4
};
constexpr std::array<std::pair<float, float>, CTRL_OSC_N> LfoLimits {{
    {-__FLT_MAX__   , __FLT_MAX__}, 
    {0.0f   , 5.0f},     
    {0.1f   , 20.0f},    
    {0.0f   , 1.0f}
}};

enum FilterPorts {
    CTRL_FILTER_TYPE = 0,
    CTRL_FILTER_FREQ = 1,
    CTRL_FILTER_RES  = 2,
    CTRL_FILTER_N    = 3
};
constexpr std::array<std::pair<float, float>, CTRL_OSC_N> FilterLimits {{
    {0.0f   , 4.0f}, 
    {0.0f   , 20000.0f},
    {0.5f   , 10.0f}    
}};

const std::pair<int,int> PORT_MIDI_RANGE = {
    0,
    MIDI_N - 1
};
const std::pair<int,int> PORT_AUDIO_OUT_RANGE = {
    PORT_MIDI_RANGE.second + 1,
    PORT_MIDI_RANGE.second + AUDIO_OUT_N
};
const std::pair<int,int> PORT_OSC_RANGE = {
    PORT_AUDIO_OUT_RANGE.second + 1,
    PORT_AUDIO_OUT_RANGE.second + CTRL_OSC_N * N_OSCILLATORS
};
const std::pair<int,int> PORT_ENV_RANGE = {
    PORT_OSC_RANGE.second + 1,
    PORT_OSC_RANGE.second + CTRL_ENV_N * N_ENVELOPES
};
const std::pair<int,int> PORT_LFO_RANGE = {
    PORT_ENV_RANGE.second + 1,
    PORT_ENV_RANGE.second + CTRL_LFO_N * N_LFOS
};
const std::pair<int,int> PORT_FILTER_RANGE = {
    PORT_LFO_RANGE.second + 1,
    PORT_LFO_RANGE.second + CTRL_FILTER_N * N_FILTERS
};

const int TOTAL_PORTS = MIDI_N + 
                        AUDIO_OUT_N + 
                        CTRL_OSC_N * N_OSCILLATORS + 
                        CTRL_ENV_N * N_ENVELOPES + 
                        CTRL_LFO_N * N_LFOS +
                        CTRL_FILTER_N * N_FILTERS
;

#endif // PORT_INFO_HPP_