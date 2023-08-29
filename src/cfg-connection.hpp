#ifndef CONNECTION_CONFIG_HPP_
#define CONNECTION_CONFIG_HPP_

// note: the current connection manager uses a 6 bit data structure, none of these enume can go higher than 3.

enum ModulatableType {
    MODULATABLE_NONE,
    MODULATABLE_OSCILLATOR,
    MODULATABLE_FILTER
};

enum OscillatorConnectionPorts {
    OSC_CONNECT_AMP,
    OSC_CONNECT_FREQ,
    OSC_CONNECT_PHASE,
    OSC_CONNECT_N
};

enum FilterConnectionPorts {
    FILTER_CONNECT_FREQ,
    FILTER_CONNECT_Q,
    FILTER_CONNECT_N
};


#endif // CONNECTION_CONFIG_HPP_