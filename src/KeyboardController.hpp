#ifndef __KEYBOARD_CONTROLLER_HPP_
#define __KEYBOARD_CONTROLLER_HPP_

#include "config.hpp"
#include "Note.hpp"
#include "NoteInfo.hpp"

#include <lv2/midi/midi.h>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/vector.hpp>
#include <cstdint>


/**
 * @brief Feature class to handle midi information
 * 
 * Handle midi inputs. This class will keep track of what midi notes have been pressed, if they have been released, 
 * and how long it has been since they were released.
*/
class KeyboardController {
private:
    boost::container::flat_map<uint8_t, Note> notes_ ;
    boost::container::flat_set<uint8_t> active_note_ids_ ;

    float pitchbend_frequency_scale_factor_ ;
    float sustain_ ;

    void pressNote(uint8_t midi_note, float velocity);
    void releaseNote(uint8_t midi_note);
    float getPitchbend() const ;
    void setPitchbend(uint16_t pitchBendValue );
    void setSustain(uint8_t sustain );

public:
    KeyboardController();

    /**
     * @brief get all notes currently pressed or released within the RELEASE parameter type upper limit
    */
    boost::container::vector<NoteInfo> get_active_notes();

    /**
     * @brief process midi message
     * 
     * @param message_type LV2 Midi message type
     * @param msg The message block of the LV2 Atom
    */
    void processMidi(LV2_Midi_Message_Type message_type, const uint8_t* const msg);

    /**
     * @brief tick all active components
    */
    void tick(double time);
};


#endif // __KEYBOARD_CONTROLLER_HPP_