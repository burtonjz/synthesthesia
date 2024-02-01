#include "Detune.hpp"
#include "KeyboardController.hpp"

#include <cmath>

// initialization of static class member variables
std::array<double,CONFIG_DETUNE_MAX_CENTS_2 + 1> Detune::scaleFactor ;



double Detune::getDetuneFactor(int cents){
    int idx = cents + CONFIG_DETUNE_MAX_CENTS ; // shift negative values into index range
    return scaleFactor[idx] ;
}

void Detune::generate(){
    for (int i = 0; i < CONFIG_DETUNE_MAX_CENTS_2 + 1; ++i){
        int cents = i - CONFIG_DETUNE_MAX_CENTS ; 
        scaleFactor[i] = std::pow(2.0, cents / 1200.0 );
    }
}

double Detune::modulate(double value, ParameterModMap* modp){
    if ( modp->find(ModulationParameter::DETUNE_CENTS) == modp->end() ) return value * KeyboardController::getPitchbend() ;
    
    return value * KeyboardController::getPitchbend() * getDetuneFactor((*modp)[ModulationParameter::DETUNE_CENTS]);

}
