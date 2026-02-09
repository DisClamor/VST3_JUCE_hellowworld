// Source/PluginEditor.h
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class HelloGainAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit HelloGainAudioProcessorEditor (HelloGainAudioProcessor&);
    ~HelloGainAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    HelloGainAudioProcessor& processor;

    juce::Label title;
    juce::Slider gainSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelloGainAudioProcessorEditor)
};
