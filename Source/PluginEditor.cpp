/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto sliderLeft = 120;
    auto label_left = 20;
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //id as enum value 
    m_vibrato_shape_combobox.addItem("Sine", (int)Waveform::SINE);
    m_vibrato_shape_combobox.addItem("Square", (int)Waveform::SQUARE);
    m_vibrato_shape_combobox.addItem("Traingle", (int)Waveform::TRIANGLE);
    m_vibrato_shape_combobox.setSelectedId(audioProcessor.getVibratoWaveform());
 
    setSize (400, 300);
    /*audioProcessor*/
    //adding sliders and labels
    addAndMakeVisible(m_attack_slider);
    addAndMakeVisible(m_attack_slider_label);
    addAndMakeVisible(m_decay_slider);
    addAndMakeVisible(m_decay_slider_label);
    addAndMakeVisible(m_sustain_slider);
    addAndMakeVisible(m_sustain_slider_label);
    addAndMakeVisible(m_release_slider);
    addAndMakeVisible(m_release_slider_label);
    addAndMakeVisible(m_vibrato_rate_slider);
    addAndMakeVisible(m_vibrato_rate_slider_label);
    addAndMakeVisible(m_vibrato_depth_slider);
    addAndMakeVisible(m_vibrato_depth_slider_label);
    addAndMakeVisible(m_vibrato_shape_combobox);
    addAndMakeVisible(m_vibrato_shape_label);

    //attack
    // 
    //in milis
    m_attack_slider.setRange(1, 10000);          // [1]
    m_attack_slider.setSkewFactor(0.1, false);
    m_attack_slider.setTextValueSuffix("attack value");     // [2]
    m_attack_slider.setBounds(sliderLeft, 25, getWidth() - sliderLeft - 10, 20);
    m_attack_slider.setValue(audioProcessor.getAttack());
    m_attack_slider.onValueChange = [this]() {
        //change of a current value
        audioProcessor.setAttack(m_attack_slider.getValue());
    };
    m_attack_slider_label.setText("Attack: ", juce::NotificationType::dontSendNotification);
    m_attack_slider_label.setBounds(label_left, 25, getWidth() - sliderLeft - 10, 20);

    //decay
    m_decay_slider.setRange(1, 10000);          // [1]
    m_decay_slider.setSkewFactor(0.1, false);
    m_decay_slider.setTextValueSuffix("decay value");     // [2]
    m_decay_slider.setValue(audioProcessor.m_decay);
    m_decay_slider.setBounds(sliderLeft, 65, getWidth() - sliderLeft - 10, 20);
    m_decay_slider.onValueChange = [this]() {
        audioProcessor.setDecay(m_decay_slider.getValue());
    };
    m_decay_slider_label.setText("Decay: ", juce::NotificationType::dontSendNotification);
    m_decay_slider_label.setBounds(label_left, 65, getWidth() - sliderLeft - 10, 20);

    //sustain
    m_sustain_slider.setRange(0, 1);          // [1]
    m_sustain_slider.setValue(audioProcessor.m_sustain);
    m_sustain_slider.setTextValueSuffix("sustain value");     // [2]
    m_sustain_slider.setBounds(sliderLeft, 105, getWidth() - sliderLeft - 10, 20);
    m_sustain_slider.onValueChange = [this]() {
        audioProcessor.setSustain(m_sustain_slider.getValue());
    };
    m_sustain_slider_label.setText("Sustain: ", juce::NotificationType::dontSendNotification);
    m_sustain_slider_label.setBounds(label_left, 105, getWidth() - sliderLeft - 10, 20);

    m_release_slider.setRange(1, 10000);          // [1]
    m_release_slider.setSkewFactor(0.1, false);
    m_release_slider.setTextValueSuffix("release value");     // [2]
    m_release_slider.setBounds(sliderLeft, 145, getWidth() - sliderLeft - 10, 20);
    m_release_slider.onValueChange = [this]() {
        audioProcessor.setRelease(m_release_slider.getValue());
    };
    m_release_slider.setValue(audioProcessor.m_release);
    m_release_slider_label.setText("release ", juce::NotificationType::dontSendNotification);
    m_release_slider_label.setBounds(label_left, 145, getWidth() - sliderLeft - 10, 20);

    //vibrato_rate
    m_vibrato_rate_slider.setRange(0.1, 10);          // [1]
    m_vibrato_rate_slider.setSkewFactor(0.1, false);
    m_vibrato_rate_slider.setTextValueSuffix("vibrato rate");     // [2]
    m_vibrato_rate_slider.setBounds(sliderLeft, 185, getWidth() - sliderLeft - 10, 20);
    m_vibrato_rate_slider.setValue(audioProcessor.getVibratoHertz());
    m_vibrato_rate_slider.onValueChange = [this]() {
        audioProcessor.setVibratoRate(m_vibrato_rate_slider.getValue());
    };
    m_vibrato_rate_slider_label.setText("Vibrato rate: ", juce::NotificationType::dontSendNotification);
    m_vibrato_rate_slider_label.setBounds(label_left, 185, getWidth() - sliderLeft - 10, 20);

    //vibrato_depth
    m_vibrato_depth_slider.setRange(0, 50);          // [1]
    m_vibrato_depth_slider.setTextValueSuffix("vibrato depth");     // [2]
    m_vibrato_depth_slider.setBounds(sliderLeft, 225, getWidth() - sliderLeft - 10, 20);
    m_vibrato_depth_slider.setValue(audioProcessor.getVibratoDepth());
    m_vibrato_depth_slider.onValueChange = [this]() {
        audioProcessor.setVibratoDepth(m_vibrato_depth_slider.getValue());
    };
    m_vibrato_depth_slider_label.setText("Vibrato depth: ", juce::NotificationType::dontSendNotification);
    m_vibrato_depth_slider_label.setBounds(label_left, 225, getWidth() - sliderLeft - 10, 20);

    m_vibrato_shape_combobox.setBounds(sliderLeft, 265, getWidth() - sliderLeft - 10, 20);
    m_vibrato_shape_combobox.onChange= [this]() {
        audioProcessor.setVibratoWaveform((Waveform)m_vibrato_shape_combobox.getSelectedId());
    };
    m_vibrato_shape_label.setText("Vibrato shape: ", juce::NotificationType::dontSendNotification);
    m_vibrato_shape_label.setBounds(label_left, 265, getWidth() - sliderLeft - 10, 20);

}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);*/
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
