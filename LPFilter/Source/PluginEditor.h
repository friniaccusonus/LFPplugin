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

class KnobSlider :    public Slider
{
public:
    KnobSlider()
    {
        setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        setRotaryParameters(5*M_PI_4, 11*M_PI_4, true);
    }

    String getTextFromValue (double value) override
    {
        if (value > 999)
        {
            auto newValue = value / 1000;
            return String (newValue, 0) + " "+ "k"+getTextValueSuffix();
        }
        else
            return String (value,0) + " " + getTextValueSuffix();
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
    KnobSlider frequencyKnob;
    Slider gainSlider;
    
    Label filterModeLabel;
    Label frequencyLabel;
    Label gainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LpfilterAudioProcessorEditor)
};
