#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND "KinnuDSP"
#define DISTRHO_PLUGIN_NAME  "ducker"
#define DISTRHO_PLUGIN_URI   "https://github.com/kinnu/ducker"
//#define DISTRHO_PLUGIN_UNIQUE_ID      'd','u','c','k'

#define DISTRHO_PLUGIN_LV2_CATEGORY   "lv2:CompressorPlugin"
#define DISTRHO_PLUGIN_USES_MODGUI   0 //mod gui


enum Parameters{
    kInputGain,
    kOutLevel,
    kSidechainGain,
    kThreshold,
    kRatio,
    kAttack,
    kRelease,
    kParameterCount
};

#define DISTRHO_PLUGIN_NUM_INPUTS   3 // stereo plus side chain
#define DISTRHO_PLUGIN_NUM_OUTPUTS  2
#define DISTRHO_PLUGIN_IS_RT_SAFE   1
#define DISTRHO_PLUGIN_NUM_PARAMETERS kParameterCount

#endif