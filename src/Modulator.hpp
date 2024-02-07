#ifndef __MODULATOR_HPP_
#define __MODULATOR_HPP_

#include "ModulatorType.hpp"
#include "commonTypes.hpp"

/**
 * @brief base class for all Modulator Modules.
*/
class Modulator {
private:
    ModulatorType type_ ;

public:
    Modulator(ModulatorType typ):
        type_(typ)
    {}

    virtual ~Modulator() = default ;

    virtual double modulate(double value, ParameterModMap* modp) const = 0 ;

    virtual ModulatorType getType() const {
        return type_ ;
    }

};

#endif // __MODULATOR_HPP_

