#include "Note.hpp"

#include <cmath>
#include <iostream>

Note::Note(uint8_t midi_note, uint8_t midi_velocity, bool is_note_pressed,double time_since_event):
    is_note_pressed_(is_note_pressed),
    time_since_event_(time_since_event)
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
    frequency_ = pow(2.0, (static_cast<float>(midi_note) - 69.0) / 12.0) * 440.0 ;
}

uint8_t Note::getNote() const {
    return midi_note_ ;
}

void Note::setNote(uint8_t note){
    if ( note > 127 ) note = 127 ;
    else midi_note_ = note ;
    setFrequency(midi_note_);
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

float Note::getTimeSinceEvent() const {
    return time_since_event_ ;
}

void Note::resetTimeSinceEvent(){
    std::cout << "Note Time Reset from " << time_since_event_ << std::endl;
    time_since_event_ = 0.0f ;
}

void Note::tick(double time){
    time_since_event_ += time ;
}