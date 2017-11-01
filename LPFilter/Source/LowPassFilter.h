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
    enum class filterType
    : int
    {
        juceDspModules = 0,
        vfLibrary = 1,
        customFilter = 2,
    };
    
    LowPassFilter (AudioProcessor& processor);
    ~LowPassFilter();
    
    void setup();
    void prepareFilter(double sampleRate, int samplesPerBlock, int filterMode);
    void process(AudioSampleBuffer& bufferToProcess, int filterMode);
    void updateFilterParameters();
    void updateFilterType();
private:
    AudioProcessor& activeProcessor;
    float frequencyParameter;
    float previousFrequency;
    
    void juceDspProcess(AudioSampleBuffer& processBuffer) noexcept;
    void vfLibraryProcess(AudioSampleBuffer& processBuffer) noexcept;
    void customFilterProcess(AudioSampleBuffer& processBuffer) noexcept;
    
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lpfJuce;
    ScopedPointer<Dsp::Filter> lpfDspLib;
    
    IIRCoefficients      iirCoef;
    Dsp::Params paramsDsp;
    
    AudioSampleBuffer previousBuffer;
    AudioSampleBuffer filteredBuffer;
};

#endif /* LowPassFilter_h */
