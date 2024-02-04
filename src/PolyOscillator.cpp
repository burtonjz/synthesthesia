#include "commonTypes.hpp"

#include "PolyOscillator.hpp"
#include "ParameterController.hpp"
#include "ParameterType.hpp"
#include "LinearFader.hpp"

#include "ModulationParameter.hpp"
#include "ModulatorType.hpp"

#include "BMap.hpp"
#include <cstdint>

#ifdef DEBUG
#include <iostream>
#endif

// define static variables
std::array<ParameterType, 5> PolyOscillator::control_params_ = {
    ParameterType::STATUS,
    ParameterType::WAVEFORM,
    ParameterType::GAIN,
    ParameterType::DETUNE,
    ParameterType::PAN
};


PolyOscillator::PolyOscillator(const double* sampleRate):
    Module(sampleRate),
    keyboardController_(nullptr),
    oscillator_(),
    detuner_(nullptr),
    envelope_(nullptr)
{
    parameterController_.addParameter<ParameterType::STATUS>(true,false);
    parameterController_.addParameter<ParameterType::WAVEFORM>(parameterDefaults[static_cast<int>(ParameterType::WAVEFORM)],false);
    parameterController_.addParameter<ParameterType::GAIN>(1.0, false);
    parameterController_.addParameter<ParameterType::DETUNE>(0.0, true);
    parameterController_.addParameter<ParameterType::PAN>(0.0, true);

}

std::pair<const ParameterType*, size_t> PolyOscillator::getControlPorts(){
    return { control_params_.data(), control_params_.size() };
}

void PolyOscillator::activate(KeyboardController* keyboardController, ADSREnvelope* envelope){
        keyboardController_ = keyboardController ;
        detuner_.activate(keyboardController);
        envelope_ = envelope ;
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

    // modulate parameters
    parameterController_.modulateParameter<ParameterType::PAN>();
    parameterController_.modulateParameter<ParameterType::DETUNE>();

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

    // set modulation TODO: make modulation controller, do this all fancy?
    setModulation(params, ParameterType::AMPLITUDE, envelope_, midi_note);
    setModulation(params, ParameterType::FREQUENCY, &detuner_, midi_note);
    
    updateChildOutputBuffers(midi_note);
}

void PolyOscillator::updateChildOutputBuffers(uint8_t index){
    for(int i = 0; i < AudioPorts::AUDIO_N; ++i ){
        oscillator_[index].setOutputBuffer(outputBuffer_.get(i),i);
    }
}

void PolyOscillator::setModulation(ParameterController* params, ParameterType p, Modulator* mod_ptr, uint32_t midi_note){
    if( !mod_ptr ) return ;
    if ( !params->parameterExists(p) ) return ;

    ParameterModMap map ;
    switch(mod_ptr->getType()){
    case ModulatorType::ADSREnvelope:
        map[ModulationParameter::MIDI_NOTE] = midi_note ;
        map[ModulationParameter::INITIAL_VALUE] = 0.0 ;
        map[ModulationParameter::LAST_VALUE] = 0.0 ;
        break ;
    case ModulatorType::Detuner:
        map[ModulationParameter::DETUNE_CENTS] = params->getParameterInstantaneousValue<ParameterType::DETUNE>() ;
        break ;
    case ModulatorType::LinearFader:
        map[ModulationParameter::MIDI_NOTE] = midi_note ;
        break ;
    case ModulatorType::None:
    default:
        break ;
    }

    switch(p){
    case ParameterType::AMPLITUDE:
        params->setParameterModulation<ParameterType::AMPLITUDE>(mod_ptr, map);
        return ;
    case ParameterType::FREQUENCY:
        params->setParameterModulation<ParameterType::FREQUENCY>(mod_ptr, map);
        return ;
    case ParameterType::PHASE:
        params->setParameterModulation<ParameterType::PHASE>(mod_ptr, map);
        return ;
    default:
        return ;

    }

    
}

#ifdef DEBUG
#include "Wavetable.hpp"
#include "MidiNote.hpp"
#include <iostream>
// gcc -DDEBUG -std=c++17 -o test ADSREnvelope.cpp MidiNote.cpp Wavetable.cpp Oscillator.cpp Note.cpp KeyboardController.cpp PolyOscillator.cpp -I/usr/include/lv2 -L/usr/lib/lv2 -lboost_container -lm -lstdc++
int main() {
    double sample_rate = 10 ;
    int n_samples = 100 ;
    uint8_t midi_msg[3] = {0, 69, 80} ;

    KeyboardController::generate();
    MidiNote::generate();
    ADSREnvelope::activate(&sample_rate);

    PolyOscillator p(&sample_rate);
    

    float audio_buffer_L[n_samples] ;
    float audio_buffer_R[n_samples] ;

    for(int i = 0; i < n_samples; ++i){
        audio_buffer_L[i] = 0.0f ;
        audio_buffer_R[i] = 0.0f ;
    }

    // activate stuff
    Wavetable::generate() ; 

    p.activate();
    p.setOutputBuffer(audio_buffer_L, 0);
    p.setOutputBuffer(audio_buffer_R, 1);

    // give keyboard controller a midi message
    KeyboardController::processMidi(LV2_MIDI_MSG_NOTE_ON, midi_msg);
    // p.tick();

    for(int i = 0; i < 60; ++i ){
        std::cout << std::to_string(i) ;
        p.processSample(i);
        p.tick();
        KeyboardController::tick(1.0/sample_rate);
        std::cout
            << ", audio_out=" << std::to_string(audio_buffer_L[i]) 
            << std::endl ;
    }

    // release note and refresh buffer
    KeyboardController::processMidi(LV2_MIDI_MSG_NOTE_OFF, midi_msg);
    for(int i = 0; i < n_samples; ++i){
        audio_buffer_L[i] = 0.0f ;
        audio_buffer_R[i] = 0.0f ;
    }

    for(int i = 0; i < 15; ++i ){
        std::cout << std::to_string(i) ;
        p.processSample(i);
        p.tick();
        KeyboardController::tick(1.0/sample_rate);
        std::cout
            << ", audio_out=" << std::to_string(audio_buffer_L[i]) 
            << std::endl ;
    }

    // press note again & refresh buffer
    KeyboardController::processMidi(LV2_MIDI_MSG_NOTE_ON, midi_msg);
    for(int i = 0; i < n_samples; ++i){
        audio_buffer_L[i] = 0.0f ;
        audio_buffer_R[i] = 0.0f ;
    }

    for(int i = 0; i < 15; ++i ){
        std::cout << std::to_string(i) ;
        p.processSample(i);
        p.tick();
        KeyboardController::tick(1.0/sample_rate);
        std::cout
            << ", audio_out=" << std::to_string(audio_buffer_L[i]) 
            << std::endl ;
    }



    return 0 ;

}
#endif