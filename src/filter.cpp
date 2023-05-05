#include "filter.hpp"


LowPassFilter::LowPassFilter():
    filter_type(FILTER_OFF),
    rate(DEFAULT_SAMPLING_RATE),
    cutoff_freq(10000.0),
    q_factor(0.7071)
{
    reset(); // set inputs/outputs to 0
}

void LowPassFilter::set_type(FilterType t){
    filter_type = t;
    if(is_active()) calculate_coefficients();
    else reset();
}

bool LowPassFilter::is_active(){
    return filter_type != FILTER_OFF;
}

std::array<float,N_CHANNELS> LowPassFilter::get_sample() const {
    return output[0];
}

double LowPassFilter::get_cutoff_freq() const {
    return cutoff_freq;
}

void LowPassFilter::set_cutoff_freq(double f){
    cutoff_freq = f;
    if(is_active()) needs_recalculate = true;
    else reset();
}

double LowPassFilter::get_q_factor() const {
    return q_factor;
}

void LowPassFilter::set_q_factor(double q) {
    q_factor = q;
    if(is_active()) needs_recalculate = true;
}

void LowPassFilter::reset(){
    for(int i = 0; i < static_cast<int>(input.size()); ++i){
        input[i].fill(0.0f);
        output[i].fill(0.0f);
    }
}

void LowPassFilter::tick(std::array<float,N_CHANNELS> inp){
    if(needs_recalculate) calculate_coefficients();
    if(is_active()){
        // shift input/output samples
        input[2] = input[1];
        input[1] = input[0];
        input[0] = inp;
        
        output[2] = output[1];
        output[1] = output[0];
        output[0] = get_output_sample();
    } else reset();
}


// see pg. 484 The Audio Programming Book
void LowPassFilter::calculate_coefficients(){
    double w0, alpha, cosw0, b0;

    switch(filter_type){
    case FILTER_LP:
        w0 = 2.0 * M_PI * cutoff_freq / (rate/2.0);
        alpha = std::sin(w0) / (2.0 * q_factor);
        cosw0 = std::cos(w0);
        b0 = 1.0 + alpha;
        a0 = (1.0 - cosw0) / (2.0 * b0);
        a1 = (1.0 - cosw0) / b0;
        a2 = a0;
        b1 = (-2.0 * cosw0) / b0;
        b2 = (1.0 - alpha) / b0;
        break;
    case FILTER_HP:
        w0 = 2.0 * M_PI * cutoff_freq / (rate/2.0);
        alpha = std::sin(w0) / (2.0 * q_factor);
        cosw0 = std::cos(w0);
        b0 = 1.0 + alpha;
        a0 = (1.0 + cosw0) / (2.0 * b0);
        a1 = -(1.0 + cosw0) / b0;
        a2 = a0;
        b1 = (-2.0 * cosw0) / b0;
        b2 = (1.0 - alpha) / b0;
        break;
    default:
        break;
    }
}

std::array<float,N_CHANNELS> LowPassFilter::get_output_sample(){
    std::array<float,N_CHANNELS> out = {};
    for(int i = 0; i < N_CHANNELS; ++i){
        out[i] = a0*input[0][i] + a1*input[1][i] + a2*input[2][i] - b1*output[1][i] - b2*output[2][i];
    }   
    return out;
}
