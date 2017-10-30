//
//  LowPassFilter.h
//  LPFilter
//
//  Created by Effrosyni Paschou on 30/10/2017.
//
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#ifndef LowPassFilter_h
#define LowPassFilter_h


#endif /* LowPassFilter_h */

class LowPassFilter
{
public:
    LowPassFilter (LpfilterAudioProcessor&);
    ~LowPassFilter();
    
    void updateFilterParameters();
    void prepareFilter();
    void process(AudioSampleBuffer& bufferToProcess);
private:
    LpfilterAudioProcessor& processor;
};
