#ifndef PANNER_HPP_
#define PANNER_HPP_

/*
A Panner with a balance input, currently only supports 1 or 2 channels.
*/ 

#include <array>
#include "linear-fader.hpp"
#include "config.hpp"

class Panner {
private:
    LinearFader<float> balanceFader;

    // template function to handle N_CHANNELS
    template  <int N>
    std::array<float,N> _get_impl() const {
        std::array<float,N> result;
        if constexpr (N == 1){
            result[0] = 1.0;
        } else if constexpr (N == 2){
            result[1] = balanceFader.get() / 2.0 + 0.5;
            result[0] = 1 - result[1];
        } else {
            assert(false && "Unesxpected value in switch statement");
        }
        return result;
    }

public:
    Panner(): balanceFader(0.0f) {}

    void set(float balance, uint32_t dist){
        balanceFader.set(balance,dist);
    }

    // return channel weights in array
    std::array<float,N_CHANNELS> get() const {
        return _get_impl<N_CHANNELS>();
    }

    void tick(){
        balanceFader.tick();
    }
};

#endif // PANNER_HPP_