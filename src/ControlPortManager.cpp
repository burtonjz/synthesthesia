#include "ControlPortManager.hpp"

#include "config.hpp"
#include "portInfo.hpp"

#include "Oscillator.hpp"
#include "PolyOscillator.hpp"
#include "ADSREnvelope.hpp"
#include "LFO.hpp"
// #include "Filter.hpp"

#include <utility>

#include <iostream>

// static functions
std::pair<const ParameterType*, size_t> ControlPortManager::getControlPorts(ModuleType typ){
    switch(typ){
    case ModuleType::Oscillator:
        return Oscillator::getControlPorts();
    case ModuleType::PolyOscillator:
        return PolyOscillator::getControlPorts();
    case ModuleType::ADSREnvelope:
        return ADSREnvelope::getControlPorts();
    case ModuleType::LFO:
        return LFO::getControlPorts();
    // case ModuleType::Filter:
    default:
        throw std::invalid_argument("invalid module type received.");
        break ;
    }
}

std::pair<const ParameterType*, size_t> ControlPortManager::getModulatableParameters(ModuleType typ){
    switch(typ){
    case ModuleType::Oscillator:
        return Oscillator::getModulatableParameters();
    case ModuleType::PolyOscillator:
        return PolyOscillator::getModulatableParameters();
    case ModuleType::ADSREnvelope:
        return ADSREnvelope::getModulatableParameters();
    case ModuleType::LFO:
        return LFO::getModulatableParameters();
    // case ModuleType::Filter:
    default:
        throw std::invalid_argument("invalid module type received.");
        break ;
    }
}

// end static functions
ControlPortManager::ControlPortManager():
    start_index_(MidiPorts::MIDI_N + AudioPorts::AUDIO_N),
    n_ports_(),
    module_start_index_(),
    control_values_()
{
}

void ControlPortManager::initialize(){
    // set module start indices and total n_ports
    module_start_index_[ModuleType::PolyOscillator] = start_index_ ;
    module_start_index_[ModuleType::ADSREnvelope] = 
        module_start_index_[ModuleType::PolyOscillator] + getControlPorts(ModuleType::PolyOscillator).second * N_POLY_OSCILLATORS ;
    module_start_index_[ModuleType::LFO] =
        module_start_index_[ModuleType::ADSREnvelope] + getControlPorts(ModuleType::ADSREnvelope).second * N_ENVELOPES ;
    n_ports_ = 
        module_start_index_[ModuleType::LFO] + getControlPorts(ModuleType::LFO).second * N_LFOS ;
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

    uint32_t n_module_ports = getControlPorts(p.module_type).second ;
    port -= module_start_index_[p.module_type] ;
    p.module_index = port / n_module_ports ;
    p.relative_port = port % n_module_ports ;

    return p ;
}

uint32_t ControlPortManager::getAbsolutePort(PortData port_data){
    uint32_t n_module_ports = getControlPorts(port_data.module_type).second ;

    return module_start_index_[port_data.module_type] + n_module_ports * port_data.module_index + port_data.relative_port ;
}

void ControlPortManager::connectPort(uint32_t port, void* data){
    std::cout << "[ControlPortManager] Connecting port " << port << std::endl ;
    control_values_[port] = static_cast<const float*>(data) ;
}

void ControlPortManager::updateModuleParameters(ParameterController* params, ModuleType m, uint32_t instance){
    std::pair<const ParameterType*, size_t> ctrl = getControlPorts(m);

    PortData port_data ;
    port_data.module_type = m ;
    port_data.module_index = instance ;

    uint32_t relative_port = 0 ;
    for (size_t i = 0; i < ctrl.second; ++i){
        port_data.relative_port = relative_port ;
        uint32_t port = getAbsolutePort(port_data);
        setParameterValue(params,ctrl.first[i],port);
        ++relative_port;
    }
}

void ControlPortManager::setParameterValue(ParameterController* params, ParameterType pt, uint32_t port){
    float v ;

    if (control_values_[port]){
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
        case ParameterType::GAIN:
            params->setParameterValue<ParameterType::GAIN>(v);
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
        case ParameterType::FILTER_TYPE:
            params->setParameterValue<ParameterType::FILTER_TYPE>(v);
            break ;
        case ParameterType::CUTOFF:
            params->setParameterValue<ParameterType::CUTOFF>(v);
            break ;
        case ParameterType::Q_FACTOR:
            params->setParameterValue<ParameterType::Q_FACTOR>(v);
            break ;
        case ParameterType::MC_FREQUENCY:
            params->setParameterValue<ParameterType::MC_FREQUENCY>(v);
            break ;
        case ParameterType::MC_AMPLITUDE:
            params->setParameterValue<ParameterType::MC_AMPLITUDE>(v);
            break ;
        case ParameterType::MC_PHASE:
            params->setParameterValue<ParameterType::MC_PHASE>(v);
            break ;
        case ParameterType::MC_PAN:
            params->setParameterValue<ParameterType::MC_PAN>(v);
            break ;
        default:
            break ;
        }
}