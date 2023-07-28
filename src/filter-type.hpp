#ifndef FILTER_TYPE_HPP_
#define FILTER_TYPE_HPP_

enum FilterType {
    FILTER_OFF = 0,
    FILTER_LP  = 1,
    FILTER_HP  = 2,
    FILTER_BP  = 3,
    FILTER_BR  = 4,
    FILTER_N   = 5
};

#define FILTER_TYPE_STRINGS {"Low Pass","High Pass","Band Pass","Band Reject"}

#endif //FILTER_TYPE_HPP_
