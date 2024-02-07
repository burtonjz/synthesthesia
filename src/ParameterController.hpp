#ifndef __PARAMETER_CONTROLLER_HPP_
#define __PARAMETER_CONTROLLER_HPP_

#include "commonTypes.hpp"

#include "ParameterType.hpp"
#include "ParameterBase.hpp"
#include "Parameter.hpp"
#include "Modulator.hpp"

#include "BMap.hpp"
#include <limits>
#include <functional>
#include <variant>

#include <stdexcept>
#include <sstream>

#define TYPE_TRAIT(type) typename ParameterTypeTraits<type>::ValueType

/**
 * @brief Feature class to maintain control of module parameters
 * 
 * The ParameterController is responsible for storing and controlling all 
 * parameter information. It should be initialized prior to real-time processing.
 * 
*/
class ParameterController {
private:
    BMap<ParameterType,ParameterBase*,N_PARAMETER_TYPES> parameters_ ;
    BMap<ParameterType,bool,N_PARAMETER_TYPES> reference_parameters_ ;

public:
    /**
     * @brief constructor for ParameterController
    */
    ParameterController():
        parameters_()
    {}

    /**
     * @brief add a Parameter to the controller
     * 
     * @param param Id for the parameter
     * @param defaultValue default value to reset value to
     * @param modulatable set the parameter to be modulatable
     * @param minValue minimum value limit [default: minimum for type]
     * @param maxValue maximum value limit [default: maximum for type]
     * @param mod_ptr pointer to modulator 
     * @param modp modulation parameters
    */
    template <ParameterType param>
    void addParameter(
        TYPE_TRAIT(param) defaultValue, 
        bool modulatable,
        TYPE_TRAIT(param) minValue,
        TYPE_TRAIT(param) maxValue,
        Modulator* mod_ptr,
        ParameterModMap modp
    ){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            Parameter<param>* p = new Parameter<param>(defaultValue, modulatable, minValue, maxValue, mod_ptr, modp);
            parameters_[param] = p ;
        }
    }

    /**
     * @brief add a Parameter to the controller
     * 
     * @param param Id for the parameter
     * @param defaultValue default value to reset value to
     * @param modulatable set the parameter to be modulatable
     * @param minValue minimum value limit [default: minimum for type]
     * @param maxValue maximum value limit [default: maximum for type]
    */
    template <ParameterType param>
    void addParameter(TYPE_TRAIT(param) defaultValue, bool modulatable, TYPE_TRAIT(param) minValue, TYPE_TRAIT(param) maxValue){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            Parameter<param>* p = new Parameter<param>(defaultValue, modulatable, minValue, maxValue);
            parameters_[param] = p ;
        }
    }

    /**
     * @brief add a Parameter to the controller
     * 
     * @param param Id for the parameter
     * @param defaultValue default value to reset value to
     * @param modulatable set the parameter to be modulatable
     * @param mod_ptr pointer to modulator instance
     * @param modp modulation parameters map
    */
    template <ParameterType param>
    void addParameter(
        TYPE_TRAIT(param) defaultValue, 
        bool modulatable,
        Modulator* mod_ptr, 
        ParameterModMap modp
    ){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            Parameter<param>* p = new Parameter<param>(defaultValue, modulatable, mod_ptr, modp);
            parameters_[param] = p ;
        }
    }

    /**
     * @brief add a Parameter to the controller
     * 
     * @param param Id for the parameter
     * @param defaultValue default value to reset value to
     * @param modulatable set the parameter to be modulatable
    */
    template <ParameterType param>
    void addParameter(TYPE_TRAIT(param) defaultValue, bool modulatable){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            Parameter<param>* p = new Parameter<param>(defaultValue, modulatable);
            parameters_[param] = p ;
        }
    }

    /**
     * @brief add all parameters from other as references to this controller
     * 
     * @param other other ParameterController object reference
    */
    void addReferences(ParameterController& other){
        const auto& otherParams = other.getParameters();
        for (const auto& pair : otherParams ){
            parameters_[pair.first] = pair.second ;
            reference_parameters_[pair.first] = true ;
        }
    }

    /**
     * @brief set value for parameter
     * 
     * @param param Id for the parameter
     * @param value value to set
    */
    template <ParameterType param>
    void setParameterValue(TYPE_TRAIT(param) value ){
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            dynamic_cast<Parameter<param>*>(it->second)->setValue(value);
        }
    }

    template <ParameterType param>
    TYPE_TRAIT(param) getParameterValue() const {
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            return dynamic_cast<Parameter<param>*>(it->second)->getValue();
        }

        std::stringstream errorMessage ;
        errorMessage << "ParameterController: requested parameter " << static_cast<int>(param)
                     << " does not exist in controller." ;
        throw std::runtime_error(errorMessage.str());
    }

    template <ParameterType param>
    TYPE_TRAIT(param) getParameterInstantaneousValue() const {
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            return dynamic_cast<Parameter<param>*>(it->second)->getInstantaneousValue();
        }

        std::stringstream errorMessage ;
        errorMessage << "ParameterController: requested parameter " << static_cast<int>(param)
                     << " does not exist in controller." ;
        throw std::runtime_error(errorMessage.str());
    }

    template <ParameterType param>
    void setParameterModulation(
        Modulator* mod_ptr, 
        ParameterModMap modp
    ){
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            dynamic_cast<Parameter<param>*>(it->second)->setModulation(mod_ptr,modp);
        }
    }

    /**
     * @brief get a read-only copy of the parameters map
    */
    BMap<ParameterType, ParameterBase*, N_PARAMETER_TYPES> getParameters(){
        return parameters_;
    }

    /**
     * @brief returns whether the given parameter exists in the controller
     * 
     * @param param parameter type to search
    */
    bool parameterExists(ParameterType param){
        auto it = parameters_.find(param);
        return it != parameters_.end();
    }

    /**
     * @brief modulates a parameter if it is modulatable
     * 
     * @param param Id for the parameter
    */
    template <ParameterType param>
    void modulateParameter(){
        auto it = parameters_.find(param);
        if (it != parameters_.end() && !isReferenceParameter(param) ){
            dynamic_cast<Parameter<param>*>(it->second)->modulate();
        }
    }

private:

    bool isReferenceParameter(ParameterType param){
        auto it = reference_parameters_.find(param);
        return it != reference_parameters_.end();
    }
};

#endif // __PARAMETER_CONTROLLER_HPP_