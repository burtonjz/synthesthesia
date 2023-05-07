#include "oscillator-base.hpp"
#include <ctime>
#include <cmath>

// CONSTRUCTORS
BaseOscillator::BaseOscillator(Waveform wf,double f,double r,double wave_range_min,double wave_range_max, double a,double p):
    waveform(wf),
    rate(r),
    step(0.0),
    is_active(true),
    last_sample(0),
    gain(nullptr),
    detune(0.0),
    wave_range(wave_range_min,wave_range_max),
    rnd(std::time(0)),
    distr(-1.0f,1.0f)
{
    set_freq(f);
    set_phase(p);
    set_amp(a);
}

BaseOscillator::BaseOscillator(Waveform wf,uint8_t note,double r,double wave_range_min,double wave_range_max, double a,double p):
    waveform(wf),
    rate(r),
    step(0.0),
    is_active(true),
    last_sample(0.0),
    gain(nullptr),
    detune(0.0),
    wave_range(wave_range_min,wave_range_max),
    rnd(std::time(0)),
    distr(-1.0f,1.0f)
{
    set_freq(note);
    set_phase(p);
    set_amp(a);
}

BaseOscillator::BaseOscillator(Waveform wf,double f,double r, double wave_range_min, double wave_range_max):
    BaseOscillator::BaseOscillator(wf,f,r,wave_range_min,wave_range_max,DEFAULT_AMPLITUDE,DEFAULT_PHASE)
{}

BaseOscillator::BaseOscillator(Waveform wf,uint8_t note,double r):
    BaseOscillator::BaseOscillator(wf,note,r,DEFAULT_WAVE_RANGE_MIN,DEFAULT_WAVE_RANGE_MAX,DEFAULT_AMPLITUDE,DEFAULT_PHASE)
{}

BaseOscillator::BaseOscillator(Waveform wf, double f, double r):
    BaseOscillator::BaseOscillator(wf,f,r,DEFAULT_WAVE_RANGE_MIN,DEFAULT_WAVE_RANGE_MAX,DEFAULT_AMPLITUDE,DEFAULT_PHASE)
{}

BaseOscillator::BaseOscillator():
    BaseOscillator::BaseOscillator(WAVE_SINE,static_cast <uint8_t> (0),DEFAULT_SAMPLING_RATE)
{}

BaseOscillator::~BaseOscillator(){};

// GETTERS/SETTERS

Waveform BaseOscillator::get_waveform() const {
    return waveform;
}

void BaseOscillator::set_waveform(Waveform w){
    waveform = w;
}

double BaseOscillator::get_freq() const {
    return freq; 
}
void BaseOscillator::set_freq(double f){
    freq = f;
    inst_freq = f;
    _post_freq_set();
}
void BaseOscillator::set_freq(uint8_t note){ // set freq through midi note
    freq = midi2freq(note);
    inst_freq = freq;
    _post_freq_set();
}

double BaseOscillator::get_amp() const {
    return amp;
}
void BaseOscillator::set_amp(double a){
    amp = a;
    inst_amp = a;
    _post_amp_set();
}

double BaseOscillator::get_phase() const {
    return phase;
}
void BaseOscillator::set_phase(double p){
    phase = p;
    inst_phase = p;
    _post_phase_set();
}

double BaseOscillator::get_rate() const {
    return rate;
}
void BaseOscillator::set_rate(double r){
    rate = r;
}

double BaseOscillator::get_step() const {
    return step; 
}
void BaseOscillator::set_step(double p){  // maybe not this one? CHECK!!
    step = p; 
}

bool BaseOscillator::get_is_active() const {
    return is_active;
}

void BaseOscillator::set_is_active(bool status){
    is_active = status;
}

void BaseOscillator::set_gain_ptr(LinearFader<float>* ptr){
    gain = ptr;
}

void BaseOscillator::set_detune(float d){
    detune = d;
}

// Dynamic/Instantaneous Vars (can be modulated)
std::pair<double,double> BaseOscillator::get_wave_range() const {
    return wave_range;
}
void BaseOscillator::set_wave_range(double min, double max){
    wave_range = std::make_pair(min,max);
}
void BaseOscillator::set_wave_range(std::pair<double,double> wrng){
    wave_range = wrng;
}

double BaseOscillator::get_inst_freq() const {
    return inst_freq; 
}
void BaseOscillator::set_inst_freq(double f){
    inst_freq = f;
    _post_freq_set();
}

double BaseOscillator::get_inst_amp() const {
    return inst_amp;
}
void BaseOscillator::set_inst_amp(double a){
    inst_amp = a;
    _post_amp_set();
}

double BaseOscillator::get_inst_phase() const {
    return inst_phase;
}
void BaseOscillator::set_inst_phase(double p){
    inst_phase = p;
    _post_phase_set();
}

// SAMPLE GENERATION FUNCTIONS

// reduce aliasing for square/saw waves
double BaseOscillator::poly_blep(double t){
    double dt = step_increment / (M_PI * 2.0);
    if(t < dt){
        t /= dt;
        return t + t - t * t - 1.0;
    }
    else if(t > 1.0f - dt){
        t = (t - 1.0) / dt;
        return t * t + t + t + 1.0;
    }
    return 0.0;
}

// make sample value fit within wave range
float BaseOscillator::squish_sample(float val){ 
    return (((val + 1) / 2 ) * (wave_range.second - wave_range.first)) + wave_range.first;
}

void BaseOscillator::_post_freq_set(){
    switch(waveform){
    case WAVE_SINE:
        step_increment = (2.0 * M_PI * inst_freq ) / rate;
        break;
    case WAVE_TRIANGLE:
        step_increment = (2.0 * M_PI * inst_freq ) / rate;
        break;
    case WAVE_SQUARE:
        step_increment = (2.0 * M_PI * inst_freq ) / rate;
        break;
    case WAVE_SAW:
        step_increment = (2.0 * M_PI * inst_freq ) / rate;
        break;
    default:
        break;
    }
}

void BaseOscillator::_post_amp_set(){
    switch(waveform){
    case WAVE_SINE:
        break;
    case WAVE_TRIANGLE:
        break;
    case WAVE_SQUARE:
        break;
    case WAVE_SAW:
        break;
    default:
        break;
    }  
}

void BaseOscillator::_post_phase_set(){
    switch(waveform){
    case WAVE_SINE:
        break;
    case WAVE_TRIANGLE:
        break;
    case WAVE_SQUARE:
        break;
    case WAVE_SAW:
        break;
    default:
        break;
    }
}

double BaseOscillator::midi2freq(uint8_t note, double detune ){
    double f;
    f = pow(2.0, (static_cast <double> (note) - 69.0 + detune) / 12.0) * 440.0;
    return f;
}

double BaseOscillator::midi2freq(uint8_t note){
    return midi2freq(note,detune);
}


float BaseOscillator::get_sine_sample(){
    return std::sin(step + inst_phase) * inst_amp;
}

// TODO: change to pulse wave/add duty cycle?
float BaseOscillator::get_square_sample(){
    double p = std::fmod(step + inst_phase, 2.0 * M_PI);
    double s = (p < M_PI) ? 1.0 : -1.0;
    double t = p / (2.0 * M_PI);
    s += poly_blep(t); // first drop
    s -= poly_blep(fmod(t + 0.5, 1.0)); // second drop
    return s * inst_amp;

}

float BaseOscillator::get_saw_sample(){
    double p = std::fmod(step + inst_phase, 2.0 * M_PI);
    double t = p / (2.0 * M_PI);
    double s = 2.0 * t - 1.0;
    s -= poly_blep(t);
    return s * inst_amp;
}

float BaseOscillator::get_triangle_sample(){
    // get square wave
    double p = std::fmod(step + inst_phase, 2.0 * M_PI);
    double s = (p < M_PI) ? 1.0 : -1.0;
    double t = p / (2.0 * M_PI);
    s += poly_blep(t); // first drop
    s -= poly_blep(fmod(t + 0.5, 1.0)); // second drop

    // leaky integration
    s = step_increment * s + (1.0 - step_increment) * last_sample;
    last_sample = s;
    return s * inst_amp;
}

float BaseOscillator::get_noise_sample(){
    double s = distr(rnd);
    return s * inst_amp;
}

float BaseOscillator::get_sample(){
        float sample = 0.0f;
        switch(waveform){
        case WAVE_SINE:
            sample = get_sine_sample();
            break;
        case WAVE_TRIANGLE: 
            sample = get_triangle_sample();
            break;
        case WAVE_SQUARE:
            sample = get_square_sample();
            break;
        case WAVE_SAW:
            sample = get_saw_sample();
            break;
        case WAVE_NOISE:
            sample = get_noise_sample();
            break;
        default:
            sample = 0.0f;
            break;
        }
        
        // squish sample if necessary
        if (wave_range.first != -1.0 && wave_range.second != 1.0){
            sample = squish_sample(sample);
        }

        // apply gain if set
        if(gain) sample *= gain->get();

        return sample;
}

void BaseOscillator::tick(){
    step += step_increment;
}