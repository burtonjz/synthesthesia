#include "filter.hpp"
#include "modulator-env.hpp"
#include "modulator-lfo.hpp"
#include "key-status.hpp"
#include "synthesthesia.hpp"

Filter::Filter():
    filter_type(FILTER_OFF),
    rate(DEFAULT_SAMPLING_RATE),
    cutoff_freq(10000.0),
    q_factor(0.7071),
    mod_q(nullptr),
    mod_fc(nullptr)
{
    reset();
}

void Filter::set_type(FilterType t){
    filter_type = t;
    if(is_active()) calculate_coefficients();
    else reset();
}

bool Filter::is_active(){
    return filter_type != FILTER_OFF;
}

std::array<float,N_CHANNELS> Filter::get_sample() const {
    return output[0];
}

double Filter::get_cutoff_freq() const {
    return cutoff_freq;
}

void Filter::set_cutoff_freq(double f){
    cutoff_freq = f;
    inst_cutoff_freq = f;
    if(is_active()) needs_recalculate = true;
}

double Filter::get_q_factor() const {
    return q_factor;
}

void Filter::set_q_factor(double q) {
    q_factor = q;
    inst_q_factor = q;
    if(is_active()) needs_recalculate = true;
}

double Filter::get_inst_cutoff_freq() const {
    return inst_cutoff_freq;
}

void Filter::set_inst_cutoff_freq(double f){
    inst_cutoff_freq = f;
    if(is_active()) needs_recalculate = true;
    else reset();
}

double Filter::get_inst_q_factor() const {
    return inst_q_factor;
}

void Filter::set_inst_q_factor(double q) {
    inst_q_factor = q;
    if(is_active()) needs_recalculate = true;
}




void Filter::set_synth_ptr(Synthesthesia* ptr){
    synth_ptr = ptr;
}

// MODULATION

void Filter::connect_modulator_q(Modulator* qmod){
    mod_q = qmod;
}
void Filter::disconnect_modulator_q(){
    mod_q = nullptr;
    inst_q_factor = q_factor;
    needs_recalculate = true;
}

void Filter::connect_modulator_fc(Modulator* fcmod){
    mod_fc = fcmod;
}
void Filter::disconnect_modulator_fc(){
    mod_fc = nullptr;
    inst_cutoff_freq = cutoff_freq;
    needs_recalculate = true;
}

void Filter::modulate_q(){
    if(!mod_q || !mod_q->get_is_active()) return;

    if(ADSREnvelope* qenv = static_cast<ADSREnvelope*>(mod_q)){
        std::tuple<KeyStatus,double,float> params = synth_ptr->get_global_key_params();
        set_inst_q_factor(get_q_factor() + qenv->modulate_filter_q(get_q_factor(),std::get<0>(params),std::get<1>(params),std::get<2>(params)));
    }

    if(LFO* qlfo = static_cast<LFO*>(mod_q)){
        set_inst_q_factor(get_q_factor() + qlfo->modulate_filter_q(get_q_factor()));
    }
}

void Filter::modulate_fc(){
    if(!mod_fc || !mod_fc->get_is_active()) return;

    if(ADSREnvelope* fcenv = static_cast<ADSREnvelope*>(mod_fc)){
        std::tuple<KeyStatus,double,float> params = synth_ptr->get_global_key_params();
        set_inst_cutoff_freq(get_cutoff_freq() + fcenv->modulate_filter_q(get_cutoff_freq(),std::get<0>(params),std::get<1>(params),std::get<2>(params)));
    }

    if(LFO* fclfo = static_cast<LFO*>(mod_fc)){
        set_inst_cutoff_freq(get_cutoff_freq() + fclfo->modulate_filter_fc(get_cutoff_freq()));
    }
}

void Filter::modulate(){
    modulate_fc();
    modulate_q();
}

void Filter::reset(){
    for(int i = 0; i < static_cast<int>(input.size()); ++i){
        input[i].fill(0.0f);
        output[i].fill(0.0f);
    }
}



void Filter::tick(std::array<float,N_CHANNELS> inp){
    modulate();
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
void Filter::calculate_coefficients(){
    double w0, alpha, cosw0, a0;

    switch(filter_type){
    case FILTER_LP:
        w0 = 2.0 * M_PI * inst_cutoff_freq / (rate/2.0); // normalized by nyquist frequency
        alpha = std::sin(w0) / (2.0 * inst_q_factor);
        cosw0 = std::cos(w0);
        a0 = 1.0 + alpha;
        b0 = (1.0 - cosw0) / (2.0 * a0);
        b1 = (1.0 - cosw0) / a0;
        b2 = b0;
        a1 = (-2.0 * cosw0) / a0;
        a2 = (1.0 - alpha) / a0;
        break;
    case FILTER_HP:
        w0 = 2.0 * M_PI * inst_cutoff_freq / (rate/2.0);
        alpha = std::sin(w0) / (2.0 * inst_q_factor);
        cosw0 = std::cos(w0);
        a0 = 1.0 + alpha;
        b0 = (1.0 + cosw0) / (2.0 * a0);
        b1 = -(1.0 + cosw0) / a0;
        b2 = b0;
        a1 = (-2.0 * cosw0) / a0;
        a2 = (1.0 - alpha) / a0;
        break;
    default:
        break;
    }
}

std::array<float,N_CHANNELS> Filter::get_output_sample(){
    std::array<float,N_CHANNELS> out = {};
    for(int i = 0; i < N_CHANNELS; ++i){
        out[i] = b0*input[0][i] + b1*input[1][i] + b2*input[2][i] - a1*output[1][i] - a2*output[2][i];
    }   
    return out;
}
