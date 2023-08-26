#ifndef UI_CONFIG_HPP_
#define UI_CONFIG_HPP_

constexpr int UI_WINDOW_WIDTH = 2250;
constexpr int UI_WINDOW_HEIGHT = 1800;
constexpr int UI_MODULE_HEIGHT = 150;

// OSCILLATOR UI PARAMETERS

#define MOD_SOURCES {"None","Env1","Lfo1"}

constexpr int UI_OSC_WIDTH = 2250;
constexpr int UI_OSC_HEIGHT = 150;

constexpr const char* UI_OSC_LABEL_TEXT = "OSC";



constexpr int UI_OSC_SWITCH_ON_X = 80;
constexpr int UI_OSC_SWITCH_ON_Y = 45;
constexpr int UI_OSC_SWITCH_ON_WIDTH = 80;
constexpr int UI_OSC_SWITCH_ON_HEIGHT = 40;

constexpr int UI_OSC_DIAL_GAIN_X = 220;
constexpr int UI_OSC_DIAL_GAIN_Y = 23;
constexpr int UI_OSC_DIAL_GAIN_WIDTH = 90;
constexpr int UI_OSC_DIAL_GAIN_HEIGHT = 90;

constexpr int UI_OSC_DIAL_PAN_X = 350;
constexpr int UI_OSC_DIAL_PAN_Y = 23;
constexpr int UI_OSC_DIAL_PAN_WIDTH = 90;
constexpr int UI_OSC_DIAL_PAN_HEIGHT = 90;

constexpr int UI_OSC_BOX_WAVEFORM_X = 488;
constexpr int UI_OSC_BOX_WAVEFORM_Y = 45;
constexpr int UI_OSC_BOX_WAVEFORM_WIDTH = 160;
constexpr int UI_OSC_BOX_WAVEFORM_HEIGHT = 40;

constexpr int UI_OSC_DIAL_DETUNE_SEMI_X = 1155;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_Y = 23;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_WIDTH = 90;
constexpr int UI_OSC_DIAL_DETUNE_SEMI_HEIGHT = 90;

constexpr int UI_OSC_DIAL_DETUNE_CENTS_X = 1328;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_Y = 23;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_WIDTH = 90;
constexpr int UI_OSC_DIAL_DETUNE_CENTS_HEIGHT = 90;

constexpr int UI_OSC_BOX_MOD_FREQ_X = 1605;
constexpr int UI_OSC_BOX_MOD_FREQ_Y = 45;
constexpr int UI_OSC_BOX_MOD_FREQ_WIDTH = 160;
constexpr int UI_OSC_BOX_MOD_FREQ_HEIGHT = 40;

constexpr int UI_OSC_BOX_MOD_AMP_X = 1815;
constexpr int UI_OSC_BOX_MOD_AMP_Y = 45;
constexpr int UI_OSC_BOX_MOD_AMP_WIDTH = 160;
constexpr int UI_OSC_BOX_MOD_AMP_HEIGHT = 40;

constexpr int UI_OSC_BOX_MOD_PHASE_X = 2010;
constexpr int UI_OSC_BOX_MOD_PHASE_Y = 45;
constexpr int UI_OSC_BOX_MOD_PHASE_WIDTH = 160;
constexpr int UI_OSC_BOX_MOD_PHASE_HEIGHT = 40;

// LFO UI PARAMETERS

constexpr int UI_LFO_WIDTH = 1125;
constexpr int UI_LFO_HEIGHT = 150;

constexpr int UI_LFO_SLIDER_FREQ_X = 100;
constexpr int UI_LFO_SLIDER_FREQ_Y = 45;
constexpr int UI_LFO_SLIDER_FREQ_WIDTH = 160;
constexpr int UI_LFO_SLIDER_FREQ_HEIGHT = 60;

constexpr int UI_LFO_SLIDER_DEPTH_X = 270;
constexpr int UI_LFO_SLIDER_DEPTH_Y = 45;
constexpr int UI_LFO_SLIDER_DEPTH_WIDTH = 160;
constexpr int UI_LFO_SLIDER_DEPTH_HEIGHT = 60;

constexpr int UI_LFO_BOX_WAVEFORM_X = 425;
constexpr int UI_LFO_BOX_WAVEFORM_Y = 55;
constexpr int UI_LFO_BOX_WAVEFORM_WIDTH = 160;
constexpr int UI_LFO_BOX_WAVEFORM_HEIGHT = 40;

// ENVELOPE UI PARAMETERS
constexpr int UI_ENV_WIDTH = 1125;
constexpr int UI_ENV_HEIGHT = 150;

constexpr int UI_ENV_SLIDER_ATTACK_X = 95;
constexpr int UI_ENV_SLIDER_ATTACK_Y = 45;
constexpr int UI_ENV_SLIDER_ATTACK_WIDTH = 160;
constexpr int UI_ENV_SLIDER_ATTACK_HEIGHT = 60;

constexpr int UI_ENV_SLIDER_DECAY_X = 265;
constexpr int UI_ENV_SLIDER_DECAY_Y = 45;
constexpr int UI_ENV_SLIDER_DECAY_WIDTH = 160;
constexpr int UI_ENV_SLIDER_DECAY_HEIGHT = 60;

constexpr int UI_ENV_SLIDER_SUSTAIN_X = 415;
constexpr int UI_ENV_SLIDER_SUSTAIN_Y = 45;
constexpr int UI_ENV_SLIDER_SUSTAIN_WIDTH = 160;
constexpr int UI_ENV_SLIDER_SUSTAIN_HEIGHT = 60;

constexpr int UI_ENV_SLIDER_RELEASE_X = 600;
constexpr int UI_ENV_SLIDER_RELEASE_Y = 45;
constexpr int UI_ENV_SLIDER_RELEASE_WIDTH = 160;
constexpr int UI_ENV_SLIDER_RELEASE_HEIGHT = 60;

// FILTER UI PARAMETERS
constexpr int UI_FLT_WIDTH = 1125;
constexpr int UI_FLT_HEIGHT = 150;

constexpr int UI_FLT_BOX_TYPE_X = 100;
constexpr int UI_FLT_BOX_TYPE_Y = 45;
constexpr int UI_FLT_BOX_TYPE_WIDTH = 160;
constexpr int UI_FLT_BOX_TYPE_HEIGHT = 40;

constexpr int UI_FLT_SLIDER_CUTOFF_X = 270;
constexpr int UI_FLT_SLIDER_CUTOFF_Y = 30;
constexpr int UI_FLT_SLIDER_CUTOFF_WIDTH = 160;
constexpr int UI_FLT_SLIDER_CUTOFF_HEIGHT = 60;

constexpr int UI_FLT_DIAL_RES_X = 460;
constexpr int UI_FLT_DIAL_RES_Y = 23;
constexpr int UI_FLT_DIAL_RES_WIDTH = 90;
constexpr int UI_FLT_DIAL_RES_HEIGHT = 90;

#endif // UI_CONFIG_HPP_