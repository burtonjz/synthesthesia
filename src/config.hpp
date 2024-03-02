#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#define SYNTHESTHESIA_URI "https://github.com/burtonjz/synthesthesia"

constexpr int WAVETABLE_SIZE = 1024 ;
constexpr int N_POLY_OSCILLATORS = 1 ;
constexpr int CONFIG_MAX_NUM_CONTROL_PORTS = 1000 ; 

// MODULATION CONFIG
constexpr int N_FADERS = 1 ;
constexpr int N_ENVELOPES = 1 ;
constexpr int N_DETUNERS = 1 ;
constexpr int N_LFOS = 1 ;

constexpr int N_MODULATORS = 1 // Midi
    + N_FADERS
    + N_ENVELOPES
    + N_DETUNERS
    + N_LFOS
;

constexpr float CONFIG_LINEAR_FADER_FADE_TIME = 0.1 ; // time after release/press for fader modulation to reach 0/value (respectively)

constexpr int CONFIG_DETUNE_MAX_CENTS = 1250 ; // max detune in one direction
constexpr int CONFIG_DETUNE_MAX_CENTS_2 = CONFIG_DETUNE_MAX_CENTS * 2 ;

constexpr int CONFIG_PITCHBEND_MAX_SHIFT = 2 ; // max semitone shift for pitchbend

constexpr float CONFIG_LFO_DEFAULT_FREQ = 2.0 ;
constexpr float CONFIG_LFO_MIN_FREQ = 0.01 ;
constexpr float CONFIG_LFO_MAX_FREQ = 20.0 ;

constexpr int CONFIG_MAX_MODULATION_CHAIN = 5 ; // total number of modulators that can be present in a chain. CANNOT BE GREATER THAN 5.

// for ModulationController encoding
constexpr int CONFIG_MODULATOR_TYPE_N_BITS = 4 ;
constexpr int CONFIG_MODULATOR_INST_N_BITS = 2 ;
constexpr int CONFIG_MODULATOR_N_BITS = CONFIG_MODULATOR_INST_N_BITS + CONFIG_MODULATOR_TYPE_N_BITS ;
constexpr int CONFIG_MAX_MODULATION_ENCODED_VALUE = ( 1 << (CONFIG_MODULATOR_N_BITS * CONFIG_MAX_MODULATION_CHAIN)) - 1 ;


#endif // __CONFIG_HPP_