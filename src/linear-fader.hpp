#ifndef LINEARFADER_HPP_
#define LINEARFADER_HPP_

#include <cstdint>

// gradually change value to prevent potential clicking
template <class T>
class LinearFader {
private:
    T dest_;
    uint32_t dist_;
    T val_;
public:
    LinearFader(const T dest):
        dest_ (dest),
        dist_ (0),
        val_ (dest)
    {}

    LinearFader():
        LinearFader(0.0)
    {}
    
    void set(const T dest, const uint32_t dist){
        dest_ = dest;
        dist_ = dist;
        if(dist == 0) val_ = dest;
    }

    T get() const {
        return val_;
    }

    void tick(){
        if (dist_ == 0) val_ = dest_;
        else {
            val_ += (dest_ - val_) * (1.0 / static_cast<double>(dist_));
            dist_ -= 1;
        }
    };
};

#endif // LINEARFADER_HPP_