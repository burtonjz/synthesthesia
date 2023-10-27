#ifndef __PARAMETER_CONTROLLER_HPP_
#define __PARAMETER_CONTROLLER_HPP_

#include "ParameterType.hpp"
#include "Parameter.hpp"

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/any.hpp>
#include <limits>
#include <functional>
#include <exception>
#include <variant>

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
    boost::container::flat_map<ParameterType,boost::any> parameters_ ;
    boost::container::flat_set<ParameterType> modulatableParameters_ ;
    boost::container::flat_set<ParameterType> keys_ ;

public:
    /**
     * @brief constructor for ParameterController
    */
    ParameterController():
        parameters_(),
        modulatableParameters_()
    {}

    /**
     * @brief add a Parameter to the controller
     * 
     * @param param Id for the parameter
     * @param defaultValue default value to reset value to
     * @param modulatable set the parameter to be modulatable
     * @param minValue minimum value limit [default: minimum for type]
     * @param maxValue maximum value limit [default: maximum for type]
     * @param modulationFunction function to perform modulation
     * @param modulationParameters map of ModulationParameters
    */
    template <ParameterType param>
    void addParameter(
        TYPE_TRAIT(param) defaultValue, 
        bool modulatable,
        TYPE_TRAIT(param) minValue,
        TYPE_TRAIT(param) maxValue,
        std::function<TYPE_TRAIT(param)(TYPE_TRAIT(param), boost::container::flat_map<ModulationParameter,TYPE_TRAIT(param)>)> modulationFunction, 
        boost::container::flat_map<ModulationParameter,double> modulationParameters
    ){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            parameters_[param] = Parameter<param>(defaultValue, modulatable, minValue, maxValue, modulationFunction, modulationParameters);
            keys_.insert(param);
            if (modulatable) modulatableParameters_.insert(param);
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
            parameters_[param] = Parameter<param>(defaultValue, modulatable, minValue, maxValue);
            keys_.insert(param);
            if (modulatable) modulatableParameters_.insert(param);
        }
    }

    /**
     * @brief add a Parameter to the controller
     * 
     * @param param Id for the parameter
     * @param defaultValue default value to reset value to
     * @param modulatable set the parameter to be modulatable
     * @param modulationFunction function to perform modulation
     * @param modulationParameters map of ModulationParameters
    */
    template <ParameterType param>
    void addParameter(
        TYPE_TRAIT(param) defaultValue, 
        bool modulatable,
        std::function<TYPE_TRAIT(param)(TYPE_TRAIT(param), boost::container::flat_map<ModulationParameter,TYPE_TRAIT(param)>)> modulationFunction, 
        boost::container::flat_map<ModulationParameter,double> modulationParameters
    ){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            parameters_[param] = Parameter<param>(defaultValue, modulatable, modulationFunction, modulationParameters);
            keys_.insert(param);
            if (modulatable) modulatableParameters_.insert(param);
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
            parameters_[param] = Parameter<param>(defaultValue, modulatable);
            keys_.insert(param);
            if (modulatable) modulatableParameters_.insert(param);
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
            boost::any_cast<Parameter<param>&>(it->second).setValue(value);
        }
    }

    template <ParameterType param>
    TYPE_TRAIT(param) getParameterValue() const {
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            return boost::any_cast<Parameter<param>>(it->second).getValue();
        }

        throw std::runtime_error("ParameterController: requested parameter does not exist in controller");
    }

    template <ParameterType param>
    TYPE_TRAIT(param) getParameterInstantaneousValue() const {
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            return boost::any_cast<Parameter<param>>(it->second).getInstantaneousValue();
        }

        throw std::runtime_error("ParameterController: requested parameter does not exist in controller");
    }

    template <ParameterType param>
    void setParameterModulation(
        std::function<TYPE_TRAIT(param)(TYPE_TRAIT(param), boost::container::flat_map<ModulationParameter,TYPE_TRAIT(param)>* )> modulationFunction, 
        boost::container::flat_map<ModulationParameter,double> modulationParameters
    ){
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            boost::any_cast<Parameter<param>&>(it->second).setModulationFunction(modulationFunction,modulationParameters);
        }
    }

    /**
     * @brief get a read-only copy of all ParameterTypes
    */
    const boost::container::flat_set<ParameterType>* getAllParameterTypes() const {
        return &keys_ ;
    }

    /**
     * @brief modulates a parameter if it is modulatable
     * 
     * @param param Id for the parameter
    */
    template <ParameterType param>
    void modulateParameter(){
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            boost::any_cast<Parameter<param>&>(it->second).modulate();
        }
    }

};

#endif // __PARAMETER_CONTROLLER_HPP_