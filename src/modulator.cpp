#include "modulator.hpp"

Modulator::Modulator(){}

Modulator::~Modulator(){}

ModulatorType Modulator::getType() const {
    return MODULATOR_NONE;
}

bool Modulator::get_is_active() const {
    return connections.has_active_connections();
}

int Modulator::find_connection(ModulatableType module, int instance, int port) const {
    return connections.find_connection(module,instance,port);
}

void Modulator::set_connections(float d){
    connections.set_data_from_float(d);
}

float Modulator::get_connections_as_float(){
    return connections.encode_as_float();
}