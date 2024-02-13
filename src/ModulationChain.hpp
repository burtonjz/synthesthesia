#ifndef __MODULATION_CHAIN_HPP_
#define __MODULATION_CHAIN_HPP_

#include "config.hpp"
#include "Modulator.hpp"


#include <array>
#include <iostream>

class ModulationChain: public Modulator {
private:
    std::array<Modulator*,CONFIG_MAX_MODULATION_CHAIN> modulators_ ;

public:
    ModulationChain():
        Modulator(ModulatorType::ModulationChain),
        modulators_{nullptr}
    {
    }

    double modulate(double value, ParameterModMap* modp) const override {
        double v = value ;
        for(int i = 0; i < CONFIG_MAX_MODULATION_CHAIN; ++i){
            if(modulators_[i]){
                v = modulators_[i]->modulate(v,modp);
            }
        }

        return v ;
    }

    /**
     * @brief removes all modulator pointers from the chain
    */
    void reset(){
        for(int i = 0; i < CONFIG_MAX_MODULATION_CHAIN; ++i){
            modulators_[i] = nullptr ;
        }
    }

    /**
     * @brief adds a modulator to the next available slot in the modulator chain
    */
    void addModulator(Modulator* mod){
        for(int i = 0; i < CONFIG_MAX_MODULATION_CHAIN; ++i){
            if(!modulators_[i]){
                modulators_[i] = mod ;
                return ;
            }
        }
        std::cout << "[ModulationChain] addModulator: no more space to add another modulator. Exiting..." << std::endl ;
    }

};


#endif // __MODULATION_CHAIN_HPP_