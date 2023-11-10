#include "ControlPortManager.hpp"
#include "config.hpp"
#include "portInfo.hpp"
#include "PolyOscillator.hpp"
#include "ADSREnvelope.hpp"

#include <boost/container/vector.hpp>

#include <iostream>

uint32_t ControlPortManager::start_index_ = MidiPorts::MIDI_N + AudioPorts::AUDIO_N ;
uint32_t ControlPortManager::n_ports_ ;
boost::container::flat_map<ModuleType,uint32_t> ControlPortManager::module_start_index_ ;
boost::container::flat_map<uint32_t, const float*> ControlPortManager::control_values_ ;

void ControlPortManager::initialize(){
    // set module start indices and total n_ports
    module_start_index_[ModuleType::PolyOscillator] = start_index_ ;
    module_start_index_[ModuleType::ADSREnvelope] = 
        module_start_index_[ModuleType::PolyOscillator] + PolyOscillator::getNumControlPorts() * N_POLY_OSCILLATORS - 1;

    n_ports_ = 
        module_start_index_[ModuleType::ADSREnvelope] + ADSREnvelope::getNumControlPorts() * N_ENVELOPES - 1;

}

uint32_t ControlPortManager::getNumModulePorts(ModuleType m){
    switch(m){
    case ModuleType::PolyOscillator: return PolyOscillator::getNumControlPorts();
    case ModuleType::ADSREnvelope: return ADSREnvelope::getNumControlPorts();
    }
}

PortData ControlPortManager::getPortData(uint32_t port){
    if (port >= n_ports_ ) throw std::invalid_argument("port must be lower than max number of ports");

    PortData p ;
    for(auto it = module_start_index_.rbegin(); it != module_start_index_.rend(); ++it ){
        if(port >= it->second ){
            p.module_type = it->first ;
            break ;
        } 
    }

    uint32_t n_module_ports = getNumModulePorts(p.module_type);
    port -= module_start_index_[p.module_type] ;
    p.module_index = port / n_module_ports ;
    p.relative_port = port % n_module_ports ;

    return p ;
}

uint32_t ControlPortManager::getAbsolutePort(PortData port_data){
    uint32_t n_module_ports = getNumModulePorts(port_data.module_type);

    return module_start_index_[port_data.module_type] + n_module_ports * port_data.module_index + port_data.relative_port ;
}

void ControlPortManager::connectPort(uint32_t port, void* data){
    control_values_[port] = static_cast<const float*>(data) ;
}

void ControlPortManager::updateModuleParameters(ParameterController* params, ModuleType m, uint32_t instance ){
    const boost::container::vector<ParameterType>* control_params ;
    if(m == ModuleType::PolyOscillator) control_params = PolyOscillator::getControlPorts();
    else if (m == ModuleType::ADSREnvelope) control_params = ADSREnvelope::getControlPorts();
    else return ;

    PortData port_data ;
    port_data.module_type = m ;
    port_data.module_index = instance ;

    uint32_t relative_port = 0 ;
    for (const auto& p : *control_params){
        port_data.relative_port = relative_port ;
        uint32_t port = getAbsolutePort(port_data);
        setParameterValue(params,p,port);
        ++relative_port;
    }
    
}

void ControlPortManager::setParameterValue(ParameterController* params, ParameterType pt, uint32_t port ){
    float v ;
    if(control_values_[port]){
        v = *control_values_[port];
    } else return ;
    
    switch(pt){
        case ParameterType::STATUS:
            params->setParameterValue<ParameterType::STATUS>(v);
            break ;
        case ParameterType::WAVEFORM:
            params->setParameterValue<ParameterType::WAVEFORM>(v);
            break ;
        case ParameterType::FREQUENCY:
            params->setParameterValue<ParameterType::FREQUENCY>(v);
            break ;
        case ParameterType::AMPLITUDE:
            params->setParameterValue<ParameterType::AMPLITUDE>(v);
            break ;
        case ParameterType::PHASE:
            params->setParameterValue<ParameterType::PHASE>(v);
            break ;
        case ParameterType::PAN:
            params->setParameterValue<ParameterType::PAN>(v);
            break ;
        case ParameterType::DETUNE:
            params->setParameterValue<ParameterType::DETUNE>(v);
            break ;
        case ParameterType::ATTACK:
            params->setParameterValue<ParameterType::ATTACK>(v);
            break ;
        case ParameterType::DECAY:
            params->setParameterValue<ParameterType::DECAY>(v);
            break ;
        case ParameterType::SUSTAIN:
            params->setParameterValue<ParameterType::SUSTAIN>(v);
            break ;
        case ParameterType::RELEASE:
            params->setParameterValue<ParameterType::RELEASE>(v);
            break ;
        default:
            break ;
        }
}