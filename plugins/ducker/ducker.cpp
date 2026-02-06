#include "DistrhoPlugin.hpp"
#include "DistrhoPluginInfo.h"
#include "env_follower.h"

START_NAMESPACE_DISTRHO

class ducker : public Plugin {
    public:
        ducker() : Plugin(kParameterCount, 0, 0), 
                   InputGain(1.0f),
                    OutLevel(1.0f),
                    SidechainGain(1.0f),
                    Threshold(1.0f),
                    Ratio(4.0f),
                    Attack(5.0f),
                    Release(50.0f),
                    currentEnvLevel(0.0f)

    {
        //constructor body
        sampleRateChanged(getSampleRate());
    }
  
    void sampleRateChanged(double newSampleRate) override {
        envelope.setSampleRate(newSampleRate);
    }

    protected:

    const char *getLabel() const override { return "ducker"; }
    const char *getDescription() const override {return "Simple ducker plugin.";}
    const char *getMaker() const override { return "KinnuDSP"; }
    const char *getLicense() const override { return "MIT"; }

    uint32_t getVersion() const override { return d_version(1,0,0); }
    int64_t getUniqueId() const override { return d_cconst('d','u','c','k'); }

            void initParameter (uint32_t index, Parameter& parameter) override {
        
        // this allows mod devices to map parameters
        parameter.hints = kParameterIsAutomatable;

        switch (index) {
            case kInputGain:
                parameter.name = "Input Gain";
                parameter.symbol = "gain";
                parameter.ranges.def = 1.0f;
                parameter.ranges.min = 0.0f;
                parameter.ranges.max = 2.0f;
                break;
            case  kOutLevel:
                parameter.name = "Output Gain";
                parameter.symbol = "level";
                parameter.ranges.def = 1.0f;
                parameter.ranges.min = 0.0f;
                parameter.ranges.max = 2.0f;
                break;
            case  kSidechainGain:
                parameter.name = "Sidechain Level";
                parameter.symbol = "SC_level";
                parameter.ranges.def = 1.0f;
                parameter.ranges.min = 0.0f;
                parameter.ranges.max = 4.0f;
                break;
            case kThreshold:
                parameter.name = "Threshold";
                parameter.symbol = "threshold";
                parameter.unit = "dB";
                parameter.ranges.def = -10.0f;
                parameter.ranges.min = -60.0f;
                parameter.ranges.max = 0.0f;
                break;
            case kRatio:
                parameter.name = "Ratio";
                parameter.symbol = "ratio";
                parameter.ranges.def = 4.0f;
                parameter.ranges.min = 1.0f;
                parameter.ranges.max = 20.0f;
                break;
            case kAttack:
                parameter.name = "Attack";
                parameter.symbol = "attack";
                parameter.unit = "ms";
                parameter.ranges.def = 10.0f;
                parameter.ranges.min = 1.0f;
                parameter.ranges.max = 150.0f;
                break;
            case kRelease:
                parameter.name = "Release";
                parameter.symbol = "release";
                parameter.unit = "ms";
                parameter.ranges.def = 10.0f;
                parameter.ranges.min = 1.0f;
                parameter.ranges.max = 1000.0f;
                break;
            default:
                break;
        }
    }

    float getParameterValue(uint32_t index) const override {
        switch (index) {
            case kInputGain:     return InputGain;
            case kOutLevel:      return OutLevel;
            case kSidechainGain: return SidechainGain;
            case kThreshold:     return Threshold;
            case kRatio:         return Ratio;
            case kAttack:        return Attack;
            case kRelease:       return Release;
            default: return 0.0f;
        }
    }
    
    void setParameterValue(uint32_t index, float value) override {
        switch (index) {
            case kInputGain:     InputGain = value; break;
            case kOutLevel:      OutLevel = value; break;
            case kSidechainGain: SidechainGain = value; break;
            case kThreshold:     Threshold = value; break;
            case kRatio:         Ratio = value; break;
            
            case kAttack:        Attack = value; 
            envelope.setAttack(Attack); 
            break;
                
            case kRelease:         Release = value; 
            envelope.setRelease(Release); 
            break;
        }
    
    }

void run(const float **inputs, float **outputs, uint32_t frames) override {
            
            const float *inL = inputs[0];
            const float *inR = inputs[1];
            // Sidechain signal, if sidechain not assigned use input 2 (R)
            const float *sidechain = (DISTRHO_PLUGIN_NUM_INPUTS > 2) ? inputs[2] : inputs[1];

            // convert threshold from float
            float linearThreshold = std::pow(10.0f, Threshold / 20.0f);

            float *outL = outputs[0];
            float *outR = outputs[1];



            for (uint32_t i = 0; i < frames; i++) {
                
   
                envelope.run( std::abs(sidechain[i] * SidechainGain), currentEnvLevel );
                float envLevel = currentEnvLevel;



                // Alustetaan gain 1.0:aan (eli oletuksena ääni menee täysillä läpi)
                float gainReduction = 1.0f; 

  
                if (envLevel > linearThreshold)
                {
                    float overshot = envLevel - linearThreshold; 
                    gainReduction = 1.0 - (overshot * (1.0 -(1.0/ Ratio)));
                    if (gainReduction < 0.0f) gainReduction = 0.0f;
                }

                outL[i] = inL[i] * InputGain * gainReduction * OutLevel;
                outR[i] = inR[i] * InputGain * gainReduction * OutLevel; 
            }
    }
 


    private:
        float InputGain;
        float OutLevel;
        float SidechainGain;
        float Threshold;
        float Ratio;
        float Attack;
        float Release;

    AttRelEnvelope envelope;
    double currentEnvLevel;
    
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ducker);

};

Plugin *createPlugin() { return new ducker(); }


END_NAMESPACE_DISTRHO
