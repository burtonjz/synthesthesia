#include "Wavetable.hpp"

#include <cmath>
#include <ctime>

std::array<std::array<double, WAVETABLE_SIZE>, static_cast<int>(Waveform::N)> Wavetable::wavetable_;

const std::array<double,WAVETABLE_SIZE>* Wavetable::getWavetable(Waveform waveform) {
    return &wavetable_[static_cast<int>(waveform)] ;
}

void Wavetable::generate(){
    generateSineWavetable();
    generateSquareWavetable();
    generateTriangleWavetable();
    generateSawWavetable();
    generateNoiseWavetable();
}

void Wavetable::generateSineWavetable(){
    double phase ;
    for(int i = 0; i < WAVETABLE_SIZE; ++i){
        phase = static_cast<double>(i) / WAVETABLE_SIZE ;
        wavetable_[static_cast<int>(Waveform::SINE)][i] = std::sin(2.0 * M_PI * phase);
    }
}

void Wavetable::generateSquareWavetable(){
    double dt = 1.0 / WAVETABLE_SIZE ;
    double phase ;
    double sample ;

    for(int i = 0; i < WAVETABLE_SIZE; ++i){
        phase = static_cast<double>(i) / WAVETABLE_SIZE ;

        sample = phase < 0.5 ? 1.0 : -1.0 ;
        sample += polyBlep(phase,dt); // first drop
        sample -= polyBlep(std::fmod(phase + 0.5, 1.0), dt); // second drop

        wavetable_[static_cast<int>(Waveform::SQUARE)][i] = sample ;
    }
}

void Wavetable::generateTriangleWavetable(){
    // triangle is an integral of a square wave, we'll do a reimann sum
    double dt = 1.0 / WAVETABLE_SIZE ;
    double square_integral = -1.0;
    for(int i = 0; i < WAVETABLE_SIZE; ++i){
        square_integral += wavetable_[static_cast<int>(Waveform::SQUARE)][i] * dt * 4.0;
        wavetable_[static_cast<int>(Waveform::TRIANGLE)][i] = square_integral ;
    }
}

void Wavetable::generateSawWavetable(){
    double sample ;
    double t ;
    double dt = 1.0 / WAVETABLE_SIZE ;

    for(int i = 0; i < WAVETABLE_SIZE; ++i){
        t = i * dt ;    
        sample = -1.0 + 2.0 * t ;
        sample -= polyBlep(t, dt);

        wavetable_[static_cast<int>(Waveform::SAW)][i] = sample ;
    }
}

void Wavetable::generateNoiseWavetable(){
    std::minstd_rand rnd;
    std::uniform_real_distribution<float> distr;
    for(int i = 0; i < WAVETABLE_SIZE; ++i){
        wavetable_[static_cast<int>(Waveform::NOISE)][i] = distr(rnd);
    }
}

double Wavetable::polyBlep(double t, double dt){
    if (t < dt){
        t /= dt ;
        return t + t - t * t - 1.0 ;
    } 

    if (t > 1.0 - dt){
        t = (t - 1.0) / dt;
        return t * t + t + t + 1.0;
    }

    return 0.0;
}