#ifndef UI_PORT_HANDLER_HPP_
#define UI_PORT_HANDLER_HPP_

#include "ui-config.hpp"
#include "../port-info.hpp"
#include "../module-type.hpp"

#include <utility>
#include <cstdint>

/**
 * @struct PortData
 * @brief data package to describe port data for a particular frame
*/
struct PortData {
    ModuleType module_type;
    uint32_t instance;
    uint32_t index;
};

class PortHandler {
private:
    /**
     * @brief check if a port index is within a range
     * 
     * @param port_index the LV2 port index
     * @param range min and max port index values
    */
    static bool port_in_range(const uint32_t port_index, const std::pair<int,int> range);

    /**
     * @brief gets module instance and relative port index
     * 
     * @param port_index the port index relative to the start of the module port range
     * @param n_ports the number of ports in the defined module
     * 
     * @return std::pair where first is module instance and second is relative port index
    */
    static std::pair<uint32_t,uint32_t> get_relative_port(uint32_t port_index, int n_ports);

public:
    /**
     * @brief convert absolute LV2 port to the respective PortData data (which module type, instance, and relative port)
     * 
     * @param port_index the absolute LV2 port_index
    */
    static PortData get_port_data(const uint32_t port_index);

    /**
     * @brief calculates the LV2 port index
     * 
     * @param module_type module type
     * @param instance instance index of the module
     * @param relative_port port relative to the instance
    */
    static uint32_t get_port(const ModuleType module_type, const uint32_t instance, const uint32_t relative_port);
    
    

};


#endif // UI_PORT_HANDLER_HPP_