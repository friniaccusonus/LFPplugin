//
//  LowPassFilter.cpp
//  LPFilter
//
//  Created by Effrosyni Paschou on 30/10/2017.
//


#include "LowPassFilter.h"
#include "DspFilters/Dsp.h"

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
    setSampleRate(sampleRate);
    setNumChannels(numChannels);
    
    // Create RBJ filter (VFLibraru)
    lpfVfLib = new Dsp::SmoothedFilterDesign<Dsp::RBJ::Design::LowPass, 2> (numSamples);
    parameters[0] = sampleRate;
    //Initialise filteredBuffer for mono processing with lpfVfLib
    filteredBuffer.setSize(2, numSamples);
    
    // Prepare lpfJuce filter
    dsp::ProcessSpec spec {sampleRate, static_cast<uint32>(numSamples), static_cast<uint32>(numChannels)};
    lpfJuce.prepare(spec);
    
    // Set up previousBuffer for custom filter
    previousBuffer.setSize(numChannels, numSamples);
    previousBuffer.clear();
}

void LowPassFilter::setFilterParameters (FilterMode filterMode, int filterOrder, float cutoffFrequency, float qValue)
{
    bool parameterChanged = false;
    // Set parameters for lpfVfLib
    if (cutoffFrequency != fc)
    {
        setCutoffFrequency(cutoffFrequency);
        parameters[1] = cutoffFrequency;
        parameterChanged = true;
    }
    
    if (qValue != q)
    {
        setQValue(qValue);
        parameters[2] = qValue;
        parameterChanged = true;
    }
    
    if (filterMode != fMode)
    {
        setFilterMode(filterMode);
        parameterChanged = true;
    }
    
    if (filterOrder != fOrder)
    {
        setFilterOrder(filterOrder);
        parameterChanged = true;
    }
    
    if (parameterChanged && (filterMode == FilterMode::VFLibrary) )
        lpfVfLib->setParams(parameters);
    
    else if (parameterChanged && (filterMode == FilterMode::JuceDspModules))
    {
        // Get baseFilter coefficients
        calculateCoefficients();
        
        // Set coefficients to lpfJuce
        dsp::IIR::Coefficients<float>* stateToUse (&filterCoeffs);
        *lpfJuce.state = *stateToUse;
    }
    else if ( parameterChanged && (filterMode == FilterMode::Custom) )
    {
        calculateCoefficients();
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
        case FilterMode::Custom:
        {
            customProcess(bufferToProcess);
            break;
        }
        default:
            jassertfalse;
            break;
    }
}

void LowPassFilter::calculateCoefficients()
{
    baseFilter.setup(fs, fc, q);
    
    double a0 = baseFilter.getA0 ();
    double a1 = baseFilter.getA1 ();
    double a2 = baseFilter.getA2 ();
    double b0 = baseFilter.getB0 ();
    double b1 = baseFilter.getB1 ();
    double b2 = baseFilter.getB2 ();
    
    if (fMode == FilterMode::JuceDspModules)
    {
        filterCoeffs = dsp::IIR::Coefficients<float> (b0, b1, b2, a0, a1, a2);
    }
    else if (fMode == FilterMode::Custom)
    {
        customCoefficients = IIRCoefficients(b0, b1, b2, a0, a1, a2);
    }
}

void LowPassFilter::juceProcess (AudioSampleBuffer& ioBuffer)
{
    dsp::AudioBlock<float> ioBlock (ioBuffer);
    //for (int iOrder = 0; iOrder < fOrder; ++iOrder)
    //{
        lpfJuce.process (dsp::ProcessContextReplacing<float> (ioBlock));
    //}
}

void LowPassFilter::vfProcess (AudioSampleBuffer& ioBuffer)
{
    int bufferChans = ioBuffer.getNumChannels(); // channels of the input buffer (1 or 2)
    int filterChans = filteredBuffer.getNumChannels(); // channels of the filtered buffer (2)
    
    // fill filteredBuffer with data from ioBuffer
    for (int iChan = 0; iChan < filterChans; ++iChan)
    {
        /* left channel of filteredBuffer = left channel of ioBuffer
         right channel of filteredBuffer = left channel of ioBuffer(mono version)
         or right channel of ioBuffer(stereo version)
         */
        filteredBuffer.copyFrom(iChan, 0, ioBuffer, jmin(iChan, bufferChans - 1), 0, ioBuffer.getNumSamples());
    }
    
    // process data
    lpfVfLib->process(filteredBuffer.getNumSamples(), filteredBuffer.getArrayOfWritePointers());
    
    for (int iChan = 0; iChan < bufferChans; ++iChan)
    {
        // copy data to the output buffer
        ioBuffer.copyFrom(iChan, 0, filteredBuffer, iChan, 0, ioBuffer.getNumSamples());
    }
    
}

void LowPassFilter::customProcess(AudioSampleBuffer& ioBuffer)
{
    double b0 = customCoefficients.coefficients[0];
    double b1 = customCoefficients.coefficients[1];
    double b2 = customCoefficients.coefficients[2];
    double a1 = customCoefficients.coefficients[3];
    double a2 = customCoefficients.coefficients[4];
    
    for (int channels = 0; channels < nChannels; ++channels)
    {
        float* const writePtr = ioBuffer.getWritePointer(channels);
        const float* readPtr = ioBuffer.getReadPointer(channels);
        float* const previousWritePtr = previousBuffer.getWritePointer(channels);
        const float* previousReadPtr = previousBuffer.getReadPointer(channels);
        
        int lastSample = previousBuffer.getNumSamples() - 1;
        
        writePtr[0] = -a1 * previousWritePtr[lastSample] -
                        a2 * previousWritePtr[lastSample-1] +
                        b0 * readPtr[0] +
                        b1 * previousReadPtr[lastSample] +
                        b2 * previousReadPtr[lastSample-1] ;
        
        writePtr[1] = -a1 * writePtr[0] -
                        a2 * previousWritePtr[lastSample] +
                        b0 * readPtr[1] +
                        b1 * readPtr[0] +
                        b2 * previousReadPtr[lastSample];
        
        for (int sample = 2; sample < ioBuffer.getNumSamples(); ++sample)
        {
            writePtr[sample]  = -a1 * writePtr[sample-1] -
                                a2 * writePtr[sample-2] +
                                b0 * readPtr[sample] +
                                b1 * readPtr[sample-1] +
                                b2 * readPtr[sample-2] ;
        }
        
    }
    
    previousBuffer = ioBuffer;
}


void LowPassFilter::setFilterMode(FilterMode filterMode)
{
    fMode = filterMode;
}

void LowPassFilter::setCutoffFrequency(float cutoffFrequency)
{
    fc = cutoffFrequency;
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

void LowPassFilter::setSampleRate(float sampleRate)
{
    fs = sampleRate;
}

float LowPassFilter::getSampleRate()
{
    return fs;
}

void LowPassFilter::setNumChannels(int numChannels)
{
    nChannels = numChannels;
}

int LowPassFilter::getNumChannels()
{
    return nChannels;
}

