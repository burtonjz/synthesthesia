#ifndef MODULATOR_ENV_HPP_
#define MODULATOR_ENV_HPP_

#include <cstdint>
#include "modulator.hpp"
#include "modulator-type.hpp"
#include "key-status.hpp"
#include "config.hpp"
#include "adsr.hpp"


class ADSREnvelope : public Modulator {
private:
    ADSR adsr;
    uint16_t connections; // contains logic for what signals are being modulated.
public:
    ADSREnvelope(ADSR a);
    ADSREnvelope();

    ModulatorType getType() const override;

    ADSR get_adsr() const;
    void set_adsr(ADSR a);

    uint16_t get_connections() const;
    void set_connections(uint16_t c);

    float get_level(KeyStatus status, double time, float start_level);

    // modulator overrides
    float modulate_frequency(double input,KeyStatus status, double time, float start_level);
    float modulate_amplitude(double input,KeyStatus status, double time, float start_level);
    float modulate_phase(double input,KeyStatus status, double time, float start_level);

    float modulate_frequency(double input) override;
    float modulate_amplitude(double input) override;
    float modulate_phase(double input) override;

};

#endif // MODULATOR_ENV_HPP_