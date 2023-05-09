#ifndef PORT_INFO_HPP_
#define PORT_INFO_HPP_

#include <array>
#include <utility>

enum PortGroups {
    PORT_MIDI_IN      = 0,
    PORT_AUDIO_OUT    = 1,
    PORT_CTRL         = 2,
    PORT_N            = 3
};

enum MidiPorts {
    MIDI_IN = 0,
    MIDI_N  = 1
};

enum AudioPorts {
    AUDIO_OUT_L  = 0,
    AUDIO_OUT_R  = 1,
    AUDIO_OUT_N  = 2
};

enum ControlPorts {
    CTRL_OSC1_STATUS      = 0,
    CTRL_OSC1_WAVEFORM    = 1,
    CTRL_OSC1_GAIN        = 2,
    CTRL_OSC1_DETUNE      = 3,
    CTRL_OSC1_PAN         = 4,
    CTRL_OSC2_STATUS      = 5,
    CTRL_OSC2_WAVEFORM    = 6,
    CTRL_OSC2_GAIN        = 7,
    CTRL_OSC2_DETUNE      = 8,
    CTRL_OSC2_PAN         = 9,
    CTRL_OSC3_STATUS      = 10,
    CTRL_OSC3_WAVEFORM    = 11,
    CTRL_OSC3_GAIN        = 12,
    CTRL_OSC3_DETUNE      = 13,
    CTRL_OSC3_PAN         = 14,
    CTRL_ENV1_CONNECTIONS = 15,
    CTRL_ENV1_ATTACK      = 16,
    CTRL_ENV1_DECAY       = 17,
    CTRL_ENV1_SUSTAIN     = 18,
    CTRL_ENV1_RELEASE     = 19,
    CTRL_LFO1_CONNECTIONS = 20,
    CTRL_LFO1_WAVEFORM    = 21,
    CTRL_LFO1_FREQ        = 22,
    CTRL_LFO1_DEPTH       = 23,
    CTRL_FILTER1_TYPE     = 24,
    CTRL_FILTER1_FREQ     = 25,
    CTRL_FILTER1_RES      = 26,
    CTRL_N                = 27
};

// Oscillator Looping Functions
constexpr int OSC_GET_STATUS(int i){return i * 5;}
constexpr int OSC_GET_WAVEFORM(int i){return i * 5 + 1;}
constexpr int OSC_GET_GAIN(int i){return i * 5 + 2;}
constexpr int OSC_GET_DETUNE(int i){return i * 5 + 3;}
constexpr int OSC_GET_PAN(int i){return i * 5 + 4;}


const int TOTAL_PORTS = MIDI_N + AUDIO_OUT_N + CTRL_N;

// value ranges for control ports
constexpr std::array<std::pair<float, float>, CTRL_N> ctrlLimits = {{
    {0.0f   , 1.0f},     // CTRL_OSC1_STATUS
    {0.0f   , 5.0f},     // CTRL_OSC1_WAVEFORM
    {0.0f   , 1.0f},     // CTRL_OSC1_GAIN
    {-24.5  , 24.5},     // CTRL_OSC1_DETUNE
    {-1.0f  , 1.0f},     // CTRL_OSC1_PAN
    {0.0f   , 1.0f},     // CTRL_OSC2_STATUS
    {0.0f   , 5.0f},     // CTRL_OSC2_WAVEFORM
    {0.0f   , 1.0f},     // CTRL_OSC2_GAIN
    {-24.5  , 24.5},     // CTRL_OSC2_DETUNE
    {-1.0f  , 1.0f},     // CTRL_OSC2_PAN
    {0.0f   , 1.0f},     // CTRL_OSC3_STATUS
    {0.0f   , 5.0f},     // CTRL_OSC3_WAVEFORM
    {0.0f   , 1.0f},     // CTRL_OSC3_GAIN
    {-24.5  , 24.5},     // CTRL_OSC3_DETUNE
    {-1.0f  , 1.0f},     // CTRL_OSC3_PAN
    {0.0f   , 65535.0f}, // CTRL_ENV1_CONNECTIONS
    {0.001f , 4.0f},     // CTRL_ENV1_ATTACK
    {0.001f , 4.0f},     // CTRL_ENV1_DECAY
    {0.0f   , 1.0f},     // CTRL_ENV1_SUSTAIN
    {0.001f , 4.0f},     // CTRL_ENV1_RELEASE
    {0.0f   , 65535.0f}, // CTRL_LFO1_CONNECTIONS
    {0.0f   , 5.0f},     // CTRL_LFO1_WAVEFORM
    {0.1f   , 20.0f},    // CTRL_LFO1_FREQ
    {0.0f   , 1.0f},     // CTRL_LFO1_DEPTH
    {0.0f   , 4.0f},     // CTRL_FILTER1_TYPE
    {0.0f   , 20000.0f}, // CTRL_FILTER1_FREQ
    {0.5f   , 10.0f}     // CTRL_FILTER1_RES
}};

#endif // PORT_INFO_HPP_