#ifndef CONNECTION_MANAGER_HPP_
#define CONNECTION_MANAGER_HPP_

#include <cstdint>
#include "connection-config.hpp"
#include "modulator.hpp"

class ConnectionManager {
private:
    uint64_t data_;

public:
    ConnectionManager();

    /**
     * @brief create the connection value from the module, instance, and port
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    uint64_t create_connection_value(ModulatableType module, int instance, int port) const;

    /**
     * @brief validate a connection. A valid connection type has a module that is not 0, and all values within their proper ranges.
     * 
     * @param module the module index for which to add the connection
     * @param instance the instance index to connect with
     * @param port the port index to connect with
    */
    bool is_connection_valid(ModulatableType module, int instance, int port) const;

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
    int find_connection(uint64_t connection) const;

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
    void add_connection(uint64_t connection);

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
    void remove_connection(uint64_t connection);
};



#endif // CONNECTION_MANAGER_HPP_