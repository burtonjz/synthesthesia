#ifndef __PARAMETER_HPP_
#define __PARAMETER_HPP_

#include "ParameterBase.hpp"
#include "ParameterType.hpp"
#include "Modulator.hpp"

#include <limits>
#include <functional>
#include <algorithm>


/**
 * @brief template class containing logic to define and manipulate a parameter
 * 
 * See ParameterType.hpp for a list of parameter types, 
*/

template <ParameterType Type>
class Parameter : public ParameterBase {
public:
    using ValueType = typename ParameterTypeTraits<Type>::ValueType ;
    
private:
    ValueType value_ ;
    ValueType instantaneousValue_ ;
    ValueType defaultValue_ ;
    ValueType minValue_ ;
    ValueType maxValue_ ;

public:
    /**
     * @brief simplest parameter constructor
    */
    Parameter(ValueType defaultValue, bool modulatable):
        Parameter(
            defaultValue,
            modulatable, 
            static_cast<ValueType>(parameterLimits[static_cast<int>(Type)].first), 
            static_cast<ValueType>(parameterLimits[static_cast<int>(Type)].second)
        )
    {}

    /**
     * @brief Construct with min/max values
    */
    Parameter(
        ValueType defaultValue, 
        bool modulatable, 
        ValueType minValue, 
        ValueType maxValue
    ):
        ParameterBase(Type,modulatable),
        minValue_(minValue),
        maxValue_(maxValue)
    {
        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_);
    }

    /**
     * @brief construct with modulation source, no min/max values
    */
    Parameter(
        ValueType defaultValue, 
        bool modulatable,
        Modulator* mod_ptr,
        ParameterModMap modp
    ):
        ParameterBase(Type,modulatable,mod_ptr,modp),
        minValue_(parameterLimits[static_cast<int>(type_)].first),
        maxValue_(parameterLimits[static_cast<int>(type_)].second)
    {
        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_);
    }

    /**
     * @brief construct with modulation source and default values
    */
    Parameter(
        ValueType defaultValue, 
        bool modulatable, 
        ValueType minValue, 
        ValueType maxValue,
        Modulator* mod_ptr,
        ParameterModMap modp
    ):
        ParameterBase(Type,modulatable,mod_ptr,modp),
        minValue_(minValue),
        maxValue_(maxValue)
    {
        defaultValue_ = limitToRange(defaultValue);
        setValue(defaultValue_) ;
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
    void resetValue() override {
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
    void setModulatable(bool modulatable) override {
        modulatable_ = modulatable ;
    }

    /**
     * @brief returns whether the parameter is modulatable
    */
    bool isModulatable() const override {
        return modulatable_ ;
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
    void modulate() override {
        if(!modulatable_ || !mod_ptr_ ) return ;

        setInstantaneousValue(mod_ptr_->modulate(value_, &modp_));
    }

private:

    void setInstantaneousValue(ValueType v){
        instantaneousValue_ = v ;
    }

};

#endif // __PARAMETER_HPP_