#include "PolyOscillator.hpp"
#include "NoteInfo.hpp"
#include "ParameterController.hpp"
#include "ParameterType.hpp"

#include <boost/container/vector.hpp>

PolyOscillator::PolyOscillator(const double* sampleRate):
    Module(sampleRate),
    oscillator_(),
    keyboard_controller_ptr_(nullptr)
{}

void PolyOscillator::activate(){
}

void PolyOscillator::activate(KeyboardController* controller){
    keyboard_controller_ptr_ = controller ;
    activate();
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
    if(keyboard_controller_ptr_){
        updateOscillators(keyboard_controller_ptr_->get_active_notes());
    }
}

#include <iostream>

void PolyOscillator::updateOscillators(const boost::container::flat_map<uint8_t,NoteInfo> active_notes){
    // first, update oscillators with new information from active_notes
    for (const auto& pair : active_notes ){
        auto it = oscillator_.find(pair.first);
        if(it == oscillator_.end()){
            oscillator_.insert(std::make_pair(pair.first,Oscillator(sampleRate_)));
            ParameterController* p = oscillator_[pair.first].getParameterController();
            
            p->setParameterValue<bool>(ParameterType::STATUS,true);
            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.frequency);
            p->setParameterValue<double>(ParameterType::AMPLITUDE,pair.second.velocity / 127.0);

            std::cout << "[PolyOscillator] midi_note=" << std::to_string(static_cast<int>(pair.first)) 
                << ", frequency=" << p->getParameterValue<double>(ParameterType::FREQUENCY) 
                << ", amplitude=" << p->getParameterValue<double>(ParameterType::AMPLITUDE)
                << std::endl ;

            updateChildOutputBuffers(pair.first);

        } else {
            ParameterController* p = oscillator_[pair.first].getParameterController();

            p->setParameterValue<double>(ParameterType::FREQUENCY,pair.second.frequency);

        }
    }

    // remove oscillators that are not active_notes
    for (auto it = oscillator_.begin(); it != oscillator_.end(); ){
        if (active_notes.find(it->first) == active_notes.end()){
            it = oscillator_.erase(it);
        } else {
            oscillator_[it->first].tick();
            ++it ;
        }
    }

}

void PolyOscillator::updateChildOutputBuffers(uint8_t index){
    for(int i = 0; i < AUDIO_OUT_N; ++i ){
        oscillator_[index].setOutputBuffer(outputBuffer_.get(i),i);
    }
}