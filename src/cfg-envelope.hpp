// ARGUMENTS REQUIRED TO CREATE AN ENVELOPE

#ifndef ENVELOPE_CFG_HPP_
#define ENVELOPE_CFG_HPP_

#include "adsr.hpp"
#include <cstdint>

struct EnvelopeConfig {
    ADSR adsr;
    uint16_t connections;
};


#endif // ENVELOPE_CFG_HPP_