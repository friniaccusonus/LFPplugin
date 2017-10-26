/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspFilters/Dsp.h"

//==============================================================================
/**
*/
class LpfilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    LpfilterAudioProcessor();
    ~LpfilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void updateParameters();

private:
    
    void juceModulesProcess (AudioSampleBuffer& buffer) noexcept;
    void dspFiltersProcess (AudioSampleBuffer& buffer) noexcept;
    void customProcess (AudioSampleBuffer& buffer) noexcept;
    
    //==============================================================================
    AudioParameterFloat* gain;
    AudioParameterFloat* frequency;
    IIRCoefficients      iirCoef;

    Dsp::Params paramsDsp;
    float defaultFreq = 60.f;
    
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lpfJuce;
    
    ScopedPointer<Dsp::Filter> lpfDspLib;
    
    AudioSampleBuffer prevBuffer;
    AudioSampleBuffer filteredBuffer;

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LpfilterAudioProcessor)
};
