#include "config.hpp"
#include "Oscillator.hpp"
#include "ParameterType.hpp"
#include "Waveform.hpp"
#include "Wavetable.hpp"

#include <array>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <algorithm>

Oscillator::Oscillator(const double* sampleRate):
    Module(sampleRate),
    outputBuffer_(),
    phase_(0),
    increment_(0)
{
    parameterController_.addParameter<bool>(ParameterType::STATUS,true,false);
    parameterController_.addParameter<int>(ParameterType::WAVEFORM,static_cast<int>(Waveform::SQUARE),false);    
    parameterController_.addParameter<double>(ParameterType::AMPLITUDE, 1.0, true);
    parameterController_.addParameter<double>(ParameterType::PHASE, 0.0, true);
    parameterController_.addParameter<double>(ParameterType::PAN, 0.0, true);
    parameterController_.addParameter<double>(ParameterType::DETUNE, 0.0, true);

    if(sampleRate_) parameterController_.addParameter<double>(ParameterType::FREQUENCY,440.0,true,0.0, *sampleRate_/2.0);
    else parameterController_.addParameter<double>(ParameterType::FREQUENCY, 440.0,true);
}

Oscillator::Oscillator():
    Oscillator(nullptr)
{}

void Oscillator::activate(){
}

void Oscillator::setOutputBuffer(float* buffer, size_t channel){
    outputBuffer_.set(buffer,channel);
}

bool Oscillator::isOutputBufferSet(){
    for (size_t i = 0; i < AUDIO_OUT_N; ++i ){
        if(outputBuffer_.get(i) == nullptr) return false ;
    }
    return true ;
}

void Oscillator::processSample(uint32_t idx){
    // only process sample if output buffer exists and module has status
    if(!parameterController_.getParameterValue<bool>(ParameterType::STATUS)) return ;
    if(!isOutputBufferSet()) return ;

    const std::array<double,WAVETABLE_SIZE>* wave_ptr;
    double p ;
    double index ;
    int index_floor ;
    double frac ;
    double sample ;

    wave_ptr = Wavetable::getWavetable(static_cast<Waveform>(parameterController_.getParameterValue<int>(ParameterType::WAVEFORM)));

    // get linear interpolation of wavetable value
    p = std::fmod(phase_ + parameterController_.getParameterValue<double>(ParameterType::PHASE),1.0);
    index = p * (WAVETABLE_SIZE - 1);
    index_floor = static_cast<int>(std::floor(index));
    frac = index - index_floor ;

    sample = (1.0 - frac) * (*wave_ptr)[index_floor] + frac * (*wave_ptr)[index_floor + 1];

    // add sample to buffer
    if(AUDIO_OUT_N == 2){
        std::pair<double,double> panned_sample = panSample(sample);
        outputBuffer_.addToBuffer(panned_sample.first,idx,0);
        outputBuffer_.addToBuffer(panned_sample.second,idx,1);
    } else {
        outputBuffer_.addToBuffer(sample,idx,0);
    }

}

void Oscillator::tick(){
    increment_ = parameterController_.getParameterValue<double>(ParameterType::FREQUENCY) / *sampleRate_ ;
    phase_ = std::fmod(phase_ + increment_, 1.0);
}

std::pair<double,double> Oscillator::panSample(double sample_value){
    double v = sample_value / 2.0 + 0.5 ;
    return std::make_pair(v, 1.0 - v);
}