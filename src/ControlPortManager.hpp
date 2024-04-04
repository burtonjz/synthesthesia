#ifndef __CONTROL_PORT_MANAGER_HPP_
#define __CONTROL_PORT_MANAGER_HPP_

#include "ModuleType.hpp"
#include "ParameterController.hpp"
#include "BMap.hpp"

#include <cstdint>



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

/**
 * @brief class passes port data from LV2 host to respective modules
*/
class ControlPortManager {
private:
    uint32_t start_index_ ;
    uint32_t n_ports_ ;
    BMap<ModuleType,uint32_t,N_MODULE_TYPES> module_start_index_ ;
    BMap<uint32_t, const float*, CONFIG_MAX_NUM_CONTROL_PORTS> control_values_ ;

public:
    static std::pair<const ParameterType*, size_t> getControlPorts(ModuleType typ);
    static std::pair<const ParameterType*, size_t> getModulatableParameters(ModuleType typ);

    /**
     * @brief default constructor
    */
    ControlPortManager();

    /**
     * @brief creates initial data structures
    */
    void initialize();

    /**
     * @brief gets the relative port information from absolute port index
     * 
     * @param port LV2 port index
     * 
    */
    PortData getPortData(uint32_t port) ;

    /**
     * @brief returns absolute port from PortData
     * 
     * @param port_data PortData object
    */
    uint32_t getAbsolutePort(PortData port_data);

    /**
     * @brief connects a port to the portManager
     * 
     * @param data data pointer from LV2 Host
    */
    void connectPort(uint32_t port, void* data);

    /**
     * @brief update all ports for a specified module
     * 
     * @param params module's parameter controller
     * @param m module type
     * @param instance module instance
     * 
    */
    void updateModuleParameters(ParameterController* params, ModuleType m, uint32_t instance );

private:
    /**
     * @brief set a parameter value on the controller
     * 
     * @param params module's parameter controller
     * @param pt parameter type
     * @param port absolute port to update
    */
    void setParameterValue(ParameterController* params, ParameterType pt, uint32_t port );
    
};

#endif // __CONTROL_PORT_MANAGER_HPP_