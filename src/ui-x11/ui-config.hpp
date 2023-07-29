#ifndef UI_CONFIG_HPP_
#define UI_CONFIG_HPP_

constexpr int UI_WINDOW_WIDTH = 1500;
constexpr int UI_WINDOW_HEIGHT = 1200;

// OSCILLATOR UI PARAMETERS

#define MOD_SOURCES {"Env1","Lfo1"}

constexpr int UI_OSC_WIDTH = 750;
constexpr int UI_OSC_HEIGHT = 100;

constexpr int UI_OSC_BUTTON_ON_X = 70;
constexpr int UI_OSC_BUTTON_ON_Y = 30;
constexpr int UI_OSC_BUTTON_ON_WIDTH = 40;
constexpr int UI_OSC_BUTTON_ON_HEIGHT = 40;

constexpr int UI_OSC_DIAL_GAIN_X = 155;
constexpr int UI_OSC_DIAL_GAIN_Y = 15;
constexpr int UI_OSC_DIAL_GAIN_WIDTH = 60;
constexpr int UI_OSC_DIAL_GAIN_HEIGHT = 60;

constexpr int UI_OSC_DIAL_PAN_X = 235;
constexpr int UI_OSC_DIAL_PAN_Y = 15;
constexpr int UI_OSC_DIAL_PAN_WIDTH = 60;
constexpr int UI_OSC_DIAL_PAN_HEIGHT = 60;

constexpr int UI_OSC_BOX_WAVEFORM_X = 325;
constexpr int UI_OSC_BOX_WAVEFORM_Y = 30;
constexpr int UI_OSC_BOX_WAVEFORM_WIDTH = 120;
constexpr int UI_OSC_BOX_WAVEFORM_HEIGHT = 40;

constexpr int UI_OSC_DIAL_DETUNE_SEMI_X = 780;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_Y = 15;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_WIDTH = 60;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_HEIGHT = 60;

constexpr int UI_OSC_DIAL_DETUNE_CENTS_X = 885;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_Y = 15;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_WIDTH = 60;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_HEIGHT = 60;

constexpr int UI_OSC_BOX_MOD_FREQ_X = 1070;
constexpr int UI_OSC_BOX_MOD_FREQ_Y = 30;
constexpr int UI_OSC_BOX_MOD_FREQ_WIDTH = 120;
constexpr int UI_OSC_BOX_MOD_FREQ_HEIGHT = 40;

constexpr int UI_OSC_BOX_MOD_AMP_X = 1210;
constexpr int UI_OSC_BOX_MOD_AMP_Y = 30;
constexpr int UI_OSC_BOX_MOD_AMP_WIDTH = 120;
constexpr int UI_OSC_BOX_MOD_AMP_HEIGHT = 40;

constexpr int UI_OSC_BOX_MOD_PHASE_X = 1340;
constexpr int UI_OSC_BOX_MOD_PHASE_Y = 30;
constexpr int UI_OSC_BOX_MOD_PHASE_WIDTH = 120;
constexpr int UI_OSC_BOX_MOD_PHASE_HEIGHT = 40;

// LFO UI PARAMETERS

constexpr int UI_LFO_WIDTH = 375;
constexpr int UI_LFO_HEIGHT = 100;

constexpr int UI_LFO_SLIDER_FREQ_X = 70;
constexpr int UI_LFO_SLIDER_FREQ_Y = 30;
constexpr int UI_LFO_SLIDER_FREQ_WIDTH = 120;
constexpr int UI_LFO_SLIDER_FREQ_HEIGHT = 40;

constexpr int UI_LFO_SLIDER_DEPTH_X = 240;
constexpr int UI_LFO_SLIDER_DEPTH_Y = 30;
constexpr int UI_LFO_SLIDER_DEPTH_WIDTH = 120;
constexpr int UI_LFO_SLIDER_DEPTH_HEIGHT = 40;

constexpr int UI_LFO_BOX_WAVEFORM_X = 390;
constexpr int UI_LFO_BOX_WAVEFORM_Y = 30;
constexpr int UI_LFO_BOX_WAVEFORM_WIDTH = 120;
constexpr int UI_LFO_BOX_WAVEFORM_HEIGHT = 40;

// ENVELOPE UI PARAMETERS
constexpr int UI_ENV_WIDTH = 375;
constexpr int UI_ENV_HEIGHT = 100;

constexpr int UI_ENV_SLIDER_ATTACK_X = 70;
constexpr int UI_ENV_SLIDER_ATTACK_Y = 30;
constexpr int UI_ENV_SLIDER_ATTACK_WIDTH = 120;
constexpr int UI_ENV_SLIDER_ATTACK_HEIGHT = 40;

constexpr int UI_ENV_SLIDER_DECAY_X = 180;
constexpr int UI_ENV_SLIDER_DECAY_Y = 30;
constexpr int UI_ENV_SLIDER_DECAY_WIDTH = 120;
constexpr int UI_ENV_SLIDER_DECAY_HEIGHT = 40;

constexpr int UI_ENV_SLIDER_SUSTAIN_X = 290;
constexpr int UI_ENV_SLIDER_SUSTAIN_Y = 30;
constexpr int UI_ENV_SLIDER_SUSTAIN_WIDTH = 120;
constexpr int UI_ENV_SLIDER_SUSTAIN_HEIGHT = 40;

constexpr int UI_ENV_SLIDER_RELEASE_X = 400;
constexpr int UI_ENV_SLIDER_RELEASE_Y = 30;
constexpr int UI_ENV_SLIDER_RELEASE_WIDTH = 120;
constexpr int UI_ENV_SLIDER_RELEASE_HEIGHT = 40;

// FILTER UI PARAMETERS
constexpr int UI_FLT_WIDTH = 375;
constexpr int UI_FLT_HEIGHT = 100;

constexpr int UI_FLT_BOX_TYPE_X = 70;
constexpr int UI_FLT_BOX_TYPE_Y = 30;
constexpr int UI_FLT_BOX_TYPE_WIDTH = 120;
constexpr int UI_FLT_BOX_TYPE_HEIGHT = 40;

constexpr int UI_FLT_SLIDER_CUTOFF_X = 220;
constexpr int UI_FLT_SLIDER_CUTOFF_Y = 30;
constexpr int UI_FLT_SLIDER_CUTOFF_WIDTH = 120;
constexpr int UI_FLT_SLIDER_CUTOFF_HEIGHT = 40;

constexpr int UI_FLT_DIAL_RES_X = 450;
constexpr int UI_FLT_DIAL_RES_Y = 30;
constexpr int UI_FLT_DIAL_RES_WIDTH = 40;
constexpr int UI_FLT_DIAL_RES_HEIGHT = 40;

#endif // UI_CONFIG_HPP_