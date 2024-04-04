#include "PolyOscillator.hpp"
#include "ParameterController.hpp"
#include "ParameterType.hpp"

#include "BMap.hpp"

#include <cstdint>
#include <utility>

#ifdef DEBUG
#include <iostream>
#endif

// define static variables
std::array<ParameterType, 9> PolyOscillator::control_params_ = {
    ParameterType::STATUS,
    ParameterType::WAVEFORM,
    ParameterType::GAIN,
    ParameterType::DETUNE,
    ParameterType::PAN,
    ParameterType::MC_AMPLITUDE,
    ParameterType::MC_FREQUENCY,
    ParameterType::MC_PAN,
    ParameterType::MC_PHASE
};

std::array<ParameterType, 1> PolyOscillator::modulatable_params_ = {
    ParameterType::PAN,
};

std::pair<const ParameterType*, size_t> PolyOscillator::getControlPorts(){
    return { control_params_.data(), control_params_.size() };
}

std::pair<const ParameterType*, size_t> PolyOscillator::getModulatableParameters(){
    return { modulatable_params_.data(), modulatable_params_.size() } ;
}

PolyOscillator::PolyOscillator(const double* sampleRate):
    MidiModule(ModuleType::PolyOscillator),
    modulationController_(nullptr)
{
    parameterController_.addParameter<ParameterType::STATUS>(true,false);
    parameterController_.addParameter<ParameterType::WAVEFORM>(parameterDefaults[static_cast<int>(ParameterType::WAVEFORM)],false);
    parameterController_.addParameter<ParameterType::GAIN>(1.0, false);
    parameterController_.addParameter<ParameterType::DETUNE>(0.0, false);
    parameterController_.addParameter<ParameterType::PAN>(0.0, true);
    parameterController_.addParameter<ParameterType::MC_AMPLITUDE>(0,false);
    parameterController_.addParameter<ParameterType::MC_FREQUENCY>(0,false);
    parameterController_.addParameter<ParameterType::MC_PAN>(0,false);
    parameterController_.addParameter<ParameterType::MC_PHASE>(0,false);

    setSampleRate(sampleRate);
}

void PolyOscillator::activate(KeyboardController* keyboardController, ModulationController* modulationController){
        MidiModule::activate(keyboardController);
        modulationController_ = modulationController ;
}

void PolyOscillator::setOutputBuffer(float* buffer, size_t channel){
    outputBuffer_.set(buffer,channel);
    // for (auto& pair : getChildren()){
    //     pair.second.setOutputBuffer(buffer,channel);
    // }
}

void PolyOscillator::processSample(uint32_t idx){
    for (auto& pair : children_ ){
        pair.second.processSample(idx);
    }
}

void PolyOscillator::tick(){
    MidiModule::tick();
}

void PolyOscillator::createChild(uint8_t midi_note, const Note note){
    children_.insert(std::make_pair(midi_note,Oscillator(sampleRate_,parameterController_)));
    ParameterController* params = children_[midi_note].getParameterController();
    
    // set note-specific parameters
    params->setParameterValue<ParameterType::FREQUENCY>(note.getFrequency());
    params->setParameterValue<ParameterType::AMPLITUDE>(note.getVelocity() / 127.0);

    // set modulation
    modulationController_->setModulation(
        getType(),
        getInstance(),
        params,
        ParameterType::FREQUENCY,
        parameterController_.getParameterValue<ParameterType::MC_FREQUENCY>(),
        midi_note
    ); 
    modulationController_->setModulation(
        getType(),
        getInstance(),
        params,
        ParameterType::AMPLITUDE,
        parameterController_.getParameterValue<ParameterType::MC_AMPLITUDE>(),
        midi_note
    ); 
    // modulationController_->setModulation(
    //     getType(),
    //     getInstance(),
    //     params,
    //     ParameterType::PHASE,
    //     parameterController_.getParameterValue<ParameterType::MC_PHASE>(),
    //     midi_note
    // ); 
    
    for(int i = 0; i < AudioPorts::AUDIO_N; ++i ){
        children_[midi_note].setOutputBuffer(outputBuffer_.get(i),i);
    }
}

void PolyOscillator::repressChild(uint8_t midi_note, const Note note){
    ParameterController* p = children_[midi_note].getParameterController();
    p->setParameterValue<ParameterType::AMPLITUDE>(note.getVelocity() / 127.0);
}



