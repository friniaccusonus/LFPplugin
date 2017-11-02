//
//  LowPassFilter.cpp
//  LPFilter
//
//  Created by Effrosyni Paschou on 30/10/2017.
//


#include "LowPassFilter.h"

LowPassFilter::LowPassFilter()
{
    
}

LowPassFilter::~LowPassFilter()
{
    
}

void LowPassFilter::filterSetup(FilterMode filterMode, float sampleRate, int numChannels)
{
    // Check for mono or stereo
    jassert(numChannels < 3);
    switch (filterMode) {
        case FilterMode::JuceDspModules:
        {
            
            break;
        }
        case FilterMode::VFLibrary:
        {
            
            break;
        }
        default:
            jassertfalse;
            break;
    }
}

/*
dsp::IIR::Coefficients<float>* stateToUse = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequencyParameter);
*lpfJuce.state = *stateToUse;
// Prepare lpfJuce filter
dsp::ProcessSpec spec {sampleRate, static_cast<uint32>(samplesPerBlock), channels};
lpfJuce.prepare(spec);
*/
//dsp::ProcessorDuplicator<<#typename MonoProcessorType#>, <#typename StateType#>>
