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

class LowPassFilter : public dsp::IIR::Filter<float>,
                        public Dsp::Filter
{
public:
    LowPassFilter ();
    ~LowPassFilter();
    
    void prepareFilter(double sampleRate, int samplesPerBlock, int filterType);
    void process(AudioSampleBuffer& bufferToProcess);
    void updateFilterParameters();
    void updateFilterType();
private:
    
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lpfJuce;
    ScopedPointer<Dsp::Filter> lpfDspLib;
    
};

#endif /* LowPassFilter_h */
