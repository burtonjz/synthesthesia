#ifndef KEY_HPP_
#define KEY_HPP_

#include <cstdint>
#include <array>
#include "key-status.hpp"
#include "linear-fader.hpp"
#include "oscillator.hpp"
#include "cfg-oscillator.hpp"
#include "config.hpp"

class Synthesthesia; // forward declaration

class Key {
private:
    KeyStatus status;
    double time;
    double rate;
    uint8_t note;
    uint8_t velocity;
    LinearFader<float> keyFader; // for key press and mute
    std::array<Oscillator,N_OSCILLATORS> oscillator;
    
    // for handling amp envelope modulation
    std::array<float,N_OSCILLATORS> start_level;
public:
    Key();
    Key(const double rt);
    ~Key();

    KeyStatus get_status() const;
    double get_time() const;
    float get_start_level(int index) const; 
    double get_rate() const;
    
    void press(const std::array<OscillatorConfig,N_OSCILLATORS> osc_config,
        const uint8_t nt, const uint8_t vel, Synthesthesia* synth_ptr);

    void release();
    void release(const uint8_t nt);

    void off();

    void mute();

    std::array<float,N_OSCILLATORS> get_sample();

    void tick();

    bool isOn();
};

#endif // KEY_HPP_