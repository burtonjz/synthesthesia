#ifndef ADSR_HPP_
#define ADSR_HPP_

#include "key-status.hpp"

struct ADSR{
    double attack;
    double decay;
    float sustain;
    double release;
    float get_level(KeyStatus status, double time, float start_level);
};

#endif //ADSR_HPP_