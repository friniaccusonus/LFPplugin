//
//  LowPassFilter.h
//  LPFilter
//
//  Created by Effrosyni Paschou on 30/10/2017.
//

#ifndef LowPassFilter_h
#define LowPassFilter_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspFilters/Dsp.h"

class LowPassFilter
{
public:
    enum class FilterMode
    :int
    {
        JuceDspModules = 0,
        VFLibrary = 1
    };
    
    LowPassFilter();
    ~LowPassFilter();
    
    void filterSetup(FilterMode filterMode, float sampleRate, int numChannels);
    
    void setFilterMode(FilterMode filterMode);
    
    FilterMode getFilterMode();
    
    void setCutoffFrequency(float cutoffFrequency);
    
    float getCutoffFrequency();
    
    void setQFactor(float qFactor);
    
    float getQFactor();
    
    void setFilterOrder(int filterOrder);
    
    int getFilterOrder();
    
    
    /** Processing is in-place
     */
    void process(AudioSampleBuffer& buffer);
    
private:
    void calculateFilterCoefficients();
    
    int numChannels;
    FilterMode filterMode;
    int   filterOrder;
    float sampleRate;
    float cutoffFrequency;
    
    ScopedPointer<Dsp::Filter> lpfDspLib;
};


#endif /* LowPassFilter_h */
