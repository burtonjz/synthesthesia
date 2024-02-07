#ifndef __COMMON_TYPES_HPP_
#define __COMMON_TYPES_HPP_

#include "config.hpp"
#include "ModulationParameter.hpp"
#include "BMap.hpp"

using ParameterModMap = BMap<ModulationParameter, double, N_MODULATION_PARAMETERS> ;

#endif // __COMMON_TYPES_HPP_