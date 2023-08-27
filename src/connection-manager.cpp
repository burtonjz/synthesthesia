#include "connection-manager.hpp"

#include <iostream>

uint64_t ConnectionManager::create_connection_value(ModulatableType module, int instance, int port) const {
    if(!is_connection_valid(module,instance,port)) return 0;

    uint64_t connection = static_cast<uint64_t>(module) << 4;
    connection |= static_cast<uint64_t>(instance) << 2;
    connection |= static_cast<uint64_t>(port);
    return connection;
}

bool ConnectionManager::is_connection_valid(ModulatableType module, int instance, int port) const {
    if (module < 0 || module > 3){
        std::cerr << "Invalid Module Index (" << module << "). Must be in range 0-3." << std::endl;
        return false;
    }
    if (instance < 0 || instance > 3){
        std::cerr << "Invalid Module Index (" << module << "). Must be in range 0-3." << std::endl;
        return false;
    }
    if (port < 0 || port > 3){
        std::cerr << "Invalid Module Index (" << module << "). Must be in range 0-3." << std::endl;
        return false;
    }

    return true;
}

int ConnectionManager::find_connection(ModulatableType module, int instance, int port) const {
    uint64_t connection = create_connection_value(module,instance,port);
    return find_connection(connection);
}

int ConnectionManager::find_connection(uint64_t connection) const {
    for (int i = 0; i < 55; i += 6){
        uint64_t masked_value = (data_ >> i) & 0b111111;
        if (masked_value == connection) return i;
    }

    return -1;
}

void ConnectionManager::add_connection(ModulatableType module, int instance, int port){
    uint64_t connection = create_connection_value(module,instance,port);
    add_connection(connection);
}

void ConnectionManager::add_connection(uint64_t connection){
    bool connection_added = false;

    if(find_connection(connection) == -1){
        std::cerr << "Connection already present in manager. Exiting." << std::endl; 
        return;
    }

    // find first available 6 bits to house new connection
    for (int i = 0; i < 55; i += 6){
        uint64_t masked_value = (data_ >> i) & 0b111111;
        if ( (masked_value >> 4) == MODULATABLE_NONE){
            data_ |= (connection << i);
            connection_added = true;
            break;
        }
    }

    if(!connection_added) std::cerr << "Error: Could not add new connection. Already have a max of 10 connections";
}

void ConnectionManager::remove_connection(ModulatableType module, int instance, int port){
    uint64_t connection = create_connection_value(module,instance,port);
    remove_connection(connection);
}

void ConnectionManager::remove_connection(uint64_t connection){
    int i = find_connection(connection);

    if(i == -1){
        std::cerr << "Connection specified for removal not present in manager." << std::endl;
        return;
    }

    // clear found connection
    uint64_t new_data = data_ & ~(0b111111 << i);

    // preserve last i bits, shift remaining bits
    data_ = (new_data & ((1ULL << i) - 1)) | ( new_data >> i) ;
}