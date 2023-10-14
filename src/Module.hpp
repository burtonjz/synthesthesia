#ifndef __MODULE_HPP_
#define __MODULE_HPP_

#include "config.hpp"
#include "ParameterController.hpp"

#include <array>

/**
 * @brief the base class for all synthesizer modules
 * 
 * This class provides the framework for module instantiation and common methods/features 
 * that all modules may use.
*/

class Module {
protected:
    const double* sampleRate_;
    ParameterController parameterController_ ;

public:
    /**
     * @brief Module constructor
    */
    Module(const double* sampleRate):
        sampleRate_(sampleRate),
        parameterController_()
    {}

    /**
     * @brief Activate the module. Must be called before real-time processing begins
     * 
     * Modules override this virtual function to perform initialization and setup of the
     * module.
    */
    virtual void activate() = 0 ;

    /**
     * @brief set output buffer for channel. All modules must have at least one output
    */
    virtual void setOutputBuffer(float* buffer, size_t channel) = 0;

    /**
     * @brief returns a read-only pointer to the module's sample rate
    */
    const double* getSampleRate(){
        return sampleRate_ ;
    }

    /**
     * @brief sets the pointer for the module's sample rate
    */
    void setSampleRate(const double* sampleRate){
        sampleRate_ = sampleRate ;
    }

    /**
     * @brief get a pointer to the parameterController
    */
    ParameterController* getParameterController(){
        return &parameterController_ ;
    }

    /**
     * @brief increment module for next sample
    */
    virtual void tick() = 0 ;

};

#endif // __MODULE_HPP_