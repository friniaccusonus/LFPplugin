//
//  LowPassFilter.cpp
//  LPFilter
//
//  Created by Effrosyni Paschou on 30/10/2017.
//


#include "LowPassFilter.h"

LowPassFilter::LowPassFilter()
{
    //..
}

LowPassFilter::~LowPassFilter()
{
    //..
}

void LowPassFilter::prepareFilter(double sampleRate, int samplesPerBlock, int filterType)
{
    switch (filterType) {
        case 0:
            // prepare LPF with JUCE modules
            
            break;
        case 1:
            // prepare VF
        case 2:
            // prepare custom
        default:
            jassertfalse;
            break;
    }
}

void LowPassFilter::process(AudioSampleBuffer& bufferToProcess)
{

}

void LowPassFilter::updateFilterParameters()
{
    //...
}

