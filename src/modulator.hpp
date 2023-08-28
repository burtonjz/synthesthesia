#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include "config.hpp"
#include "modulator-type.hpp"
#include "cfg-connection.hpp"
#include "connection-manager.hpp"
#include <cstdint>

class Modulator {
protected:
    ConnectionManager connections; 
public:
    Modulator();
    
    virtual ~Modulator();

    // getters setters
    virtual ModulatorType getType() const;

    /**
     * @brief returns true if at least one connection to the modulator exists
    */
    bool get_is_active() const;

    /**
     * @brief returns the bit index of the requested connection (-1 if not exist)
     * 
     * @param module module id to make a connection with
     * @param instance instance id to make a connection with
     * @param port which port/component to make a connection with
    */
    int find_connection(ModulatableType module, int instance, int port) const;

    /**
     * @brief set modulator connection data structure
     * 
     * @param d float value (from LV2 port)
    */
    void set_connections(float d);

    /**
     * @brief get modulator connection data encoded as a float (to send to LV2 port)
    */
    float get_connections_as_float();
    
    // virtual modulation functions
    virtual float modulate_frequency(double input) = 0;
    virtual float modulate_amplitude(double input) = 0;
    virtual float modulate_phase(double input) = 0;
    virtual float modulate_filter_q(double input) = 0;
    virtual float modulate_filter_fc(double input) = 0;
    // TODO: modulate filters? cutoff, resonance, etc..    
};


#endif // MODULATOR_HPP_