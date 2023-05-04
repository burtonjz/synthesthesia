#include "modulator.hpp"

Modulator::Modulator():
    is_active(false)
{}
Modulator::Modulator(bool is_active_):
    is_active(is_active_)
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

uint16_t Modulator::get_connections() const {
    return connections;
}

void Modulator::set_connections(uint16_t c){
    connections = c;
    set_is_active(connections != MOD_CONNECT_NONE);
}