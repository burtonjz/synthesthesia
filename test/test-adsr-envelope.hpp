#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstdint>

#include "synthesthesia.hpp"

BOOST_AUTO_TEST_SUITE(oscillator_base_test)

/*
===========================================
======= BaseOscillator UNIT TESTS =========
===========================================

Case | Description
---------------------------------------------------------------
1    | Midi note is properly converted to frequency

*/

// case 1: test that triangle wave with ADSR amp modulation returns expected values
BOOST_AUTO_TEST_CASE(triangle_wave_with_adsr){
    const double rate = 100;
    const float A = 0.1f;
    const float D = 0.1f;
    const float S = 0.75f;
    const float R = 0.1f;

    ADSREnvelope env({A,D,S,R});

    Synthesthesia synth(rate);

    // TODO allow key access and oscillator configuration access when in TEST_MODE_

    


}
BOOST_AUTO_TEST_SUITE_END()