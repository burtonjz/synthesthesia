#ifndef __MODULE_HPP_
#define __MODULE_HPP_

#include "config.hpp"
#include "ModuleType.hpp"
#include "ParameterType.hpp"
#include "ParameterController.hpp"

#include <utility>

/**
 * @brief the base class for all synthesizer modules
 * 
 * This class provides the framework for module instantiation and common methods/features 
 * that all modules may use.
*/

class Module {
protected: // TODO: set all to private I think.
    ParameterController parameterController_ ;
    const double* sampleRate_ ;
    ModuleType type_ ;
    int instance_ ;

public:
    /**
     * @brief Module constructor
    */
    Module(const double* sampleRate, ModuleType typ, int instance):
        parameterController_(),
        sampleRate_(sampleRate),
        type_(typ),
        instance_(instance)
    {}

    Module(ModuleType typ):
        Module(nullptr,typ,-1)
    {}

    virtual ~Module() = default ;

    /**
     * @brief Activate the module. Must be called before real-time processing begins
     * 
     * Modules override this virtual function to perform initialization and setup of the
     * module.
    */
    // virtual void activate() = 0 ;

    /**
     * @brief returns a read-only pointer to the module's sample rate
    */
    const double* getSampleRate() const {
        return sampleRate_ ;
    }

    /**
     * @brief sets the pointer for the module's sample rate
    */
    void setSampleRate(const double* sampleRate){
        sampleRate_ = sampleRate ;
    }

    /**
     * @brief get the module type
    */
    const ModuleType getType() const {
        return type_ ;
    }

    /**
     * @brief get the module instance
    */
    const int getInstance() const {
        return instance_ ;
    }

    /**
     * @brief set the instance for the module
    */
    void setInstance(const int instance){
        instance_ = instance ;
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
    virtual void tick(){
        // TODO: call parameterController_.modulate() here then call Module::tick() in derived classes to capture it.
    }

};

#endif // __MODULE_HPP_