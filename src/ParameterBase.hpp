#ifndef __PARAMETER_BASE_HPP_
#define __PARAMETER_BASE_HPP_

#include "ParameterType.hpp"
#include "ModulationParameter.hpp"

#include "BMap.hpp"

class ParameterBase {
protected:
    ParameterType type_ ;
    bool modulatable_ ;
    BMap<ModulationParameter,double,N_MODULATION_PARAMETERS> modulationParameters_ ;

public:
    ParameterBase(
        ParameterType typ, 
        bool modulatable, 
        BMap<ModulationParameter,double,N_MODULATION_PARAMETERS> modulationParameters
    ):
        type_(typ),
        modulatable_(modulatable),
        modulationParameters_(modulationParameters)
    {}

    ParameterBase(
        ParameterType typ, 
        bool modulatable
    ):
        type_(typ),
        modulatable_(modulatable)
    {}

    virtual void resetValue() = 0 ;
    virtual void setModulatable(bool modulatable) = 0 ;
    virtual bool isModulatable() const = 0 ;
    virtual void modulate() = 0 ;
};


#endif // __PARAMETER_BASE_HPP_