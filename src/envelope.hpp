#ifndef ENVELOPE_HPP_
#define ENVELOPE_HPP_

#if LOG_LEVEL > 0
    #include "logger.hpp"
#endif

#include "key-status.hpp"
#include "modulator-env.hpp"

#ifndef DEFAULT_ENV_ASDR
#define DEFAULT_ENV_ASDR
#define DEFAULT_ENV_ATTACK 0.0
#define DEFAULT_ENV_DECAY 0.0
#define DEFAULT_ENV_SUSTAIN 0.0
#define DEFAULT_ENV_RELEASE 0.0
#endif


// struct ADSR{
//     double attack;
//     double decay;
//     float sustain;
//     double release;
// };

class Envelope {
private:
    ADSR adsr;
    double time;
    float level; // keep track of the level through the iterative calls to get_level
    KeyStatus last_status; // trigger level variable when status switches

    #if LOG_LEVEL == 1
        Logger<8> logger;
    #endif
public:
    Envelope(ADSR adsr, double t);
    Envelope(ADSR adsr);
    Envelope();
    
    // GETTERS/SETTERS
    void set_adsr(ADSR a){
        adsr = a;
    }

    KeyStatus get_last_status(){
        return last_status;
    }

    double get_release(){
        return adsr.release;
    }

    double get_time(){
        return time;
    }

    void set_time(double t){
        time = t;
    }
    
    float get_level(KeyStatus status);

    void reset();

    void tick(double rate);

};

// CONSTRUCTORS
Envelope::Envelope(ADSR adsr, double t):
    time(t),
    level(0.0f),
    last_status(KEY_OFF)
{
    set_adsr(adsr);
};

Envelope::Envelope(ADSR adsr):
    Envelope::Envelope(adsr,0.0)
{
};

Envelope::Envelope():
    Envelope::Envelope({DEFAULT_ENV_ATTACK,DEFAULT_ENV_DECAY,DEFAULT_ENV_SUSTAIN,DEFAULT_ENV_RELEASE})
{
};

// return the output level as determined by the envelope
float Envelope::get_level(KeyStatus status){
    switch(status){
    case KEY_PRESSED:
        if(last_status != status){
            level = get_level(last_status);
            last_status = status;
            time = 0.0;
        }

        if (time < adsr.attack){    
            return level + (1.0f - level) * time / adsr.attack;
        }

        // decay
        if (time < adsr.attack + adsr.decay){
            return 1.0f + (adsr.sustain - 1.0f) * (time - adsr.attack) / adsr.decay;
        }

        // sustain
        return adsr.sustain;

    case KEY_RELEASED:
        if(last_status != status){
            level = get_level(last_status);
            last_status = status;
            time = 0.0;
        }
        return level - level * time / adsr.release;
    
    default:
        last_status = status;
        level = 0.0f;
        time = 0.0;
        return level;
    }
};

void Envelope::reset(){
    time = 0.0;
    level = 0.0;
    last_status = KEY_OFF;
}
void Envelope::tick(double rate){
    #if LOG_LEVEL > 0
        std::string msg =  "[ADSREnvelope] generated level {'KeyStatus':" + std::to_string(last_status) +
                                                         ", 'state_time':" + std::to_string(time) +
                                                         ", 'start_level':" + std::to_string(level) +
                                                         ", 'output_level':" + std::to_string(get_level(last_status)) +
                                                         ", 'attack':" + std::to_string(adsr.attack) +
                                                         ", 'decay':" + std::to_string(adsr.decay) +
                                                         ", 'sustain':" + std::to_string(adsr.sustain) +
                                                         ", 'release':" + std::to_string(adsr.release) +
                                                         "}";
            logger.log(msg);
    #endif
    time += 1.0 / rate;
};

#endif // ENVELOPE_HPP_