#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include "modulator-type.hpp"

class Modulator {
protected:
    bool is_active;

public:
    Modulator();
    Modulator(bool is_active_);
    
    virtual ~Modulator();
    
    virtual float modulate_frequency(double input) = 0;
    virtual float modulate_amplitude(double input) = 0;
    virtual float modulate_phase(double input) = 0;
    // TODO: modulate filters? cutoff, resonance, etc..

    virtual ModulatorType getType() const;

    bool get_is_active() const;

    virtual void set_is_active(bool status);
};


#endif // MODULATOR_HPP_