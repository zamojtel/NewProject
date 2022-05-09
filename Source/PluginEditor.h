/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;
    // all sliders necessary to modify the current waveform
    juce::Slider m_attack_slider;
    juce::Slider m_decay_slider;
    juce::Slider m_sustain_slider;
    juce::Slider m_release_slider;
    juce::Slider m_vibrato_rate_slider;
    juce::Slider m_vibrato_depth_slider;
    juce::ComboBox m_vibrato_shape_combobox;
    juce::Label m_attack_slider_label;
    juce::Label m_decay_slider_label;
    juce::Label m_sustain_slider_label;
    juce::Label m_release_slider_label;
    juce::Label m_vibrato_rate_slider_label;
    juce::Label m_vibrato_depth_slider_label;
    juce::Label m_vibrato_shape_label;
      

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)

};
