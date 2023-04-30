#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BaseOscillatorTest

#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstdint>

#include "oscillator-base.hpp"

/*
===========================================
======= BaseOscillator UNIT TESTS =========
===========================================

Case | Description
---------------------------------------------------------------
1    | Midi note is properly converted to frequency
2    | Sine wave outputs from base oscillator as expected
3    | Triangle wave outputs from base oscillator as expected
4    | Square wave outputs from base oscillator as expected
5    | Saw wave outputs from base oscillator as expected

*/

BOOST_AUTO_TEST_CASE(midi_note_to_frequency_test){
    const double rate = 48000.0;

    uint8_t midi_note;
    double freq;
    BaseOscillator osc;

    midi_note = 0;
    osc = BaseOscillator(WAVE_SINE,midi_note,rate,-1.0,1.0,1.0,0.0);
    freq = std::round(osc.get_freq()*100) / 100;
    BOOST_CHECK_EQUAL(freq,8.18);

    midi_note = 69;
    osc = BaseOscillator(WAVE_SINE,midi_note,rate,-1.0,1.0,1.0,0.0);
    freq = std::round(osc.get_freq()*100) / 100;
    BOOST_CHECK_EQUAL(freq,440.00);

    midi_note = 127;
    osc = BaseOscillator(WAVE_SINE,midi_note,rate,-1.0,1.0,1.0,0.0);
    freq = std::round(osc.get_freq()*100) / 100;
    BOOST_CHECK_EQUAL(freq,12543.85);
}

// Case 2: Sine wave outputs from base oscillator as expected
BOOST_AUTO_TEST_CASE(sine_wave_test){
    const double freq = 440.0;
    const double rate = 48000.0;
    const double duration = .01;

    BaseOscillator osc(WAVE_SINE,freq,rate,-1.0,1.0,1.0,0.0);

    const std::size_t n_samples = duration * rate;

    for (std::size_t i = 0; i < n_samples; ++i){
        const float sample = osc.get_sample();
        const float expected = std::sin(2 * M_PI * freq * i / rate );

        BOOST_CHECK_CLOSE(sample,expected,1e-6);

        osc.tick();
    }
}

// Case 3: Triangle wave outputs from base oscillator as expected
BOOST_AUTO_TEST_CASE(triangle_wave_test){
    const double freq = 440.0;
    const double rate = 48000.0;
    const double duration = .01;

    BaseOscillator osc(WAVE_TRIANGLE,freq,rate,-1.0,1.0,1.0,0.0);

    const std::size_t n_samples = duration * rate;

    for (std::size_t i = 0; i < n_samples; ++i){
        const float sample = osc.get_sample();
        const double step = static_cast<double>(i) * (2.0 * M_PI * freq) / rate;
        float expected = std::fmod(step,2.0 * M_PI) / M_PI - 1;
        expected = (std::abs(expected) - 0.5) * 2.0;

        BOOST_CHECK_CLOSE(sample,expected,1e-6);

        osc.tick();
    }
}

// Case 4: Square wave outputs from base oscillator as expected
BOOST_AUTO_TEST_CASE(square_wave_test){
    const double freq = 440.0;
    const double rate = 48000.0;
    const double duration = .01;

    BaseOscillator osc(WAVE_SQUARE,freq,rate,-1.0,1.0,1.0,0.0);

    const std::size_t n_samples = duration * rate;

    for (std::size_t i = 0; i < n_samples; ++i){
        const float sample = osc.get_sample();
        const float x = std::sin(2 * M_PI * freq * i / rate );
        const float expected = (x >= 0) ? 1.0 : -1.0;
        BOOST_CHECK_CLOSE(sample,expected,1e-6);

        osc.tick();
    }
}

// Case 5: Saw wave outputs from base oscillator as expected
BOOST_AUTO_TEST_CASE(saw_wave_test){
    const double freq = 440.0;
    const double rate = 48000.0;
    const double duration = .01;

    BaseOscillator osc(WAVE_SAW,freq,rate,-1.0,1.0,1.0,0.0);

    const std::size_t n_samples = duration * rate;

    for (std::size_t i = 0; i < n_samples; ++i){
        const float sample = osc.get_sample();
        const double step = static_cast<double>(i) * (2.0 * M_PI * freq) / rate;
        float expected = std::fmod(step,2.0 * M_PI) / M_PI - 1;

        BOOST_CHECK_CLOSE(sample,expected,1e-6);

        osc.tick();
    }
}