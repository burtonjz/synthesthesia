#ifndef __NOTE_INFO_HPP_
#define __NOTE_INFO_HPP_

#include "Note.hpp"

#include <cstdint>
#include <boost/container/flat_map.hpp>

/**
 * @brief basic struct storing note information required for external Modules.
 * 
 * This is different than a Note because the KeyboardController provides additional values (e.g., sustain, handles pitchbend into the frequency, etc.)
*/
struct NoteInfo {
    boost::container::flat_map<uint8_t,Note>* notes_ptr ;
    uint8_t sustain ;
    float pitchbend_scale_factor ;
};

#endif // __NOTE_INFO_HPP_