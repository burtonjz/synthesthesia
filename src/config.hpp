#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#define SYNTHESTHESIA_URI "https://github.com/burtonjz/synthesthesia"

constexpr int CONFIG_PITCHBEND_MAX_SHIFT = 2 ; // max semitone shift for pitchbend
constexpr float CONFIG_LINEAR_FADER_FADE_TIME = 0.1 ; // time after release/press for fader modulation to reach 0/value (respectively)
constexpr int WAVETABLE_SIZE = 1024 ;

constexpr int N_POLY_OSCILLATORS = 1 ;
constexpr int N_ENVELOPES = 1 ;

constexpr int CONFIG_DETUNE_MAX_CENTS = 1250 ; // max detune in one direction
constexpr int CONFIG_DETUNE_MAX_CENTS_2 = CONFIG_DETUNE_MAX_CENTS * 2 ;

constexpr int CONFIG_MAX_NUM_CONTROL_PORTS = 1000 ; 

#endif // __CONFIG_HPP_