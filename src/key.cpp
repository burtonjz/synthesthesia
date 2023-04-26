#include "key.hpp"

// CONSTRUCTORS

Key::Key():
    Key (DEFAULT_SAMPLING_RATE)
{
}

Key::Key(const double rt):
    status(KEY_OFF),
    time(0.0),
    rate(rt),
    note{0},
    velocity{0},
    keyFader{1.0f},
    oscillator{},
    start_level{}
{}

Key::~Key(){
}

// GETTERS/SETTERS

KeyStatus Key::get_status() const {
    return status;
}

double Key::get_time() const {
    return time;
}

float Key::get_start_level(int index) const {
    return start_level[index];
}

// MEMBER FUNCTIONS

/* 
when a key is pressed, we initialize the oscillator objects, apply their configurations
and keep track of certain variables for any envelope modulators connected to the 
oscillator
*/
void Key::press(const std::array<OscillatorConfig,N_OSCILLATORS> osc_config,
        const uint8_t nt, const uint8_t vel){
    note = nt;
    velocity = vel;
    

    // env.set_adsr
    keyFader.set(1.0f,0.0);

    for(int i = 0; i < N_OSCILLATORS; ++i){
        oscillator[i].set_index(i);
        oscillator[i].set_key(this);
        oscillator[i].configure(osc_config[i]);
        oscillator[i].set_freq(nt);
        start_level[i] = oscillator[i].get_env_level();
    }

    time = 0.0;
    status = KEY_PRESSED;
}

/*
When a key is released, we need to update the status, but we also need to reset
the start_level and time so that envelope logic can be used to modulate oscillators
*/
void Key::release(const uint8_t nt){
    if ((status == KEY_PRESSED) && (note == nt)){
        for(int i = 0; i < N_OSCILLATORS; ++i){
            start_level[i] = oscillator[i].get_env_level();
        }
        status = KEY_RELEASED;
        time = 0.0;
    }
}

void Key::release(){
    release(note);
}

void Key::off(){
    for(int i = 0; i < N_OSCILLATORS; ++i){
        oscillator[i].set_step(0.0);
        oscillator[i].disconnect_modulators();
    }

    status = KEY_OFF;
}

void Key::mute(){
    keyFader.set(0.0f, 0.01 * oscillator[0].get_rate());
}

std::array<float,N_OSCILLATORS> Key::get_sample(){
    std::array<float,N_OSCILLATORS> sample = {};
    float level;
    
    level = (static_cast <float> (velocity) / 127.0f) * keyFader.get();
    for(int i = 0; i < N_OSCILLATORS; ++i){
        if(oscillator[i].get_is_active()){
            sample[i] = oscillator[i].get_sample() * level;
        } else sample[i] = 0.0f;
    }
    
    return sample;
}

void Key::tick(){
    bool is_off = true;
    time += 1.0 / oscillator[0].get_rate();

    for(int i = 0; i < N_OSCILLATORS; ++i){
        oscillator[i].tick();
        is_off = is_off && (time >= oscillator[i].get_release());
    }

    if ((status == KEY_RELEASED) && is_off) off();
}

bool Key::isOn(){
    return (status != KEY_OFF);
}