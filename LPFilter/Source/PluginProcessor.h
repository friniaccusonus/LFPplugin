/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspFilters/Dsp.h"
#include "LowPassFilter.h"

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
    
    //==============================================================================
    // Parameters are public, so they can be accessed by the Editor
    AudioParameterFloat* gaindB;
    AudioParameterFloat* frequency;
    AudioParameterChoice* mode;
    AudioParameterBool* bypassParam;

    ScopedPointer<LowPassFilter> lpFilter;
private:
    

    //==============================================================================
    
    float previousFrequency;
    int filterOrder;
    float qValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LpfilterAudioProcessor)
};
