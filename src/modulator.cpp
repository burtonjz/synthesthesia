#include "modulator.hpp"

Modulator::Modulator():
    is_active(false)
{}

Modulator::~Modulator(){
}

ModulatorType Modulator::getType() const {
    return MODULATOR_BASE;
}

bool Modulator::get_is_active() const {
    return is_active;
}

void Modulator::set_is_active(bool status){
    is_active = status;
}