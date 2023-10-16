#include "KeyboardController.hpp"
#include "ParameterType.hpp"

#include <cmath>
#include <algorithm>
#include <iostream>

KeyboardController::KeyboardController():
    notes_(),
    pitchbend_scale_factor_(1.0f),
    sustain_(0.0f)
{}

void KeyboardController::pressNote(uint8_t midi_note, float velocity){
    if(notes_[midi_note].getNote() != midi_note ) notes_[midi_note].setNote(midi_note);
    notes_[midi_note].setIsPressed(true);
    notes_[midi_note].setVelocity(velocity);
}

void KeyboardController::releaseNote(uint8_t midi_note){
    if ( midi_note > 127 ) midi_note = 127 ;

    notes_[midi_note].setIsPressed(false);
}

float KeyboardController::getPitchbend() const {
    return pitchbend_scale_factor_ ;
}

void KeyboardController::setPitchbend(uint16_t pitchbend ){
    if ( pitchbend > 16383 ) pitchbend = 16383 ;

    // linear transformation to make value in  [-1.0f, 1.0f] * MAX PITCHBEND SHIFT
    float shiftValue = ( (pitchbend - 8192.0f) / 16383.0f ) * CONFIG_PITCHBEND_MAX_SHIFT ;

    pitchbend_scale_factor_ = std::pow(2.0f, (shiftValue / 12.0f));
    // TODO: precompute this
}

void KeyboardController::setSustain(uint8_t sustain ){
    if ( sustain > 127 ) sustain_ = 127 ;
    else sustain_ = sustain ;
}
    
const NoteInfo KeyboardController::get_active_notes(){
    NoteInfo n ;

    n.notes_ptr = &notes_ ;
    n.pitchbend_scale_factor = pitchbend_scale_factor_ ;
    n.sustain = sustain_ ;

    return n ;
}

void KeyboardController::processMidi(LV2_Midi_Message_Type message_type, const uint8_t* const msg){
    switch(message_type){
    case LV2_MIDI_MSG_NOTE_ON:
        pressNote(msg[1],msg[2]);
        break;
    case LV2_MIDI_MSG_NOTE_OFF:
        releaseNote(msg[1]);
        break;
    case LV2_MIDI_MSG_BENDER:
        setPitchbend(static_cast<uint16_t>((msg[2] << 7) | msg[1]));
        break;
    case LV2_MIDI_MSG_CONTROLLER:
        // TODO: figure out if we want to mess with any other controller messages
        if ( msg[1] == LV2_MIDI_CTL_SUSTAIN ) setSustain(msg[2]);
        break;
    default:
        break;
    }
}

void KeyboardController::tick(double time){
    for (auto it = notes_.begin(); it != notes_.end(); ){
        it->second.tick(time);
        if (
            !it->second.getIsPressed() &&
            it->second.getTimeSinceReleased() > parameterLimits[static_cast<int>(ParameterType::RELEASE)].second
        ){
            it = notes_.erase(it) ;
        } else {
            ++it ;
        }
    }
}