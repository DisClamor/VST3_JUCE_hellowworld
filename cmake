# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(HelloGain VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Expect JUCE as a subdirectory (e.g., git submodule) at ./JUCE
add_subdirectory(JUCE)

juce_add_plugin(HelloGain
    COMPANY_NAME "ExampleCo"
    PRODUCT_NAME "HelloGain"

    PLUGIN_MANUFACTURER_CODE ExCo
    PLUGIN_CODE HlGn

    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE

    EDITOR_WANTS_KEYBOARD_FOCUS FALSE

    FORMATS VST3
)

target_sources(HelloGain PRIVATE
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
)

target_compile_definitions(HelloGain PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

target_link_libraries(HelloGain PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra

    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
    juce::juce_recommended_lto_flags
)
