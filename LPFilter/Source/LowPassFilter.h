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
    
    
    void filterSetup( float sampleRate, int numChannels, int numSamples);
    
    void setFilterParameters (FilterMode FilterMode, int filterOrder, float cutoffFrequency, float qValue);
    
    void setFilterMode(FilterMode filterMode);
    
    FilterMode getFilterMode();
    
    void setCutoffFrequency(float cutoffFrequency);
    
    float getCutoffFrequency();
    
    void setQValue(float qValue);
    
    float getQValue();
    
    void setFilterOrder(int filterOrder);
    
    int getFilterOrder();
    
    void setSampleRate(float sampleRate);
    
    float getSampleRate();
    

    /** Processing is in-place
     */
    void process(AudioSampleBuffer& bufferToProcess);
    
private:

    void getCoefficients();
    void juceProcess (AudioSampleBuffer buffer);
    void vfProcess (AudioSampleBuffer buffer);
    
    int numChannels;
    FilterMode fMode;
    int   fOrder;
    float fs;
    float fc;
    float q;
    dsp::IIR::Coefficients<float> filterCoeffs;
    Dsp::Params parameters;
    
    Dsp::RBJ::LowPass baseFilter;
    ScopedPointer<Dsp::Filter> lpfVfLib;
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lpfJuce;
};


#endif /* LowPassFilter_h */

