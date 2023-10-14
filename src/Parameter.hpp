#ifndef __PARAMETER_HPP_
#define __PARAMETER_HPP_

#include "ParameterType.hpp"

#include <limits>
#include <functional>

/**
 * @brief template class containing logic to define and manipulate a parameter
 * 
 * See ParameterType.hpp for a list of parameter types, 
*/

template <typename T>
class Parameter {
private:
    ParameterType type_;
    bool modulatable_;
    T value_;
    T instantaneousValue_;
    T defaultValue_;
    T minValue_;
    T maxValue_;
    

public:
    Parameter(ParameterType typ, T defaultValue, bool modulatable):
        Parameter(typ,defaultValue,modulatable, parameterLimits[static_cast<int>(typ)].first, parameterLimits[static_cast<int>(typ)].second)
    {}

    Parameter(ParameterType typ, T defaultValue, bool modulatable, T minValue, T maxValue):
        type_(typ),
        modulatable_(modulatable),
        minValue_(minValue),
        maxValue_(maxValue)
    {
        defaultValue_ = limitToRange(defaultValue);
        value_ = defaultValue_ ;
    }

    /**
     * @brief set parameter value
     * 
     * @param value new value
    */
    void setValue(T value){
        value_ = limitToRange(value);
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
        return value_;
    }

    /**
     * @brief returns the parameter instantaneous (i.e., modulated) value
    */
    T getInstantaneousValue() const {
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
     * @brief modulate the parameter with an external function
     * 
     * @param modulationFunction a function returning Type T, with two inputs (parameterValue, modulatorValue)
     * @param modulatorValue the value to be the second parameter of the modulationFunction
    */
    void modulate(std::function<T(T, T)> modulationFunction, T modulatorValue){
        if(modulatable_) instantaneousValue_ = limitToRange(modulationFunction(value_, modulatorValue));
    }

};

#endif // __PARAMETER_HPP_