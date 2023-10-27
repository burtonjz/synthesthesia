#ifndef __PARAMETER_HPP_
#define __PARAMETER_HPP_

#include "ParameterType.hpp"
#include "ModulationParameter.hpp"

#include <boost/container/flat_map.hpp>
#include <limits>
#include <functional>
#include <algorithm>

/**
 * @brief template class containing logic to define and manipulate a parameter
 * 
 * See ParameterType.hpp for a list of parameter types, 
*/

template <ParameterType Type>
class Parameter {
public:
    using ValueType = typename ParameterTypeTraits<Type>::ValueType ;

private:
    ParameterType type_ ;
    bool modulatable_ ;
    ValueType value_ ;
    ValueType instantaneousValue_ ;
    ValueType defaultValue_ ;
    ValueType minValue_ ;
    ValueType maxValue_ ;
    std::function<ValueType(ValueType, boost::container::flat_map<ModulationParameter, double>*)> modulationFunction_ ;
    boost::container::flat_map<ModulationParameter,double> modulationParameters_ ;
    

public:
    Parameter(
        ValueType defaultValue, 
        bool modulatable, 
        ValueType minValue, 
        ValueType maxValue,
        std::function<ValueType(ValueType, boost::container::flat_map<ModulationParameter, double>*)> modulationFunction, 
        boost::container::flat_map<ModulationParameter,double> modulationParameters
    ):
        type_(Type),
        modulatable_(modulatable),
        minValue_(minValue),
        maxValue_(maxValue),
        modulationFunction_(modulationFunction),
        modulationParameters_(modulationParameters)
    {
        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_) ;
    }

    Parameter(
        ValueType defaultValue, 
        bool modulatable, 
        ValueType minValue, 
        ValueType maxValue
    ):
        type_(Type),
        modulatable_(modulatable),
        minValue_(minValue),
        maxValue_(maxValue)
    {
        boost::container::flat_map<ModulationParameter, double> map ;
        auto nullModulationFunction = [](ValueType value, boost::container::flat_map<ModulationParameter, double>* map) -> ValueType {
            return value;
        };

        setModulationFunction(
            nullModulationFunction,   
            map    
        );

        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_);
    }

    Parameter(ValueType defaultValue, bool modulatable):
        Parameter(
            defaultValue,
            modulatable, 
            static_cast<ValueType>(parameterLimits[static_cast<int>(Type)].first), 
            static_cast<ValueType>(parameterLimits[static_cast<int>(Type)].second)
        )
    {}

    Parameter(
        ValueType defaultValue, 
        bool modulatable,
        std::function<ValueType(ValueType, boost::container::flat_map<ModulationParameter,double>* )> modulationFunction, 
        boost::container::flat_map<ModulationParameter,double> modulationParameters
    ):
        type_(Type),
        modulatable_(modulatable),
        minValue_(parameterLimits[static_cast<int>(type_)].first),
        maxValue_(parameterLimits[static_cast<int>(type_)].second),
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
    void setValue(ValueType value){
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
    ValueType getValue() const {
        return value_ ;
    }

    /**
     * @brief returns the parameter instantaneous (i.e., modulated) value
    */
    ValueType getInstantaneousValue() const {
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
        std::function<ValueType(ValueType, boost::container::flat_map<ModulationParameter,double>*)> func, 
        boost::container::flat_map<ModulationParameter,double> modulationParameters
    ){
        modulationFunction_ = func ;
        modulationParameters_ = modulationParameters ;
    }

    /**
     * @brief limit value to Parameter's range
     * 
     * @param value value
    */
    ValueType limitToRange(ValueType value) const {
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
        setInstantaneousValue(modulationFunction_(value_, &modulationParameters_));
    }

private:

    void setInstantaneousValue(ValueType v){
        instantaneousValue_ = v ;
    }

};

#endif // __PARAMETER_HPP_