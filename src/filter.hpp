#ifndef FILTER_HPP_
#define FILTER_HPP_

#include "config.hpp"
#include <cmath>
#include <array>
#include "filter-type.hpp"
#include "linear-fader.hpp"
#include "modulator.hpp"

class Synthesthesia; // forward declaration for Synthesthesia class

// Second order Butterworth filter
class Filter{
private:
    FilterType filter_type;
    double rate;
    double cutoff_freq;
    double q_factor;
    double inst_cutoff_freq;
    double inst_q_factor;
    bool needs_recalculate;

    // sample logs
    std::array<std::array<float,N_CHANNELS>,3> input;
    std::array<std::array<float,N_CHANNELS>,3> output;

    // feedback coefficients
    double a1;
    double a2;

    // feedforward coefficients
    double b0;
    double b1;
    double b2;

    // modulation
    Modulator* mod_fc;
    Modulator* mod_q;

    Synthesthesia* synth_ptr;

public:
    Filter();

    void set_type(FilterType t);
    bool is_active();

    std::array<float,N_CHANNELS> get_sample() const;

    double get_cutoff_freq() const;
    void set_cutoff_freq(double f);

    double get_q_factor() const;
    void set_q_factor(double q);

    double get_inst_cutoff_freq() const;
    void set_inst_cutoff_freq(double f);

    double get_inst_q_factor() const;
    void set_inst_q_factor(double q);

    void set_synth_ptr(Synthesthesia* ptr);

    void connect_modulator_q(Modulator* qmod);
    void disconnect_modulator_q();

    void connect_modulator_fc(Modulator* fcmod);
    void disconnect_modulator_fc();

    void modulate_q();
    void modulate_fc();
    void modulate();

    void reset();

    void tick(std::array<float,N_CHANNELS> inp);

private:
    void calculate_coefficients();
    std::array<float,N_CHANNELS> get_output_sample();
};

#endif // FILTER_HPP_