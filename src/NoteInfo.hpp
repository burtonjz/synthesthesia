#ifndef __NOTE_INFO_HPP_
#define __NOTE_INFO_HPP_

#include <cstdint>

/**
 * @brief basic struct storing note information required for external Modules
*/
struct NoteInfo {
    float frequency ;
    uint8_t velocity ;
    uint8_t sustain ;
   double time_since_event ;
};

#endif // __NOTE_INFO_HPP_