#ifndef __IO_HPP_
#define __IO_HPP_

#include <array>
#include <cstddef>
#include <stdexcept>

/**
 * @brief class template to store and retrieve an input or output buffer within a derived module
*/
template <typename IOType, size_t NumChannels>
class IO {
private:
    std::array<IOType*, NumChannels> IOBuffers_ ;
    
public:
    IO():
        IOBuffers_{nullptr}
    {}

    IOType* get(size_t channel) const {
        if (channel > NumChannels ) throw(std::runtime_error("IO Error: channel outside of range."));
        return IOBuffers_[channel] ;
    };

    void set(IOType* io, size_t channel){
        if (channel > NumChannels) throw(std::runtime_error("IO Error: channel outside of range."));
        IOBuffers_[channel] = io ;
    }

    void addToBuffer(IOType value, size_t index, size_t channel){
        if (channel > NumChannels) throw(std::runtime_error("IO Error: channel outside of range."));
        IOBuffers_[channel][index] += value ;
    }

};

#endif // __IO_HPP_