#ifndef __PARAMETER_HPP_
#define __PARAMETER_HPP_

#include "ParameterType.hpp"
#include "ModulationParameter.hpp"

#include <boost/container/flat_map.hpp>
#include <limits>
#include <functional>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
#endif

/**
 * @brief template class containing logic to define and manipulate a parameter
 * 
 * See ParameterType.hpp for a list of parameter types, 
*/

template <typename T>
class Parameter {
private:
    ParameterType type_ ;
    bool modulatable_ ;
    T value_ ;
    T instantaneousValue_ ;
    T defaultValue_ ;
    T minValue_ ;
    T maxValue_ ;
    std::function<T(T, boost::container::flat_map<ModulationParameter, T>)> modulationFunction_ ;
    boost::container::flat_map<ModulationParameter,T> modulationParameters_ ;
    

public:
    Parameter(
        ParameterType typ, T defaultValue, bool modulatable, T minValue, T maxValue,
        std::function<T(T, boost::container::flat_map<ModulationParameter, T>)> modulationFunction, boost::container::flat_map<ModulationParameter,T> modulationParameters
    ):
        type_(typ),
        modulatable_(modulatable),
        minValue_(minValue),
        maxValue_(maxValue),
        modulationFunction_(modulationFunction),
        modulationParameters_(modulationParameters)
    {
        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_) ;
    }

    Parameter(ParameterType typ, T defaultValue, bool modulatable, T minValue, T maxValue):
        type_(typ),
        modulatable_(modulatable),
        minValue_(minValue),
        maxValue_(maxValue)
    {
        std::function<T(T, boost::container::flat_map<ModulationParameter, T>)> 
            nullModulationFunction = [](T t, const boost::container::flat_map<ModulationParameter, T>& map) -> T {
                return t ;
            };
        boost::container::flat_map<ModulationParameter, T> map ;

        setModulationFunction(
            nullModulationFunction,   
            map    
        );

        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_);
    }

    Parameter(ParameterType typ, T defaultValue, bool modulatable):
        Parameter(typ,defaultValue,modulatable, parameterLimits[static_cast<int>(typ)].first, parameterLimits[static_cast<int>(typ)].second)
    {}

    Parameter(
        ParameterType typ, T defaultValue, bool modulatable,
        std::function<T(T, boost::container::flat_map<ModulationParameter,T>)> modulationFunction, boost::container::flat_map<ModulationParameter,T> modulationParameters
    ):
        type_(typ),
        modulatable_(modulatable),
        minValue_(parameterLimits[static_cast<int>(typ)].first),
        maxValue_(parameterLimits[static_cast<int>(typ)].second),
        modulationFunction_(modulationFunction),
        modulationParameters_(modulationParameters)
    {
        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_);
    }


    /**
     * @brief set parameter value
     * 
     * @param value new value
    */
    void setValue(T value){
        value_ = limitToRange(value);
        setInstantaneousValue(value_);
    }

    /**
     * @brief sets parameter's value back to default
    */
    void resetValue(){
        setValue(defaultValue_);
    }

    /**
     * @brief returns the parameter value
    */
    T getValue() const {
        return value_ ;
    }

    /**
     * @brief returns the parameter instantaneous (i.e., modulated) value
    */
    T getInstantaneousValue() const {
        // #ifdef DEBUG
        // if (type_ == ParameterType::AMPLITUDE ){
        //     std::cout << "[Parameter] getInstantaneousValue: type=" << static_cast<int>(type_)
        //         << ", value=" << value_
        //         << ", inst_value=" << instantaneousValue_ << std::endl ;
        // }
        // #endif
        return instantaneousValue_ ;
    }

    /**
     * @brief indicate if the parameter should be modulatable
     * 
     * @param modulatable boolean true if modulatable
    */
    void setModulatable(bool modulatable){
        modulatable_ = modulatable ;
    }

    /**
     * @brief returns whether the parameter is modulatable
    */
    bool isModulatable() const {
        return modulatable_ ;
    }

    /**
     * @brief set the modulation function
    */
    void setModulationFunction(
        std::function<T(T, boost::container::flat_map<ModulationParameter,T>)> func, 
        boost::container::flat_map<ModulationParameter,T> modulationParameters
    ){
        modulationFunction_ = func ;
        modulationParameters_ = modulationParameters ;
    }

    /**
     * @brief limit value to Parameter's range
     * 
     * @param value value
    */
    T limitToRange(T value) const {
        if ( value < minValue_ ) return minValue_ ;
        if ( value > maxValue_ ) return maxValue_ ;
        return value ;
    }

    /**
     * @brief modulate the parameter with the set modulation function
     * 
     * if not explicitly set, the modulation function will do nothing!
     * 
    */
    void modulate(){
        if(!modulatable_) return ;
        
        setInstantaneousValue(modulationFunction_(value_, modulationParameters_));
    }

private:

    void setInstantaneousValue(T v){
        instantaneousValue_ = v ;
        // #ifdef DEBUG
        // if (type_ == ParameterType::AMPLITUDE ){
        //     std::cout << "[Parameter] setInstantaneousValue: type=" << static_cast<int>(type_)
        //         << ", value=" << value_
        //         << ", inst_value=" << instantaneousValue_ << std::endl ;
        // }
        // #endif
    }
};

#endif // __PARAMETER_HPP_