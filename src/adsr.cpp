#include "adsr.hpp"

float ADSR::get_level(KeyStatus status, double time, float start_level){
    switch(status){
    case KEY_PRESSED:
        if (time < attack){
            return start_level + (1.0f - start_level) * time / attack;
        }

        if (time < attack + decay){
            return 1.0f - (1.0f - sustain) * (time - attack) / decay;
        }

        return sustain;

    case KEY_RELEASED:
        return start_level - start_level * time / release ;

    default:
        return 0.0f;
        
    }
}