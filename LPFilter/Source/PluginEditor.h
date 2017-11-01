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

struct KnobSlider :    public Slider
{
    KnobSlider(){}
    
    String getTextFromValue (double value) override
    {
        if (value > 999)
        {
            auto newValue = value / 1000;
            return String (newValue, getNumDecimalPlacesToDisplay()) + " " +getTextValueSuffix();
        }
        else
            return String (value) + " " + getTextValueSuffix();
    }
};

class LpfilterAudioProcessorEditor  : public AudioProcessorEditor,
                                      public Timer,
                                      private Button::Listener,
                                      private ComboBox::Listener,
                                      private Slider::Listener
{
public:
    LpfilterAudioProcessorEditor (LpfilterAudioProcessor&);
    ~LpfilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked (Button*) override;
    void comboBoxChanged(ComboBox*) override;
    void sliderDragStarted (Slider*) override;
    void sliderValueChanged(Slider*) override;
    void sliderDragEnded(Slider*) override;
    void timerCallback() override;
    void updateComponents();
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LpfilterAudioProcessor& processor;
    
    ToggleButton bypassButton;
    ComboBox filterModeBox;
    ScopedPointer<KnobSlider> frequencyKnob;
    
    Label filterModeLabel;
    Label frequencyLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LpfilterAudioProcessorEditor)
};
