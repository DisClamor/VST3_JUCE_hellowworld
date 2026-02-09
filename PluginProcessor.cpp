// Source/PluginProcessor.cpp
#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    constexpr const char* kParamGain = "gainDb";
}

HelloGainAudioProcessor::HelloGainAudioProcessor()
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                      )
    , apvts (*this, nullptr, "PARAMS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout HelloGainAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { kParamGain, 1 },
        "Gain (dB)",
        juce::NormalisableRange<float> { -60.0f, 12.0f, 0.01f },
        0.0f
    ));

    return layout;
}

bool HelloGainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Minimal: allow mono or stereo, but input/output must match.
    const auto& in  = layouts.getMainInputChannelSet();
    const auto& out = layouts.getMainOutputChannelSet();

#if JucePlugin_IsSynth
    juce::ignoreUnused (in);
    return (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo());
#else
    if (in != out) return false;
    return (in == juce::AudioChannelSet::mono() || in == juce::AudioChannelSet::stereo());
#endif
}

void HelloGainAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    gainSmooth.reset (sampleRate, 0.02); // 20ms smoothing
    gainSmooth.setCurrentAndTargetValue (1.0f);
}

void HelloGainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);

    juce::ScopedNoDenormals noDenormals;

    const auto* gainParam = apvts.getRawParameterValue (kParamGain);
    const float gainDb = (gainParam != nullptr) ? gainParam->load() : 0.0f;

    const float targetLinear = juce::Decibels::decibelsToGain (gainDb, -100.0f);
    gainSmooth.setTargetValue (targetLinear);

    const int numChannels = buffer.getNumChannels();
    const int numSamples  = buffer.getNumSamples();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const float g = gainSmooth.getNextValue();

        for (int ch = 0; ch < numChannels; ++ch)
            buffer.setSample (ch, sample, buffer.getSample (ch, sample) * g);
    }
}

juce::AudioProcessorEditor* HelloGainAudioProcessor::createEditor()
{
    return new HelloGainAudioProcessorEditor (*this);
}

void HelloGainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    const auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void HelloGainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}
