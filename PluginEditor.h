// Source/PluginEditor.cpp
#include "PluginEditor.h"

namespace
{
    constexpr const char* kParamGain = "gainDb";
}

HelloGainAudioProcessorEditor::HelloGainAudioProcessorEditor (HelloGainAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , processor (p)
{
    title.setText ("HelloGain", juce::dontSendNotification);
    title.setJustificationType (juce::Justification::centred);
    title.setFont (juce::Font (18.0f, juce::Font::bold));
    addAndMakeVisible (title);

    gainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 22);
    gainSlider.setRange (-60.0, 12.0, 0.01);
    gainSlider.setSkewFactorFromMidPoint (-12.0); // feels nicer for gain
    gainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (gainSlider);

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, kParamGain, gainSlider);

    setSize (240, 180);
}

void HelloGainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setColour (juce::Colours::white.withAlpha (0.9f));
    g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (10.0f), 10.0f, 1.5f);

    g.setColour (juce::Colours::white.withAlpha (0.7f));
    g.setFont (juce::Font (12.0f));
    g.drawFittedText ("A minimal gain VST3.\nTurn the knob, audio gets louder/quieter.",
                      getLocalBounds().reduced (16).withTrimmedTop (120),
                      juce::Justification::centred, 2);
}

void HelloGainAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (16);

    title.setBounds (area.removeFromTop (28));
    area.removeFromTop (6);

    auto knobArea = area.removeFromTop (88);
    gainSlider.setBounds (knobArea.withSizeKeepingCentre (100, 100));
}
