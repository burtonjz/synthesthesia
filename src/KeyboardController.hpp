#ifndef __KEYBOARD_CONTROLLER_HPP_
#define __KEYBOARD_CONTROLLER_HPP_

#include "config.hpp"
#include "Note.hpp"
#include "Parameter.hpp"
#include "ModulationParameter.hpp"
#include "BMap.hpp"

#include <lv2/midi/midi.h>
#include <cstdint>
#include <array>

using KeyboardMap = BMap<uint8_t, Note, 128> ;

/**
 * @brief Feature class to handle midi information
 * 
 * Handle midi inputs. This class will keep track of what midi notes have been pressed, if they have been released, 
 * and how long it has been since they were released.
*/
class KeyboardController {
private:
    static KeyboardMap notes_ ;
    static uint16_t pitchbend_value_ ;
    static std::array<double,16384> pitchbend_scale_factor_ ;
    static uint8_t sustain_ ;

public:

    /**
     * @brief pre-compute values used in KeyboardController processing
     * 
     * Precomputed values: pitchbend scale factor
    */
    static void generate();

    /**
     * @brief get all notes currently pressed or released within the RELEASE parameter type upper limit
     * 
     * the vector will contain all active notes by order of longest to shortest total time since pressed
    */
    static const KeyboardMap* get_active_notes();

    /**
     * @brief process midi message
     * 
     * @param message_type LV2 Midi message type
     * @param msg The message block of the LV2 Atom
    */
    static void processMidi(LV2_Midi_Message_Type message_type, const uint8_t* const msg);

    /**
     * @brief tick all active components
    */
    static void tick(double time);

    /**
     * @brief get the pitchbend scale factor
    */
    static float getPitchbend() ;

    /**
     * @brief get the sustain value (between 0-127)
    */
    static uint8_t getSustain() ;

    /**
     * @brief modulation function for applying pitchbend
     * 
     * @param value value to modulate (the Parameter value)
     * @param modp ModulationParameter map.
    */
    static double pitchbendModulation(double value, ParameterModMap* modp);

private:
    static void pressNote(uint8_t midi_note, float velocity);
    static void releaseNote(uint8_t midi_note);
    static void setPitchbend(uint16_t pitchBendValue );
    static void setSustain(uint8_t sustain );

    static void computePitchbendScaleFactor();

};


#endif // __KEYBOARD_CONTROLLER_HPP_