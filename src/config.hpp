#ifndef __CONFIG_HPP_
#define __CONFIG_HPP_

#define SYNTHESTHESIA_URI "https://github.com/burtonjz/synthesthesia"

constexpr int CONFIG_PITCHBEND_MAX_SHIFT = 2 ; // max semitone shift for pitchbend

constexpr float CONFIG_FADER_RELEASE_FADE_TIME = 0.1 ; // time after release for fader modulation to reach 0


constexpr int AUDIO_OUT_N = 2;

constexpr int WAVETABLE_SIZE = 1024 ;

#endif // __CONFIG_HPP_