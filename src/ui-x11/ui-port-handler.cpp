#include "ui-port-handler.hpp"

bool PortHandler::port_in_range(const uint32_t port_index, const std::pair<int,int> range){
    return (port_index >= range.first && port_index <= range.second);
}

std::pair<uint32_t,uint32_t> PortHandler::get_relative_port(uint32_t port_index, int n_ports){
    std::pair<uint32_t,uint32_t> ret;

    ret.first = port_index / n_ports;
    ret.second = port_index % n_ports;

    return ret;
}


PortData PortHandler::get_port_data(const uint32_t port_index){
    // determine module type
    PortData port_data;
    std::pair<uint32_t,uint32_t> relative_port;

    if(port_in_range(port_index,PORT_OSC_RANGE)){
        port_data.module_type = ModuleType::OSCILLATOR;
        relative_port = get_relative_port(port_index - PORT_OSC_RANGE.first,CTRL_OSC_N);
    } else if(port_in_range(port_index,PORT_ENV_RANGE)){
        port_data.module_type = ModuleType::ENVELOPE;
        relative_port = get_relative_port(port_index - PORT_ENV_RANGE.first,CTRL_ENV_N);
    } else if(port_in_range(port_index,PORT_LFO_RANGE)){
        port_data.module_type = ModuleType::LFO;
        relative_port = get_relative_port(port_index - PORT_LFO_RANGE.first,CTRL_LFO_N);
    } else if(port_in_range(port_index,PORT_FILTER_RANGE)){
        port_data.module_type = ModuleType::FILTER;
        relative_port = get_relative_port(port_index - PORT_FILTER_RANGE.first,CTRL_FILTER_N);
    } else {
        port_data.module_type = ModuleType::MODULE_NULL;
    } 

    port_data.instance = relative_port.first;
    port_data.index = relative_port.second;

    return port_data;
}

uint32_t PortHandler::get_port(const ModuleType module_type, const uint32_t instance, const uint32_t relative_port){
    switch(module_type){
    case ModuleType::OSCILLATOR:
        return PORT_OSC_RANGE.first + CTRL_OSC_N * instance + relative_port;
    case ModuleType::ENVELOPE:
        return PORT_ENV_RANGE.first + CTRL_ENV_N * instance + relative_port;
    case ModuleType::LFO:
        return PORT_LFO_RANGE.first + CTRL_LFO_N * instance + relative_port;
    case ModuleType::FILTER:
        return PORT_FILTER_RANGE.first + CTRL_FILTER_N * instance + relative_port;
    default:
        return 0;
    }

}