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

void LowPassFilter::filterSetup(float sampleRate, int numChannels, int numSamples)
{
    // Check for mono or stereo
    jassert(numChannels < 3);
    
    // Create RBJ filter
    lpfVfLib = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::LowPass, 2> (numSamples);
    
    // Prepare lpfJuce filter
    dsp::ProcessSpec spec {sampleRate, static_cast<uint32>(numSamples), static_cast<uint32>(numChannels)};
    lpfJuce.prepare(spec);
}

void LowPassFilter::setFilterParameters (FilterMode filterMode, int filterOrder, float cutoffFrequency, float qValue)
{
    setFilterMode(filterMode);
    setFilterOrder(filterOrder);
    setCutoffFrequency(cutoffFrequency);
    setQValue(qValue);
    
    // Set parameters for lpfVfLib
    parameters[0] = getSampleRate();
    parameters[1] = cutoffFrequency;
    parameters[2] = qValue;
    lpfVfLib->setParams(parameters);
    
    if (filterMode == FilterMode::JuceDspModules)
    {
        baseFilter.setup(fs, cutoffFrequency, qValue);
        // Get baseFilter coefficients
        getCoefficients();
        
        // Set coeffs to lpfJuce
        dsp::IIR::Coefficients<float>* stateToUse (&filterCoeffs);
        *lpfJuce.state = *stateToUse;
    }
    
}

void LowPassFilter::process(AudioSampleBuffer& bufferToProcess)
{
    switch (fMode)
    {
        case FilterMode::JuceDspModules:
        {
            juceProcess(bufferToProcess);
            break;
        }
        case FilterMode::VFLibrary:
        {
            vfProcess(bufferToProcess);
            break;
        }
        default:
            jassertfalse;
            break;
    }
}

void LowPassFilter::getCoefficients()
{
    double a0 = baseFilter.getA0 ();
    double a1 = baseFilter.getA1 ();
    double a2 = baseFilter.getA2 ();
    double b0 = baseFilter.getB0 ();
    double b1 = baseFilter.getB1 ();
    double b2 = baseFilter.getB2 ();
    filterCoeffs = dsp::IIR::Coefficients<float> (b0, b1, b2, a0, a1, a2);
}

void LowPassFilter::juceProcess (AudioSampleBuffer buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    for (int iOrder = 0; iOrder<fOrder; iOrder++)
    {
        // ! output buffer becomes input buffer !
        //lpfJuce.process (bufferToProcess);
    }
}

void LowPassFilter::vfProcess (AudioSampleBuffer buffer)
{
    for (int iOrder = 0; iOrder<fOrder; iOrder++)
    {
        //lpfVfLib->process (bufferToProcess);
    }
}

void LowPassFilter::setFilterMode(FilterMode filterMode)
{
    fMode = filterMode;
}

void LowPassFilter::setCutoffFrequency(float cutoffFrequency)
{
    fs = cutoffFrequency;
}

void LowPassFilter::setQValue(float qValue)
{
    q = qValue;
}

void LowPassFilter::setFilterOrder(int filterOrder)
{
    fOrder = filterOrder;
}

LowPassFilter::FilterMode LowPassFilter::getFilterMode()
{
    return fMode;
}

float LowPassFilter::getSampleRate()
{
    return fs;
}


