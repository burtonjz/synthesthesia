#ifndef CONNECTION_MANAGER_HPP_
#define CONNECTION_MANAGER_HPP_

#include <cstdint>
#include "cfg-connection.hpp"

class ConnectionManager {
private:
    uint32_t data_;

public:
    ConnectionManager();

    /**
     * @brief type punning data to float for LV2 port value
    */
    float encode_as_float();

    /**
     * @brief type punning float from LV2 port to uint32_t data value
     * 
     * @param d data from LV2 Port
    */
    void set_data_from_float(float d);

    /**
     * @brief create the connection value from the module, instance, and port
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    uint32_t create_connection_value(ModulatableType module, int instance, int port) const;

    /**
     * @brief validate a connection. A valid connection type has a module that is not 0, and all values within their proper ranges.
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    bool is_connection_valid(ModulatableType module, int instance, int port) const;

    /**
     * @brief returns true if any connections exist
     * 
    */
    bool has_active_connections() const;

    /**
     * @brief returns index of connection within data structure (-1 if not found)
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    int find_connection(ModulatableType module, int instance, int port) const;

    /**
     * @brief returns index of connection within data structure (-1 if not found)
     * 
     * @param connection the connection value
    */
    int find_connection(uint32_t connection) const;

    /**
     * @brief add a new connection. 
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    void add_connection(ModulatableType module, int instance, int port);

    /**
     * @brief add a new connection. 
     * 
     * @param connection the connection value
    */
    void add_connection(uint32_t connection);

    /**
     * @brief remove a connection. 
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    void remove_connection(ModulatableType module, int instance, int port);

    /**
     * @brief remove a connection. 
     * 
     * @param connection the connection value
    */
    void remove_connection(uint32_t connection);
};



#endif // CONNECTION_MANAGER_HPP_