//
//  LowPassFilter.cpp
//  LPFilter
//
//  Created by Effrosyni Paschou on 30/10/2017.
//


#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(LpfilterAudioProcessor& processor)
: processor(processor)
{
    //...
}


LowPassFilter::~LowPassFilter()
{
}

void LowPassFilter::prepareFilter()
{
    //...
}

void LowPassFilter::process(AudioSampleBuffer& bufferToProcess)
{
    if (*processor.mode == 0)
    {
        // Process audio data with Juce Dsp modules
    }
    else if (*processor.mode == 1)
    {
        // Process with VF filters
    }
    else if (*processor.mode == 2)
    {
        // Process audio data with custom filter
    }
    else
        jassertfalse;
        
}

void LowPassFilter::updateFilterParameters()
{
    //...
}

