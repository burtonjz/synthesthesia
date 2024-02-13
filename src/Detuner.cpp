#include "Detuner.hpp"
#include "ModulatorType.hpp"

#include <cmath>

// initialization of static class member variables
std::array<double,CONFIG_DETUNE_MAX_CENTS_2 + 1> Detuner::scaleFactor ;

Detuner::Detuner():
    Modulator(ModulatorType::Detuner)
{}

double Detuner::getDetuneFactor(int cents){
    int idx = cents + CONFIG_DETUNE_MAX_CENTS ; // shift negative values into index range
    return scaleFactor[idx] ;
}

void Detuner::generate(){
    for (int i = 0; i < CONFIG_DETUNE_MAX_CENTS_2 + 1; ++i){
        int cents = i - CONFIG_DETUNE_MAX_CENTS ; 
        scaleFactor[i] = std::pow(2.0, cents / 1200.0 );
    }
}

double Detuner::modulate(double value, ParameterModMap* modp) const {
    if ( modp->find(ModulationParameter::DETUNE_CENTS) == modp->end() ) return value ;
    return value * getDetuneFactor((*modp)[ModulationParameter::DETUNE_CENTS]);
}
