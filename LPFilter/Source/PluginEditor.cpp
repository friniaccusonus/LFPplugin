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
    addAndMakeVisible (bypassButton);
    bypassButton.addListener(this);
    bypassButton.setButtonText(processor.bypassParam->name);
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

LpfilterAudioProcessorEditor::~LpfilterAudioProcessorEditor()
{
}

//==============================================================================
void LpfilterAudioProcessorEditor::buttonClicked(Button* button)
{
    
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
    bypassButton.setSize(100, 40);
}
