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
    
    /* Add bypass button */
    addAndMakeVisible (bypassButton);
    bypassButton.addListener(this);
    bypassButton.addMouseListener(this, false);
    bypassButton.setButtonText(processor.bypassParam->name);
    
    /* Adding filter mode drop-down menu and it's label*/
    addAndMakeVisible(filterModeLabel);
    filterModeLabel.setText("Filter Type", dontSendNotification);
    filterModeLabel.attachToComponent(&filterModeBox, true);
    
    addAndMakeVisible(filterModeBox);
    filterModeBox.addItemList(processor.mode->choices, 1);
    filterModeBox.setSelectedId(processor.mode->getIndex()+1, dontSendNotification);
    filterModeBox.addListener(this);
    filterModeBox.addMouseListener(this, false);
    
    /* Add frequency knob and it's label */
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", dontSendNotification);
    frequencyLabel.attachToComponent(&frequencyKnob, true);
    
    addAndMakeVisible(frequencyKnob);
    frequencyKnob.addListener(this);
    frequencyKnob.setRange(processor.frequency->range.start, processor.frequency->range.end, 1.0);
    frequencyKnob.setTextValueSuffix("Hz");
    frequencyKnob.setSkewFactorFromMidPoint(2000.0);
    
    /* Add gain slider and it's label*/
    addAndMakeVisible(gainLabel);
    gainLabel.setText(processor.gaindB->name, dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    gainSlider.addListener(this);
    gainSlider.setRange(processor.gaindB->range.start, processor.gaindB->range.end, 1.0);
    gainSlider.setTextValueSuffix("dB");

    updateComponents();
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 250);
    
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

void LpfilterAudioProcessorEditor::mouseDown(const MouseEvent &event)
{
    if (event.eventComponent == &filterModeBox)
    {
        processor.mode->beginChangeGesture();
    }
    else if (event.eventComponent == &bypassButton)
    {
        processor.bypassParam->beginChangeGesture();
    }
}

void LpfilterAudioProcessorEditor::mouseUp(const MouseEvent &event) 
{
    if (event.eventComponent == &filterModeBox)
    {
        processor.mode->endChangeGesture();
    }
    else if (event.eventComponent == &bypassButton)
    {
        processor.bypassParam->endChangeGesture();
    }
}

void LpfilterAudioProcessorEditor::sliderDragStarted (Slider* slider)
{
    if (slider == &frequencyKnob)
    {
        processor.frequency->beginChangeGesture();
    }
    else if (slider == &gainSlider)
    {
        processor.gaindB->beginChangeGesture();
    }
}

void LpfilterAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &frequencyKnob)
    {
        // Update the processor's frequency
        *processor.frequency = frequencyKnob.getValue();
    }
    else if (slider == &gainSlider)
    {
        *processor.gaindB = gainSlider.getValue();
    }
}

void LpfilterAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
    if (slider == &frequencyKnob)
    {
        processor.frequency->endChangeGesture();
    }
    else if (slider == &gainSlider)
    {
        processor.gaindB->endChangeGesture();
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

    // Bypass button
    bypassButton.setBounds(boundsToDivide.removeFromTop(30));
    boundsToDivide.removeFromTop(15);

    boundsToDivide.removeFromRight(20);
    boundsToDivide.removeFromLeft(100);
    filterModeBox.setBounds(boundsToDivide.removeFromTop(25)); // Removes strip, reduces the rectangle and returns the strip
    
    // Gain slider
    auto gainBounds = boundsToDivide.removeFromRight(60);
    gainSlider.setBounds(gainBounds);
    
    // Drop-down menu
    filterModeBox.setBounds(boundsToDivide.removeFromTop(25));
    boundsToDivide.removeFromTop(35);
    
    // Frequency knob
    frequencyKnob.setBounds(boundsToDivide.removeFromTop(80));
}

void LpfilterAudioProcessorEditor::updateComponents()
{
    // Update components during automation (update UI from parameters)
    // Bypass Button
    const bool newButtonValue = *processor.bypassParam;
    if (newButtonValue != bypassButton.getToggleState())
        bypassButton.setToggleState( newButtonValue, dontSendNotification);
    
    // Drop down menu
    float newFilterModeValue = *processor.mode;
    if (newFilterModeValue != filterModeBox.getSelectedItemIndex())
        filterModeBox.setSelectedItemIndex(newFilterModeValue, dontSendNotification);
    
    // Frequency knob
    auto newFrequency = processor.frequency->get();
    if (newFrequency != frequencyKnob.getValue())
        frequencyKnob.setValue(newFrequency);

    // Gain slider
    auto newGain = processor.gaindB->get();
    if (newGain != gainSlider.getValue())
        gainSlider.setValue(newGain);
}

void LpfilterAudioProcessorEditor::timerCallback()
{
    updateComponents();
}
