/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class LpfilterAudioProcessorEditor  : public AudioProcessorEditor,
                                      public Timer,
                                      private Button::Listener
{
public:
    LpfilterAudioProcessorEditor (LpfilterAudioProcessor&);
    ~LpfilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked (Button*) override;
    void timerCallback() override;
    void updateComponents();
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LpfilterAudioProcessor& processor;
    
    ToggleButton bypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LpfilterAudioProcessorEditor)
};
