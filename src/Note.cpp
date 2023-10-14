#include "Note.hpp"

#include <cmath>
#include <iostream>

Note::Note(uint8_t midi_note, uint8_t midi_velocity, bool is_note_pressed, float time_since_pressed):
    is_note_pressed_(is_note_pressed),
    time_since_pressed_(time_since_pressed),
    time_since_released_(0.0f)
{
    setNote(midi_note);
    setFrequency(midi_note_);
    setVelocity(midi_velocity);
}

Note::Note(uint8_t midi_note, uint8_t midi_velocity):
    Note(midi_note,midi_velocity,false,0.0f)
{}

Note::Note():
    Note(0,0)
{}

void Note::setFrequency(uint8_t midi_note){
    frequency_ = pow(2.0, (static_cast<float>(midi_note) - 69.0) / 12.0) * 440.0 ; // TODO: support tunings other than equal temperment
}

uint8_t Note::getNote() const {
    return midi_note_ ;
}

void Note::setNote(uint8_t note){
    if ( note > 127 ) note = 127 ;
    else midi_note_ = note ;
    setFrequency(midi_note_);

    std::cout << "[Note] midi_note " << std::to_string(static_cast<int>(note)) << " frequency set to " << frequency_ << std::endl ;
}

uint8_t Note::getVelocity() const {
    return midi_velocity_ ;
}

void Note::setVelocity(uint8_t midi_velocity){
    if ( midi_velocity > 127 ) midi_velocity_ = 127 ;
    else midi_velocity_ = midi_velocity ;
}

bool Note::getIsPressed() const {
    return is_note_pressed_ ;
}

void Note::setIsPressed(bool is_note_pressed){
    if ( is_note_pressed_ != is_note_pressed ){
        is_note_pressed_ = is_note_pressed ;
        resetTimeSinceEvent();
    }
    
    is_note_pressed_ = is_note_pressed ;
}

float Note::getFrequency() const {
    return frequency_ ;
}

float Note::getTimeSincePressed() const {
    return time_since_pressed_ ;
}

float Note::getTimeSinceReleased() const {
    return time_since_released_ ;
}

void Note::resetTimeSinceEvent(){
    // std::cout << "[Note] midi_note=" << midi_note_ << ", status=" << is_note_pressed_ << ", time_since_pressed=" << time_since_pressed_ << ", time_since_released=" << time_since_released_ << std::endl;
    if(is_note_pressed_) time_since_pressed_ = 0.0f;
    else time_since_released_ = 0.0f;
}

void Note::tick(double time){
    if(is_note_pressed_) time_since_pressed_ += time ;
    else time_since_released_ += time ;
}