/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DspFilters/Dsp.h"
#include "LowPassFilter.h"

//==============================================================================
LpfilterAudioProcessor::LpfilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  // Sets Input and Output bus to be stereo by default
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                  )
#endif
{
    // Add parameters
    addParameter(gain = new AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f));
    addParameter(frequency = new AudioParameterFloat("frequency", "Hz", 60.f, 10000.f, 60.f));
    addParameter(mode = new AudioParameterChoice("mode", "Mode", {"Juce DSP modules", "DSPFilters Lib", "Custom Filter"}, 0));
    addParameter(bypass = new AudioParameterBool("bypas", "Bypass", false));
    
}

LpfilterAudioProcessor::~LpfilterAudioProcessor()
{
}

//==============================================================================
const String LpfilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LpfilterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool LpfilterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool LpfilterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double LpfilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LpfilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int LpfilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LpfilterAudioProcessor::setCurrentProgram (int index)
{
}

const String LpfilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void LpfilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LpfilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //lpFilter->prepareFilter(sampleRate, samplesPerBlock, mode->getIndex());
}


void LpfilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LpfilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const AudioChannelSet& mainInput = layouts.getMainInputChannelSet();
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
    
    // This checks if the input layout matches the output layout
    if ( mainInput != mainOutput )
        return false;
    
    // Check for only mono or stereo layout
    if ( mainInput.size() > 2 )
        return false;
    
    return true;
    
}
#endif

void LpfilterAudioProcessor::processBlock (AudioSampleBuffer& ioBuffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        ioBuffer.clear (i, 0, ioBuffer.getNumSamples());
    
    
    // Update frequency parameter
    if (previousFrequency != *frequency)
        //lpFilter->updateFilterParameters();
    
    if (! *bypass)
    {
        //lpFilter->process(ioBuffer, mode->getIndex());
        
        // Apply gain
        ioBuffer.applyGain (*gain);
    }
}

//==============================================================================
bool LpfilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LpfilterAudioProcessor::createEditor()
{
    return new LpfilterAudioProcessorEditor (*this);
}


//==============================================================================
void LpfilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    XmlElement xml ("MYPLUGINSETTINGS");
    
    //Store parameter values
    for (auto* paramArray : getParameters())
    {
        if (auto* p = dynamic_cast<AudioProcessorParameterWithID*>(paramArray))
            xml.setAttribute(p->paramID, p->getValue());
    }
    
    copyXmlToBinary(xml, destData);
    
}

void LpfilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // ScopedPointer !!
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // Check if it's the same xml that was saved before
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // Restore the parameters
            for (auto* paramArray : getParameters())
            {
                if (auto* p = dynamic_cast<AudioProcessorParameterWithID*>(paramArray))
                {
                    float attributeValue = xmlState->getDoubleAttribute(p->paramID, p->getValue());
                    p->setValue(attributeValue);
                }
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LpfilterAudioProcessor();
}

