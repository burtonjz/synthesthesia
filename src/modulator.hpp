#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include "config.hpp"
#include "modulator-type.hpp"
#include <cstdint>

class Modulator {
protected:
    bool is_active;
    uint16_t connections; // each bit is a connection to another object
public:
    Modulator();
    Modulator(bool is_active_);
    
    virtual ~Modulator();

    // getters setters
    virtual ModulatorType getType() const;
    
    bool get_is_active() const;
    virtual void set_is_active(bool status);

    uint16_t get_connections() const;
    void set_connections(uint16_t c);

    // virtual modulation functions
    virtual float modulate_frequency(double input) = 0;
    virtual float modulate_amplitude(double input) = 0;
    virtual float modulate_phase(double input) = 0;
    // TODO: modulate filters? cutoff, resonance, etc..    
};


#endif // MODULATOR_HPP_