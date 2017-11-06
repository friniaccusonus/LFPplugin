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
    
    addAndMakeVisible(filterModeLabel);
    filterModeLabel.setText("Filter Type", dontSendNotification);
    filterModeLabel.attachToComponent(&filterModeBox, true);
    
    addAndMakeVisible(filterModeBox);
    filterModeBox.addItemList(processor.mode->choices, 1);
    filterModeBox.setSelectedId(processor.mode->getIndex()+1, dontSendNotification);
    filterModeBox.addListener(this);
    
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
    // Update parameters from UI
    
    if (comboBox == &filterModeBox)
    {
        int index = filterModeBox.getSelectedItemIndex(); // values = 0 ,1, 2
        
        *processor.mode = index;
        
    }
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
    
    auto boundsToDivide = getLocalBounds().reduced(20);
    auto newBounds = boundsToDivide;
    boundsToDivide.removeFromRight(20);
    boundsToDivide.removeFromLeft(100);
    filterModeBox.setBounds(boundsToDivide.removeFromTop(25)); // Removes strip, reduces the rectangle and returns the strip
    
    boundsToDivide.removeFromTop(5);
    
    auto buttonBounds = newBounds.removeFromTop(30);
    bypassButton.setSize(80, buttonBounds.getHeight());   // Sets button's size
    bypassButton.setCentrePosition(newBounds.removeFromTop(50).getCentre());
    
}

void LpfilterAudioProcessorEditor::updateComponents()
{
    // Update components during automation (update UI from parameters)
    
    const bool newButtonValue = *processor.bypassParam;
    float newFilterModeValue = *processor.mode;
    
    // Bypass Button
    if (newButtonValue != bypassButton.getToggleState())
        bypassButton.setToggleState( newButtonValue, dontSendNotification);
    
    // Drop down menu
    if (newFilterModeValue != filterModeBox.getSelectedItemIndex())
        filterModeBox.setSelectedItemIndex(newFilterModeValue, dontSendNotification);
}

void LpfilterAudioProcessorEditor::timerCallback()
{
    updateComponents();
}
