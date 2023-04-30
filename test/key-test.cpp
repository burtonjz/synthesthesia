#define TEST_MODE_
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE KeyTest

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstdint>
#include <array>

#include "key.hpp"
#include "cfg-oscillator.hpp"
#include "modulator-env.hpp"

/*
===========================================
============== Key UNIT TESTS =============
===========================================

Case | Description
---------------------------------------------
1    | Amp Modulation envelope performs as expected

*/

BOOST_AUTO_TEST_CASE(adsr_envelope_logic_test){
    const double rate = 100;
    const uint8_t midi_note = 1;
    const float A = 0.1;
    const float D = 0.1;
    const float S = 0.75;
    const float R = 0.1;

    double time;
    float adsr_level;
    float expected_adsr_level;

    Key key = Key(rate);
    ADSR adsr = {A,D,S,R};
    ADSREnvelope env = ADSREnvelope({A,D,S,R});
    std::array<OscillatorConfig,N_OSCILLATORS> config;

    config[0] = {
        1,
        WAVE_SINE,
        nullptr, // frequency modulator
        &env, // amplitude modulator
        nullptr, // phase modulator
        nullptr, // gain LinearFader
        0.0
    };

    BOOST_ASSERT(key.get_status() == KEY_OFF);// key should be off

    key.press(config,midi_note,127);
    BOOST_ASSERT(key.get_status() == KEY_PRESSED);

    // attack phase
    for(int i = 0; i < rate * A; ++i){
        time = i / rate;
        std::array<float,N_OSCILLATORS> sample = key.get_sample();
        
        // sample should never be larger than the ADSR stage value
        adsr_level = adsr.get_level(key.get_status(),time,key.get_start_level(0));
        BOOST_ASSERT(sample[0] <= adsr_level);

        // let's also make sure ADSR level matches what we think it should be
        expected_adsr_level = 1.0f * time / A;
        BOOST_ASSERT(adsr_level == expected_adsr_level);

        key.tick();
    }

    // TODO: add other ADSR phases, and add other tests for interrupted actions like pressing during release stage

}
