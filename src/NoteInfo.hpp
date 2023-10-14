#ifndef __NOTE_INFO_HPP_
#define __NOTE_INFO_HPP_

#include <cstdint>

/**
 * @brief basic struct storing note information required for external Modules.
 * 
 * This is different than a Note because the KeyboardController provides additional values (e.g., sustain, handles pitchbend into the frequency, etc.)
*/
struct NoteInfo {
    bool pressed ;
    double frequency ;
    uint8_t velocity ;
    uint8_t sustain ;
    float time_since_pressed ;
    float time_since_released ;
};

#endif // __NOTE_INFO_HPP_