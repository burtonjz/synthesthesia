#ifndef __CONTROL_PORT_MANAGER_HPP_
#define __CONTROL_PORT_MANAGER_HPP_

#include "ModuleType.hpp"
#include "ParameterController.hpp"

#include <cstdint>
#include <boost/container/flat_map.hpp>


/**
 * @brief a data package containing port information 
 * 
 * @param module_type the ModuleType the port corresponds to
 * @param module_index the instance index for the module
 * @param relative_port the relative control port of this index
*/
struct PortData {
    ModuleType module_type ;
    uint8_t module_index ;
    uint32_t relative_port ;  
};

class ControlPortManager {
private:
    static uint32_t start_index_ ;
    static uint32_t n_ports_ ;
    static boost::container::flat_map<ModuleType,uint32_t> module_start_index_ ;
    static boost::container::flat_map<uint32_t, const float*> control_values_ ;

public:
    /**
     * @brief creates initial data structures
    */
    static void initialize();

    /**
     * @brief returns number of ports present in a ModuleType
     * 
     * @param m ModuleType
    */
    static uint32_t getNumModulePorts(ModuleType m);

    /**
     * @brief gets the relative port information from absolute port index
     * 
     * @param port LV2 port index
     * 
    */
    static PortData getPortData(uint32_t port) ;

    /**
     * @brief returns absolute port from PortData
     * 
     * @param port_data PortData object
    */
    static uint32_t getAbsolutePort(PortData port_data);

    /**
     * @brief connects a port to the portManager
     * 
     * @param data data pointer from LV2 Host
    */
    static void connectPort(uint32_t port, void* data);

    /**
     * @brief update all ports for a specified module
     * 
     * @param params module's parameter controller
     * @param m module type
     * @param instance module instance
     * 
    */
    static void updateModuleParameters(ParameterController* params, ModuleType m, uint32_t instance );

private:
    /**
     * @brief set a parameter value on the controller
     * 
     * @param params module's parameter controller
     * @param pt parameter type
     * @param port absolute port to update
    */
    static void setParameterValue(ParameterController* params, ParameterType pt, uint32_t port );
    
};

#endif // __CONTROL_PORT_MANAGER_HPP_