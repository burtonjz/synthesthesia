#ifndef __PARAMETER_BASE_HPP_
#define __PARAMETER_BASE_HPP_

#include "commonTypes.hpp"
#include "ParameterType.hpp"
#include "Modulator.hpp"

class ParameterBase {
protected:
    ParameterType type_ ;
    bool modulatable_ ;
    Modulator* mod_ptr_ ;
    ParameterModMap modp_ ; 
    
public:
    ParameterBase(
        ParameterType typ, 
        bool modulatable,
        Modulator* mod_ptr,
        ParameterModMap modp
    ):
        type_(typ),
        modulatable_(modulatable),
        mod_ptr_(mod_ptr),
        modp_(modp)
    {}

    ParameterBase(
        ParameterType typ, 
        bool modulatable
    ):
        type_(typ),
        modulatable_(modulatable),
        mod_ptr_(nullptr),
        modp_()
    {}

    virtual void resetValue() = 0 ;
    virtual void setModulatable(bool modulatable) = 0 ;
    virtual bool isModulatable() const = 0 ;
    virtual void modulate() = 0 ;

    void setModulation(Modulator* mod_ptr, ParameterModMap modp){
        mod_ptr_ = mod_ptr ;
        modp_ = modp ;
    }
};


#endif // __PARAMETER_BASE_HPP_