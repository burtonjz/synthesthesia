#include "PolyOscillator.hpp"
#include "Note.hpp"
#include "ParameterController.hpp"
#include "KeyboardController.hpp"
#include "ParameterType.hpp"
#include "LinearFader.hpp"
#include "ADSREnvelope.hpp"
#include "ModulationParameter.hpp"

#include <cstdint>

#ifdef DEBUG
#include <iostream>
#endif

// define static variables
boost::container::vector<ParameterType> PolyOscillator::control_params_ ;


PolyOscillator::PolyOscillator(const double* sampleRate):
    Module(sampleRate),
    oscillator_()
{
    parameterController_.addParameter<ParameterType::STATUS>(true,false);
    parameterController_.addParameter<ParameterType::WAVEFORM>(parameterDefaults[static_cast<int>(ParameterType::WAVEFORM)],false);
    parameterController_.addParameter<ParameterType::AMPLITUDE>(1.0, true);
    parameterController_.addParameter<ParameterType::PHASE>(0.0, true);
    parameterController_.addParameter<ParameterType::PAN>(0.0, true);
    parameterController_.addParameter<ParameterType::DETUNE>(0.0, true);
}

uint32_t PolyOscillator::getNumControlPorts(){
    return control_params_.size() ;
}

const boost::container::vector<ParameterType>* PolyOscillator::getControlPorts(){
    return &control_params_ ;
}

void PolyOscillator::static_activate(){
    control_params_.push_back(ParameterType::STATUS);
    control_params_.push_back(ParameterType::WAVEFORM);
    control_params_.push_back(ParameterType::AMPLITUDE);
    control_params_.push_back(ParameterType::PHASE);
    control_params_.push_back(ParameterType::PAN);
    control_params_.push_back(ParameterType::DETUNE);
}

void PolyOscillator::activate(){    
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
}

void PolyOscillator::updateOscillators(){
    const boost::container::flat_map<uint8_t,Note>* notes_ptr = KeyboardController::get_active_notes();
    // first, update oscillators with new information from active_notes
    for (const auto& pair : *notes_ptr ){
        auto it = oscillator_.find(pair.first);
        if(it == oscillator_.end()){
            oscillator_.insert(std::make_pair(pair.first,Oscillator(sampleRate_)));
            ParameterController* p = oscillator_[pair.first].getParameterController();
            
            p->setParameterValue<ParameterType::STATUS>(true);
            p->setParameterValue<ParameterType::FREQUENCY>(pair.second.getFrequency() * KeyboardController::getPitchbend());
            p->setParameterValue<ParameterType::AMPLITUDE>(pair.second.getVelocity() / 127.0);

            boost::container::flat_map<ModulationParameter,double> amp_map ;
            amp_map[ModulationParameter::MIDI_NOTE] = pair.first ;
            amp_map[ModulationParameter::INITIAL_VALUE] = 0.0 ;
            amp_map[ModulationParameter::LAST_VALUE] = 0.0 ;
            
            p->setParameterModulation<ParameterType::AMPLITUDE>( 
                ADSREnvelope::modulate, 
                amp_map
            );

            updateChildOutputBuffers(pair.first);

        } else {
            ParameterController* p = oscillator_[pair.first].getParameterController();

            p->setParameterValue<ParameterType::FREQUENCY>(pair.second.getFrequency() * KeyboardController::getPitchbend());
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

void PolyOscillator::updateChildOutputBuffers(uint8_t index){
    for(int i = 0; i < AudioPorts::AUDIO_N; ++i ){
        oscillator_[index].setOutputBuffer(outputBuffer_.get(i),i);
    }
}


#ifdef DEBUG
#include "Wavetable.hpp"
#include "MidiNote.hpp"
#include <iostream>
// gcc -DDEBUG -std=c++17 -o test ADSREnvelope.cpp MidiNote.cpp Wavetable.cpp Oscillator.cpp Note.cpp KeyboardController.cpp PolyOscillator.cpp -I/usr/include/lv2 -L/usr/lib/lv2 -lboost_container -lm -lstdc++
int main() {
    KeyboardController::generate();
    MidiNote::generate();
    ADSREnvelope::activate();

    double sample_rate = 10 ;
    PolyOscillator p(&sample_rate);
    
    int n_samples = 100 ;
    uint8_t midi_msg[3] = {0, 69, 80} ;

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