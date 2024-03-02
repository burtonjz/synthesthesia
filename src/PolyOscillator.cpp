#include "commonTypes.hpp"

#include "PolyOscillator.hpp"
#include "ParameterController.hpp"
#include "ParameterType.hpp"
#include "LinearFader.hpp"

#include "ModulationParameter.hpp"
#include "ModulatorType.hpp"
#include "ModulationChain.hpp" // TODO: not a necessary header once this is encapsulated in modulation controller class.

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


PolyOscillator::PolyOscillator(const double* sampleRate):
    Module(ModuleType::PolyOscillator),
    keyboardController_(nullptr),
    modulationController_(nullptr),
    oscillator_()
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

std::pair<const ParameterType*, size_t> PolyOscillator::getControlPorts(){
    return { control_params_.data(), control_params_.size() };
}

void PolyOscillator::activate(KeyboardController* keyboardController, ModulationController* modulationController){
        keyboardController_ = keyboardController ;
        modulationController_ = modulationController ;
}

void PolyOscillator::setOutputBuffer(float* buffer, size_t channel){
    outputBuffer_.set(buffer,channel);
    for (auto& pair : oscillator_){
        pair.second.setOutputBuffer(buffer,channel);
    }
}

void PolyOscillator::processSample(uint32_t idx){
    for (auto& pair : oscillator_ ){
        pair.second.processSample(idx);
    }
}

void PolyOscillator::tick(){
    updateOscillators();

    parameterController_.modulate(); //TODO: make sure that the modulationController is updating the parent modulations (pan) when those values are changing
}

void PolyOscillator::updateOscillators(){
    const KeyboardMap* notes_ptr = keyboardController_->get_active_notes();
    // first, update oscillators with new information from active_notes
    for (const auto& pair : *notes_ptr ){
        auto it = oscillator_.find(pair.first);
        if(it == oscillator_.end()){
            createChildOscillator(pair.first,pair.second);
        } else {
            ParameterController* p = oscillator_[pair.first].getParameterController();
            p->setParameterValue<ParameterType::AMPLITUDE>(pair.second.getVelocity() / 127.0);
        }
    }

    // remove oscillators that are not active_notes
    for (auto it = oscillator_.begin(); it != oscillator_.end(); ){
        if ( (*notes_ptr).find(it->first) == (*notes_ptr).end()){
            it = oscillator_.erase(it);
        } else {
            oscillator_[it->first].tick();
            ++it ;
        }
    }
}

void PolyOscillator::createChildOscillator(uint8_t midi_note, const Note note){
    oscillator_.insert(std::make_pair(midi_note,Oscillator(sampleRate_,parameterController_)));
    ParameterController* params = oscillator_[midi_note].getParameterController();
    
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
    
    updateChildOutputBuffers(midi_note);
}

void PolyOscillator::updateChildOutputBuffers(uint8_t index){
    for(int i = 0; i < AudioPorts::AUDIO_N; ++i ){
        oscillator_[index].setOutputBuffer(outputBuffer_.get(i),i);
    }
}

