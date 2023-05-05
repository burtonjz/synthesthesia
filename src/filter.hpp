#ifndef FILTER_HPP_
#define FILTER_HPP_

#include "config.hpp"
#include <cmath>
#include <array>
#include "filter-type.hpp"
#include "linear-fader.hpp"


// Second order Butterworth filter
class LowPassFilter{
private:
    FilterType filter_type;
    double rate;
    double cutoff_freq;
    double q_factor;
    bool needs_recalculate;

    // sample logs
    std::array<std::array<float,N_CHANNELS>,3> input;
    std::array<std::array<float,N_CHANNELS>,3> output;

    // feedback coefficients
    double b1;
    double b2;

    // feedforward coefficients
    double a0;
    double a1;
    double a2;

public:
    LowPassFilter();

    void set_type(FilterType t);
    bool is_active();

    std::array<float,N_CHANNELS> get_sample() const;

    double get_cutoff_freq() const;
    void set_cutoff_freq(double f);
    double get_q_factor() const;
    void set_q_factor(double q);

    void reset();

    void tick(std::array<float,N_CHANNELS> inp);

private:
    void calculate_coefficients();
    std::array<float,N_CHANNELS> get_output_sample();
};

#endif // FILTER_HPP_