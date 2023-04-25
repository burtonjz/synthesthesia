#ifndef OSCILLATOR_HPP_
#define OSCILLATOR_HPP_

#include <cstdint>
#include "waveform.hpp"
#include "key-status.hpp"
#include "oscillator-base.hpp"
#include "cfg-oscillator.hpp"
#include "modulator.hpp"

class Key; // forward declaration for Key class

class Oscillator : public BaseOscillator {
private:
    Modulator* frequency_modulator;
    Modulator* amplitude_modulator;
    Modulator* phase_modulator;

    void modulate();
    void modulate_amplitude();
    void modulate_frequency();
    void modulate_phase();

    Key* key;
    int index;
public:  
    Oscillator(Waveform wf, double f, double r, double wave_range_min, double wave_range_max, double a, double p, Key* key);
    Oscillator(Waveform wf, uint8_t note, double r, double wave_range_min, double wave_range_max, double a, double p, Key* key);
    Oscillator(Waveform wf,double f,double r, double wave_range_min, double wave_range_max);
    Oscillator(Waveform wf,uint8_t note,double r);
    Oscillator(Waveform wf,double f,double r);
    Oscillator();
    
    // GETTERS/SETTERS
    double get_release(); // get release timing information from amplitude envelope (if exists)
    double get_env_level(KeyStatus status, double time, float start_level) const; // get level from amplitude envelope (if exists)
    void set_index(int i); // index of this oscillator within Key class
    void set_key(Key* key); // set pointer to parent key (necessary to synchronize envelope modulators to key presses/timing)

    void tick() override; // move oscillator to next sample
    void configure(OscillatorConfig config); // set oscillator members based on parameter object

    void connect_frequency_modulator(Modulator* ptr);
    void disconnect_frequency_modulator();

    void connect_amplitude_modulator(Modulator* ptr);
    void disconnect_amplitude_modulator();

    void connect_phase_modulator(Modulator* ptr);
    void disconnect_phase_modulator();

    void disconnect_modulators();
};

#endif //OSCILLATOR_HPP_