#ifndef MODULATOR_ENV_HPP_
#define MODULATOR_ENV_HPP_

#include <cstdint>
#include <tuple>
#include "modulator.hpp"
#include "modulator-type.hpp"
#include "key-status.hpp"
#include "config.hpp"
#include "adsr.hpp"


class ADSREnvelope : public Modulator {
private:
    ADSR adsr;

public:
    ADSREnvelope(ADSR a);
    ADSREnvelope();

    ModulatorType getType() const override;

    ADSR get_adsr() const;
    void set_adsr(ADSR a);

    float get_level(KeyStatus status, double time, float start_level);

    // modulation funcs
    float modulate_frequency(double input,KeyStatus status, double time, float start_level);
    float modulate_frequency(double input) override;

    float modulate_amplitude(double input,KeyStatus status, double time, float start_level);
    float modulate_amplitude(double input) override;

    float modulate_phase(double input,KeyStatus status, double time, float start_level);
    float modulate_phase(double input) override;

    float modulate_filter_fc(double input, KeyStatus status, double time, float start_level);
    float modulate_filter_fc(double input) override;
    
    float modulate_filter_q(double input, KeyStatus status, double time, float start_level);
    float modulate_filter_q(double input) override;

};

#endif // MODULATOR_ENV_HPP_