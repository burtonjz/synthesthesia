// Oscillator without any of the Modulator components

#ifndef OSCILLATOR_BASE_HPP_
#define OSCILLATOR_BASE_HPP_

#include <cstdint>
#include <random>
#include <utility>
#include "waveform.hpp"
#include "linear-fader.hpp"
#include "config.hpp"

class BaseOscillator {
private:
    // BASE OSCILLATOR VARIABLES
    Waveform waveform;
    double freq; 
    double amp;
    double phase; // in radians
    double rate;
    double step; // current position in function
    double step_increment; // increment to current position
    bool is_active;
    double last_sample; // needed for triangle wave polyblep

    LinearFader<float>* gain;
    double detune;

    // DYNAMIC OR INSTANTANEOUS (MODULATED) VARIABLES
    std::pair<double,double> wave_range;
    double inst_freq;
    double inst_amp;
    double inst_phase; // in radians

    // FOR NOISE GENERATION
    std::minstd_rand rnd;
    std::uniform_real_distribution<float> distr;

    // handle necessary changes to oscillator after a value is set
    void _post_freq_set();
    void _post_amp_set();
    void _post_phase_set();
    
    double midi2freq(uint8_t note, double detune);
    double midi2freq(uint8_t note);

    // sample calculation
    float get_sine_sample();
    float get_square_sample();
    float get_saw_sample();
    float get_triangle_sample();
    float get_noise_sample();
    float get_experimental_sample();
    double poly_blep(double t);

public:  
    BaseOscillator(Waveform wf, double f, double r, double wave_range_min, double wave_range_max, double a, double p);
    BaseOscillator(Waveform wf,uint8_t note,double r,double wave_range_min,double wave_range_max, double a,double p);
    BaseOscillator(Waveform wf,double f,double r, double wave_range_min, double wave_range_max);
    BaseOscillator(Waveform wf,uint8_t note,double r);
    BaseOscillator(Waveform wf,double f,double r);
    BaseOscillator();
    virtual ~BaseOscillator();

    // GETTERS AND SETTERS

    // Inherent Oscillator vars (shouldn't get modulated)
    Waveform get_waveform() const;
    
    void set_waveform(Waveform w);

    double get_freq() const;
    void set_freq(double f);
    void set_freq(uint8_t note);

    double get_amp() const;
    void set_amp(double a);

    double get_phase() const;
    void set_phase(double p);

    double get_rate() const;
    void set_rate(double r);

    double get_step() const;
    void set_step(double p);

    bool get_is_active() const;

    void set_is_active(bool status);

    void set_gain_ptr(LinearFader<float>* ptr);

    void set_detune(float d);

    // Dynamic/Instantaneous vars (can be modulated)

    std::pair<double,double> get_wave_range() const;
    void set_wave_range(double min, double max);
    void set_wave_range(std::pair<double,double> wrng);

    double get_inst_freq() const;
    void set_inst_freq(double f);

    double get_inst_amp() const;
    void set_inst_amp(double a);

    double get_inst_phase() const;
    void set_inst_phase(double p);


    // SAMPLE GENERATION FUNCTIONS
    float get_sample();
    float squish_sample(float val);
    virtual void tick();
};

#endif //OSCILLATOR_HPP_