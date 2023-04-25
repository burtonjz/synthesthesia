#ifndef PANNER_HPP_
#define PANNER_HPP_

/*
A Panner with a balance input, which normalizes the output
between two channels
*/ 

#include <array>
#include "linear-fader.hpp"
#include "config.hpp"

class Panner {
private:
    LinearFader<float> balanceFader;
public:
    Panner(): balanceFader(0.0f) {}

    void set(float balance, uint32_t dist){
        balanceFader.set(balance,dist);
    }

    std::array<float,N_CHANNELS> get() const {
        float right = balanceFader.get() / 2.0 + 0.5;
        float left = 1 - right;

        return { left, right };
    }

    void tick(){
        balanceFader.tick();
    }
};

#endif // PANNER_HPP_