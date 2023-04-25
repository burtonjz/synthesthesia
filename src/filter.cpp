#include "filter.hpp"


LowPassFilter::LowPassFilter():
    filter_type(FILTER_LP),
    rate(DEFAULT_SAMPLING_RATE),
    cutoff_freq(LinearFader<double>(10000.0))
{
    reset(); // set inputs/outputs to 0
}

inline void LowPassFilter::set_type(FilterType t){
    filter_type = t;
    if(is_active()) calculate_coefficients();
    else reset();
}

inline bool LowPassFilter::is_active(){
    return filter_type != FILTER_OFF;
}

inline std::array<float,N_CHANNELS> LowPassFilter::get_sample() const {
    return output[0];
}

inline double LowPassFilter::get_cutoff_freq() const {
    return cutoff_freq.get();
}

inline void LowPassFilter::set_cutoff_freq(double f){
    cutoff_freq.set(f,rate * 0.01);
    if(is_active()) calculate_coefficients();
    else reset();
}

inline void LowPassFilter::reset(){
    for(int i = 0; i < static_cast<int>(input.size()); ++i){
        input[i].fill(0.0f);
        output[i].fill(0.0f);
    }
}

void LowPassFilter::tick(std::array<float,N_CHANNELS> inp){
    cutoff_freq.tick();
    if(is_active()) calculate_coefficients();
    else reset();

    // shift input/output samples
    input[2] = input[1];
    input[1] = input[0];
    input[0] = inp;
    
    output[2] = output[1];
    output[1] = output[0];
    output[0] = get_output_sample();
}


// see pg. 484 The Audio Programming Book
void LowPassFilter::calculate_coefficients(){
    double gamma;

    switch(filter_type){
    case FILTER_LP:
        gamma = 1 / std::tan(M_PI * (cutoff_freq.get() / rate ));
        a0 = 1.0 / (1.0 + 2.0*gamma + gamma*gamma);
        a1 = 2.0 * a0;
        a2 = a0; 
        b1 = 2.0 * a0 * ( 1.0 - gamma*gamma);
        b2 = a0 * (1.0 - 2.0*gamma + gamma*gamma);
        break;
    case FILTER_HP:
        gamma = std::tan(M_PI * (cutoff_freq.get() / rate ));
        a0 = 1.0 / (1.0 + 2.0*gamma + gamma*gamma);
        a1 = 2.0 * a0;
        a2 = a0;
        b1 = 2.0 * a0 * (gamma*gamma - 1.0);
        b2 = a0 * (1.0 - 2.0*gamma + gamma*gamma);
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
