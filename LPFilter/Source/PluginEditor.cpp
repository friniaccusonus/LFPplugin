/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
LpfilterAudioProcessorEditor::LpfilterAudioProcessorEditor (LpfilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Add components
    
    /* Adding filter mode drop-down menu and it's label*/
    addAndMakeVisible(filterModeLabel);
    filterModeLabel.setText("Filter Type", dontSendNotification);
    filterModeLabel.attachToComponent(&filterModeBox, true);
    
    addAndMakeVisible(filterModeBox);
    String list[3] = {"JUCE", "VF", "Custom"};      // Creates the choices list
    filterModeBox.addItemList(StringArray(list, 3), 1);
    filterModeBox.setSelectedId(1);
    filterModeBox.addListener(this);
    
    /* Add frequency knob and it's label */
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", dontSendNotification);
    frequencyLabel.attachToComponent(&frequencyKnob, true);
    
    addAndMakeVisible(frequencyKnob);
    frequencyKnob.addListener(this);
    frequencyKnob.setRange(processor.frequency->range.start, processor.frequency->range.end, 1.0);
    frequencyKnob.setTextValueSuffix("Hz");
    frequencyKnob.setSkewFactorFromMidPoint(2000.0);
    
    /* Add bypass button */
    addAndMakeVisible (bypassButton);
    bypassButton.addListener(this);
    bypassButton.setButtonText(processor.bypassParam->name);
    
    updateComponents();
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    startTimerHz (30);
}

LpfilterAudioProcessorEditor::~LpfilterAudioProcessorEditor()
{
}

//==============================================================================
void LpfilterAudioProcessorEditor::buttonClicked(Button* button)
{
    // In case there is a second button, we must check wich of the buttons called this method
    if (button == &bypassButton)
    {
        *processor.bypassParam = button->getToggleState() ;
    }
}

void LpfilterAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox)
{
    if (comboBox == &filterModeBox)
    {
        *processor.mode = filterModeBox.getSelectedItemIndex();
    }
}

void LpfilterAudioProcessorEditor::sliderDragStarted (Slider* slider)
{
    if (slider == &frequencyKnob)
        processor.frequency->beginChangeGesture();
}

void LpfilterAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &frequencyKnob)
    {
        // Update text
        frequencyKnob.getTextFromValue(frequencyKnob.getValue());
        // Update the processor's frequency
        *processor.frequency = frequencyKnob.getValue();
    }
}

void LpfilterAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
    if (slider == &frequencyKnob)
        processor.frequency->endChangeGesture();
}


//==============================================================================
void LpfilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setColour (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll ();
}

void LpfilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    /* Drop-down menu */
    auto boundsToDivide = getLocalBounds().reduced(20);
    auto newBounds = boundsToDivide;
    //auto modeBoxBounds = boundsToDivide;
    boundsToDivide.removeFromRight(20);
    boundsToDivide.removeFromLeft(100);
    filterModeBox.setBounds(boundsToDivide.removeFromTop(25)); // Removes strip, reduces the rectangle and returns the strip
    
    boundsToDivide.removeFromTop(10);
    
    /* Frequency knob */
    //auto knobBounds = boundsToDivide;
    frequencyKnob.setBounds(boundsToDivide.removeFromTop(80));
    boundsToDivide.removeFromTop(10);
        
    /* Bypass button */
    auto buttonBounds = boundsToDivide.removeFromTop(20);
    bypassButton.setSize(80, buttonBounds.getHeight());   // Sets button's size
    bypassButton.setCentrePosition(newBounds.removeFromBottom(40).getCentre());
    
}

void LpfilterAudioProcessorEditor::updateComponents()
{
    // Update components during automation
    /* Bypass Button */
    const bool newButtonValue = *processor.bypassParam;
    if (newButtonValue != bypassButton.getToggleState())
        bypassButton.setToggleState( newButtonValue, dontSendNotification);
    
    /* Drop-down menu */
    auto newFilterType = processor.mode->getIndex();
    if (newFilterType != filterModeBox.getSelectedItemIndex())
        filterModeBox.setSelectedItemIndex(newFilterType);
    
    /* Frequency knob */
    auto newFrequency = processor.frequency->get();
    if (newFrequency != frequencyKnob.getValue())
        frequencyKnob.setValue(newFrequency);
    
}

void LpfilterAudioProcessorEditor::timerCallback()
{
    updateComponents();
}
