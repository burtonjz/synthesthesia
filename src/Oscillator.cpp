#include "Oscillator.hpp"
#include "ParameterType.hpp"
#include "Waveform.hpp"
#include "Wavetable.hpp"

#include <array>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#endif

Oscillator::Oscillator(const double* sampleRate):
    Module(sampleRate),
    outputBuffer_(),
    phase_(0),
    increment_(0)
{    
    parameterController_.addParameter<ParameterType::STATUS>(true,false);
    parameterController_.addParameter<ParameterType::WAVEFORM>(parameterDefaults[static_cast<int>(ParameterType::WAVEFORM)],false);
    parameterController_.addParameter<ParameterType::AMPLITUDE>(1.0, true);
    parameterController_.addParameter<ParameterType::PHASE>(0.0, true);
    parameterController_.addParameter<ParameterType::PAN>(0.0, true);
    parameterController_.addParameter<ParameterType::DETUNE>(0.0, true);

    if(sampleRate_) parameterController_.addParameter<ParameterType::FREQUENCY>(440.0,true,0.0, *sampleRate_/2.0);
    else parameterController_.addParameter<ParameterType::FREQUENCY>(440.0,true);
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
    if(!parameterController_.getParameterValue<ParameterType::STATUS>()) return ;
    if(!isOutputBufferSet()) return ;

    const std::array<double,WAVETABLE_SIZE>* wave_ptr;
    double p ;
    double index ;
    int index_floor ;
    double frac ;
    double sample ;

    wave_ptr = Wavetable::getWavetable(static_cast<Waveform>(parameterController_.getParameterValue<ParameterType::WAVEFORM>()));

    // get linear interpolation of wavetable value
    p = std::fmod(phase_ + parameterController_.getParameterInstantaneousValue<ParameterType::PHASE>(),1.0);
    index = p * (WAVETABLE_SIZE - 1);
    index_floor = static_cast<int>(std::floor(index));
    frac = index - index_floor ;

    sample = (1.0 - frac) * (*wave_ptr)[index_floor] + frac * (*wave_ptr)[index_floor + 1];

    // apply amplitude
    sample *= parameterController_.getParameterInstantaneousValue<ParameterType::AMPLITUDE>();

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
    increment_ = parameterController_.getParameterInstantaneousValue<ParameterType::FREQUENCY>() / *sampleRate_ ;
    phase_ = std::fmod(phase_ + increment_, 1.0);
    parameterController_.modulateParameter<ParameterType::FREQUENCY>();
    parameterController_.modulateParameter<ParameterType::AMPLITUDE>();
    parameterController_.modulateParameter<ParameterType::PHASE>();
    parameterController_.modulateParameter<ParameterType::PAN>();
    parameterController_.modulateParameter<ParameterType::DETUNE>();
}

std::pair<double,double> Oscillator::panSample(double sample_value){
    double v = parameterController_.getParameterInstantaneousValue<ParameterType::PAN>() / 2.0 + 0.5 ;
    return std::make_pair(v * sample_value, (1.0 - v) * sample_value );
}