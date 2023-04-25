#ifndef ARRAY_MATH_HPP_
#define ARRAY_MATH_HPP_

// template functions to make basic elementwise math operators between arrays simpler, useful to handle any modifiers to samples that occur after panning
// updates a1 in place

#include <array>
#include <cstddef>

template<typename T, std::size_t N>
void array_add(std::array<T,N>& a1, const std::array<T,N>& a2){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] += a2[i];
    }
}
template<typename T, std::size_t N>
void array_add(std::array<T,N>& a1, const T val){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] += val;
    }
}

template<typename T, std::size_t N>
void array_subtract(std::array<T,N>& a1, const std::array<T,N>& a2){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] -= a2[i];
    }
}
template<typename T, std::size_t N>
void array_subtract(std::array<T,N>& a1, const T val){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] -= val;
    }
}

template<typename T, std::size_t N>
void array_multiply(std::array<T,N>& a1, const std::array<T,N>& a2){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] *= a2[i];
    }
}
template<typename T, std::size_t N>
void array_multiply(std::array<T,N>& a1, const T val){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] *= val;
    }
}

template<typename T, std::size_t N>
void array_divide(std::array<T,N>& a1, const std::array<T,N>& a2){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] /= a2[i];
    }
}
template<typename T, std::size_t N>
void array_divide(std::array<T,N>& a1, const T val){
    for(std::size_t i = 0; i < N; ++i){
        a1[i] /= val;
    }
}

#endif // ARRAY_MATH_HPP_