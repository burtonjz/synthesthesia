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
    */
    template <typename T>
    void addParameter(ParameterType param, T defaultValue, bool modulatable,
        T minValue = std::numeric_limits<T>::lowest(),
        T maxValue = std::numeric_limits<T>::max()    
    ){
        auto it = parameters_.find(param);
        if (it == parameters_.end()){
            parameters_[param] = Parameter<T>(param, defaultValue, modulatable, minValue, maxValue);
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
    template <typename T>
    void setParameterValue(ParameterType param, T value ){
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            boost::any_cast<Parameter<T>>(it->second).setValue(value);
        }
    }

    template <typename T>
    T getParameterValue(ParameterType param) const {
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            return boost::any_cast<Parameter<T>>(it->second).getValue();
        }

        throw std::runtime_error("ParameterController: requested parameter does not exist in controller");
    }

    /**
     * @brief get a read-only copy of the modulatable ParameterTypes
    */
    const boost::container::flat_set<ParameterType>* getModulatableParameterTypes() const {
        return &modulatableParameters_ ;
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
     * @param modulationFunction a function returning Type T, with two inputs (parameterValue, modulatorValue)
     * @param modulatorValue the value to be the second parameter of the modulationFunction
    */
    template <typename T>
    void modulateParameter(ParameterType param, std::function<T(T,T)> modulationFunction, T modulatorValue){
        auto it = parameters_.find(param);
        if (it != parameters_.end() ){
            boost::any_cast<Parameter<T>>(it->second).modulate(modulationFunction,modulatorValue);
        }
    }

};

#endif // __PARAMETER_CONTROLLER_HPP_