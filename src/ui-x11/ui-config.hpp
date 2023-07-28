#ifndef UI_CONFIG_HPP_
#define UI_CONFIG_HPP_

constexpr int UI_WINDOW_WIDTH = 750;
constexpr int UI_WINDOW_HEIGHT = 600;

// OSCILLATOR UI PARAMETERS

#define MOD_SOURCES {"Env1","Lfo1"}

constexpr int UI_OSC_WIDTH = 750;
constexpr int UI_OSC_HEIGHT = 50;

constexpr int UI_OSC_BUTTON_ON_X = 35;
constexpr int UI_OSC_BUTTON_ON_Y = 15;
constexpr int UI_OSC_BUTTON_ON_WIDTH = 20;
constexpr int UI_OSC_BUTTON_ON_HEIGHT = 20;

constexpr int UI_OSC_DIAL_GAIN_X = 80;
constexpr int UI_OSC_DIAL_GAIN_Y = 15;
constexpr int UI_OSC_DIAL_GAIN_WIDTH = 20;
constexpr int UI_OSC_DIAL_GAIN_HEIGHT = 20;

constexpr int UI_OSC_DIAL_PAN_X = 120;
constexpr int UI_OSC_DIAL_PAN_Y = 15;
constexpr int UI_OSC_DIAL_PAN_WIDTH = 20;
constexpr int UI_OSC_DIAL_PAN_HEIGHT = 20;

constexpr int UI_OSC_BOX_WAVEFORM_X = 165;
constexpr int UI_OSC_BOX_WAVEFORM_Y = 15;
constexpr int UI_OSC_BOX_WAVEFORM_WIDTH = 60;
constexpr int UI_OSC_BOX_WAVEFORM_HEIGHT = 20;

constexpr int UI_OSC_DIAL_DETUNE_SEMI_X = 385;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_Y = 15;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_WIDTH = 20;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_HEIGHT = 20;

constexpr int UI_OSC_DIAL_DETUNE_CENTS_X = 440;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_Y = 15;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_WIDTH = 20;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_HEIGHT = 20;

constexpr int UI_OSC_BOX_MOD_FREQ_X = 530;
constexpr int UI_OSC_BOX_MOD_FREQ_Y = 15;
constexpr int UI_OSC_BOX_MOD_FREQ_WIDTH = 20;
constexpr int UI_OSC_BOX_MOD_FREQ_HEIGHT = 20;

constexpr int UI_OSC_BOX_MOD_AMP_X = 605;
constexpr int UI_OSC_BOX_MOD_AMP_Y = 15;
constexpr int UI_OSC_BOX_MOD_AMP_WIDTH = 20;
constexpr int UI_OSC_BOX_MOD_AMP_HEIGHT = 20;

constexpr int UI_OSC_BOX_MOD_PHASE_X = 670;
constexpr int UI_OSC_BOX_MOD_PHASE_Y = 15;
constexpr int UI_OSC_BOX_MOD_PHASE_WIDTH = 20;
constexpr int UI_OSC_BOX_MOD_PHASE_HEIGHT = 20;

// LFO UI PARAMETERS

constexpr int UI_LFO_WIDTH = 375;
constexpr int UI_LFO_HEIGHT = 50;

constexpr int UI_LFO_SLIDER_FREQ_X = 35;
constexpr int UI_LFO_SLIDER_FREQ_Y = 15;
constexpr int UI_LFO_SLIDER_FREQ_WIDTH = 60;
constexpr int UI_LFO_SLIDER_FREQ_HEIGHT = 20;

constexpr int UI_LFO_SLIDER_DEPTH_X = 90;
constexpr int UI_LFO_SLIDER_DEPTH_Y = 15;
constexpr int UI_LFO_SLIDER_DEPTH_WIDTH = 60;
constexpr int UI_LFO_SLIDER_DEPTH_HEIGHT = 20;

constexpr int UI_LFO_BOX_WAVEFORM_X = 145;
constexpr int UI_LFO_BOX_WAVEFORM_Y = 15;
constexpr int UI_LFO_BOX_WAVEFORM_WIDTH = 60;
constexpr int UI_LFO_BOX_WAVEFORM_HEIGHT = 20;

// ENVELOPE UI PARAMETERS
constexpr int UI_ENV_WIDTH = 375;
constexpr int UI_ENV_HEIGHT = 50;

constexpr int UI_ENV_SLIDER_ATTACK_X = 35;
constexpr int UI_ENV_SLIDER_ATTACK_Y = 15;
constexpr int UI_ENV_SLIDER_ATTACK_WIDTH = 60;
constexpr int UI_ENV_SLIDER_ATTACK_HEIGHT = 20;

constexpr int UI_ENV_SLIDER_DECAY_X = 90;
constexpr int UI_ENV_SLIDER_DECAY_Y = 15;
constexpr int UI_ENV_SLIDER_DECAY_WIDTH = 60;
constexpr int UI_ENV_SLIDER_DECAY_HEIGHT = 20;

constexpr int UI_ENV_SLIDER_SUSTAIN_X = 145;
constexpr int UI_ENV_SLIDER_SUSTAIN_Y = 15;
constexpr int UI_ENV_SLIDER_SUSTAIN_WIDTH = 60;
constexpr int UI_ENV_SLIDER_SUSTAIN_HEIGHT = 20;

constexpr int UI_ENV_SLIDER_RELEASE_X = 200;
constexpr int UI_ENV_SLIDER_RELEASE_Y = 15;
constexpr int UI_ENV_SLIDER_RELEASE_WIDTH = 60;
constexpr int UI_ENV_SLIDER_RELEASE_HEIGHT = 20;

// FILTER UI PARAMETERS
constexpr int UI_FLT_WIDTH = 375;
constexpr int UI_FLT_HEIGHT = 50;

constexpr int UI_FLT_BOX_TYPE_X = 35;
constexpr int UI_FLT_BOX_TYPE_Y = 15;
constexpr int UI_FLT_BOX_TYPE_WIDTH = 60;
constexpr int UI_FLT_BOX_TYPE_HEIGHT = 20;

constexpr int UI_FLT_SLIDER_CUTOFF_X = 110;
constexpr int UI_FLT_SLIDER_CUTOFF_Y = 15;
constexpr int UI_FLT_SLIDER_CUTOFF_WIDTH = 60;
constexpr int UI_FLT_SLIDER_CUTOFF_HEIGHT = 20;

constexpr int UI_FLT_DIAL_RES_X = 225;
constexpr int UI_FLT_DIAL_RES_Y = 15;
constexpr int UI_FLT_DIAL_RES_WIDTH = 20;
constexpr int UI_FLT_DIAL_RES_HEIGHT = 20;

#endif // UI_CONFIG_HPP_