#ifndef LIMIT_HPP_
#define LIMIT_HPP_

// force value to be within valid range
template <class T> 
T limit (const T val, const T min, const T max){
    return ( val < min ? min : (val > max ? max : val));
};

#endif // LIMIT_HPP_