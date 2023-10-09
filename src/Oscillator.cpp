#include "config.hpp"
#include "Oscillator.hpp"
#include "ParameterType.hpp"
#include "Waveform.hpp"
#include "Wavetable.hpp"

#include <array>
#include <cstdint>
#include <cmath>
#include <iostream>

Oscillator::Oscillator(const double* sampleRate):
    Module(sampleRate),
    outputBuffer_(),
    phase_(0),
    increment_(0)
{}

void Oscillator::activate(){
    parameterController_.addParameter<bool>(ParameterType::STATUS,true,false);
    parameterController_.addParameter<int>(ParameterType::WAVEFORM,static_cast<int>(Waveform::SQUARE),false);
    parameterController_.addParameter<double>(ParameterType::FREQUENCY,440.0,true,0.0, *sampleRate_/2.0);
    parameterController_.addParameter<double>(ParameterType::AMPLITUDE, 1.0, true);
    parameterController_.addParameter<double>(ParameterType::PHASE, 0.0, true);
    parameterController_.addParameter<double>(ParameterType::PAN, 0.0, true);
    parameterController_.addParameter<double>(ParameterType::DETUNE, 0.0, true);
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
    if(!parameterController_.getParameterValue<bool>(ParameterType::STATUS)) return ;
    if(!isOutputBufferSet()) return ;

    const std::array<double,WAVETABLE_SIZE>* wave_ptr;

    double p ;
    double index ;
    int index_floor ;
    double frac ;
    double sample ;

    wave_ptr = Wavetable::getWavetable(static_cast<Waveform>(parameterController_.getParameterValue<int>(ParameterType::WAVEFORM)));

    p = std::fmod(phase_ + parameterController_.getParameterValue<double>(ParameterType::PHASE),1.0);

    index = p * (WAVETABLE_SIZE - 1);
    index_floor = static_cast<int>(std::floor(index));
    frac = index - index_floor ;

    sample = (1.0 - frac) * (*wave_ptr)[index_floor] + frac * (*wave_ptr)[index_floor + 1];
    outputBuffer_.addToBuffer(sample,idx,0);

}

void Oscillator::tick(){
    increment_ = parameterController_.getParameterValue<double>(ParameterType::FREQUENCY) / *sampleRate_ ;
    phase_ = std::fmod(phase_ + increment_, 1.0);
}