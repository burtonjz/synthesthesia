#ifndef LFO_HPP_
#define LFO_HPP_

#include "waveform.hpp"
#include "oscillator-base.hpp"
#include "modulator.hpp"
#include "modulator-type.hpp"

class LFO: public Modulator {
private:
    float depth;
    BaseOscillator* oscillator;

public:
    LFO(Waveform wf, double f,float rate, double wave_range_min, double wave_range_max, float d);
    LFO();
    ~LFO();

    float modulate_frequency(double input) override;
    float modulate_amplitude(double input) override;
    float modulate_phase(double input) override;
    float modulate_filter_q(double input) override;
    float modulate_filter_fc(double input) override;
    // modulate filters? cutoff, resonance, etc..

    ModulatorType getType() const override;

    void set_freq(double f);

    void set_waveform(Waveform wf);

    void set_depth(float d);

    void tick();

    void reset();

};

#endif // LFO_HPP_