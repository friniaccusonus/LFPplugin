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

void LowPassFilter::setup()
{
    
}

void LowPassFilter::prepareFilter(double sampleRate, int samplesPerBlock, int filterMode)
{
    switch (filterMode) {
            // prepare LPF with JUCE modules
        case 0:
            // Get the number of channels
            auto channels = static_cast<uint32> (getMainBusNumInputChannels());
            
            // Setup lpfJuce
            dsp::IIR::Coefficients<float>* stateToUse = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, *frequency);
            *lpfJuce.state = *stateToUse;
            // Prepare lpfJuce filter
            dsp::ProcessSpec spec {sampleRate, static_cast<uint32>(samplesPerBlock), channels};
            lpfJuce.prepare(spec);
            
            break;
            
        // prepare VF's filter
        case 1:
            //Setup filter with DSPFilters lib
            lpfDspLib = new Dsp::FilterDesign<Dsp::Butterworth::Design::LowPass <1>, 2>;
            
            // Prepare lpfDspLib filter
            paramsDsp[0] = sampleRate;              // sample rate
            paramsDsp[1] = 1;                       // order
            paramsDsp[2] = *frequency;              // cut-off frequency
            lpfDspLib->setParams(paramsDsp);
            
            filteredBuffer.setSize(2, samplesPerBlock);
        
            break;
        // prepare custom filter
        case 2:
            // Set up custom LPF coefficients
            iirCoef = IIRCoefficients::makeLowPass(sampleRate, *frequency);
            
            // Set up previous buffer for custom filter
            prevBuffer.setSize(getMainBusNumInputChannels(), samplesPerBlock);
            prevBuffer.clear();
          
        default:
            jassertfalse;
            break;
    }
    
    // Set previous frequency to current frequency
    previousFrequency = *frequency;
}

void LowPassFilter::process(AudioSampleBuffer& bufferToProcess, int filterMode)
{
    switch (filterMode) {
        case 0:
            LowPassFilter::juceDspProcess(bufferToProcess);
            break;
            
        case 1:
            LowPassFilter::vfLibraryProcess(bufferToProcess);
            break;
            
        case 2:
            LowPassFilter::customFilterProcess(bufferToProcess);
            break;
            
        default:
            jassertfalse;
            break;
    }
}

void LowPassFilter::juceDspProcess(AudioSampleBuffer& processBuffer) noexcept
{
    // Define the block that passes into juceModulesProcess function
    dsp::AudioBlock<float> block (processBuffer);
    
    // lpfJuce filter processing
    lpfJuce.process(dsp::ProcessContextReplacing<float> (block));
}

void LowPassFilter::vfLibraryProcess(AudioSampleBuffer& processBuffer) noexcept
{
    int bufferChans = processBuffer.getNumChannels(); // channels of the input buffer (1 or 2)
    int filterChans = filteredBuffer.getNumChannels(); // channels of the filtered buffer (2)
    
    // fill filteredBuffer with data from processBuffer
    for (int iChan = 0; iChan < filterChans; ++iChan)
    {
        /* left channel of tempBuffer = left channel of processBuffer
         right channel of tempBuffer = left channel of processBuffer(mono version)
         or right channel of processBuffer(stereo version)
         */
        filteredBuffer.copyFrom(iChan, 0, bufferToProcess, jmin(iChan, bufferChans - 1), 0, processBuffer.getNumSamples());
    }
    
    // process data
    lpfDspLib->process(filteredBuffer.getNumSamples(), filteredBuffer.getArrayOfWritePointers());
    
    for (int iChan = 0; iChan < bufferChans; ++iChan)
    {
        // copy data to the output buffer
        processBuffer.copyFrom(iChan, 0, filteredBuffer, iChan, 0, processBuffer.getNumSamples());
    }
}

void LowPassFilter::customFilterProcess(AudioSampleBuffer& processBuffer) noexcept
{
    for (int ch = 0; ch < getTotalNumInputChannels(); ++ch)
    {
        float* const writePtr = processBuffer.getWritePointer(ch);
        float* const prevWritePtr = prevBuffer.getWritePointer(ch);
        const float* prevReadPtr = prevBuffer.getReadPointer(ch);
        const float* readPtr = processBuffer.getReadPointer(ch);
        int lastSample = processBuffer.getNumSamples() - 1;
        
        writePtr[0] = -iirCoef.coefficients[3] * prevWritePtr[lastSample] -
        iirCoef.coefficients[4] * prevWritePtr[lastSample-1] +
        iirCoef.coefficients[0] * readPtr[0] +
        iirCoef.coefficients[1] * prevReadPtr[lastSample] +
        iirCoef.coefficients[2] * prevReadPtr[lastSample-1] ;
        
        
        writePtr[1] = -iirCoef.coefficients[3] * writePtr[0] -
        iirCoef.coefficients[4] * prevWritePtr[lastSample] +
        iirCoef.coefficients[0] * readPtr[1] +
        iirCoef.coefficients[1] * readPtr[0] +
        iirCoef.coefficients[2] * prevReadPtr[lastSample];
        
        for (int sample = 2; sample < processBuffer.getNumSamples(); ++sample)
        {
            writePtr[sample]  = -iirCoef.coefficients[3] * writePtr[sample-1] -
            iirCoef.coefficients[4] * writePtr[sample-2] +
            iirCoef.coefficients[0] * readPtr[sample] +
            iirCoef.coefficients[1] * readPtr[sample-1] +
            iirCoef.coefficients[2] * readPtr[sample-2] ;
        }
    }
    
    prevBuffer = processBuffer;
}

void LowPassFilter::updateFilterParameters()
{
    // Update lpfJuce
    *lpfJuce.state = *dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(getSampleRate(), *frequency);
    
    // Update lpfDspLib
    paramsDsp[2] = frequency->get();
    lpfDspLib->setParams(paramsDsp);
    
    // Update custom filter coeffiecients
    iirCoef = IIRCoefficients::makeLowPass(getSampleRate(), *frequency);
    
    previousFrequency = *frequency;
}

