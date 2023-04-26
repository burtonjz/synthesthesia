#define BOOST_TEST_MODULE BaseOscillatorTest

#include <boost/test/unit_test.hpp>
#include <cmath>

#include "oscillator-base.hpp"

BOOST_AUTO_TEST_CASE(sine_wave_test){
    const double freq = 440.0;
    const double rate = 48000.0;
    const double duration = 1.0;

    BaseOscillator osc(WAVE_SINE,freq,rate,-1.0,1.0,1.0,0.0);

    const std::size_t n_samples = duration * rate;

    for (std::size_t i = 0; i < n_samples; ++i){
        const float sample = osc.get_sample();
        const float expected = std::sin(2 * M_PI * freq * i / rate );

        BOOST_CHECK_CLOSE(sample,expected,1e-6);

        osc.tick();
    }
}
