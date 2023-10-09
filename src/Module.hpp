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
    ParameterController parameterController_ ;
    const double* sampleRate_;

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

};