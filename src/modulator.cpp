#include "modulator.hpp"

Modulator::Modulator():
    is_active(false)
{}

Modulator::~Modulator(){
}

inline ModulatorType Modulator::getType() const {
    return MODULATOR_BASE;
}

inline bool Modulator::get_is_active() const {
    return is_active;
}

inline void Modulator::set_is_active(bool status){
    is_active = status;
}