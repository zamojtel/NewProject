/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include <juce_gui_basics/filebrowser/juce_FileChooser.h>
//#include <juce_gui_basics/filebrowser/juce_FileBrowserComponent.h>
//#include <juce_gui_basics/filebrowser/juce_fi>
//#include <juce_gui_basics/filebrowser/juce_filechooser.h>
//FileChooserDialogBox
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
   

    m_oscillator_waveform_combobox.addItem("Sine", (int)OscillatorWaveform::SINE);
    m_oscillator_waveform_combobox.addItem("Square", (int)OscillatorWaveform::SQUARE);
    m_oscillator_waveform_combobox.addItem("Triangle", (int)OscillatorWaveform::TRIANGLE);
    m_oscillator_waveform_combobox.addItem("Saw", (int)OscillatorWaveform::SAW);
    m_oscillator_waveform_combobox.addItem("Pulse 0.1", (int)OscillatorWaveform::PULSE01);
    m_oscillator_waveform_combobox.addItem("Pulse 0.25", (int)OscillatorWaveform::PULSE025);
    m_oscillator_waveform_combobox.addItem("Sample ", (int)OscillatorWaveform::SAMPLE);
    m_oscillator_waveform_combobox.setSelectedId((int)audioProcessor.getOscillatorWaveform());

 
    setSize (400, 500);
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
    addAndMakeVisible(m_oscillator_waveform_combobox);
    addAndMakeVisible(m_load_sample_button);

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
    //combobox waveshape

    m_oscillator_waveform_combobox.setBounds(sliderLeft, 305, getWidth() - sliderLeft - 10, 20);
    m_oscillator_waveform_combobox.onChange = [this]() {
        //by zmienic fale ktora gramy 
        audioProcessor.setOscillatorWaveform((OscillatorWaveform)m_oscillator_waveform_combobox.getSelectedId());
    };

    //buttons
    m_load_sample_button.setButtonText("Load sample");
    m_load_sample_button.onClick = [this]() {
        load_sample();
    };
    m_load_sample_button.setBounds(label_left, 330, getWidth() - sliderLeft - 10, 20);

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

void NewProjectAudioProcessorEditor::load_sample() {
    //"C:\\Users\\zamoj\\OneDrive\\Pulpit\\sound.wav"
    audioProcessor.load_sample("C:\\Users\\zamoj\\OneDrive\\Pulpit\\sound.wav");
        //auto wildcardFilter = std::make_unique<WildcardFileFilter>("*.foo", String(), "Foo files");

        //auto browser = std::make_unique<FileBrowserComponent>(FileBrowserComponent::canSelectFiles,
        //    File(),
        //    wildcardFilter.get(),
        //    nullptr);

        //auto dialogBox = std::make_unique<FileChooserDialogBox>("Open some kind of file",
        //    "Please choose some kind of file that you want to open...",
        //    *browser,
        //    false,
        //    Colours::lightgrey);
        //std::vector<float> left_channel;
        //std::vector<float> right_channel;
        //
        //juce::File file("C:\\Users\\zamoj\\OneDrive\\Pulpit\\ttsMP3.com_VoiceText_2022-5-20_20_36_1.wav");
        //juce::AudioFormatManager formatManager;
        //formatManager.registerBasicFormats();
        //juce::ScopedPointer<juce::AudioFormatReader> reader = formatManager.createReaderFor(file);
        //if (reader != 0)
        //{
        //    int samples_per_channel = reader->lengthInSamples / reader->numChannels;
        //    left_channel.resize(samples_per_channel);
        //    right_channel.resize(samples_per_channel);
        //    if (reader->numChannels == 1) {
        //        float* temp = left_channel.data();
        //        reader->read(&temp, 1, 0, samples_per_channel);
        //        memcpy(right_channel.data(), temp, samples_per_channel * sizeof(float));
        //    }
        //    else {
        //        float* data[] = {left_channel.data(),right_channel.data()};
        //        reader->read(data, 2, 0, samples_per_channel*2);
        //    }
        //    
        //   /* for (size_t i = 0; i < samples_per_channel; i++) {
        //        float t = (float)i / samples_per_channel;
        //        float value = sin(200 * t * 3.14 * 2);
        //        left_channel[i] = value;
        //        right_channel[i] = value;
        //    }*/

        //    int power_of_two = 2;
        //    for (; power_of_two < samples_per_channel; power_of_two*=2) {}

        //    left_channel
        //    /*SpectrumFFT*/
        //    left_channel.resize(power_of_two);
        //    right_channel.resize(power_of_two);

        //    audioProcessor.m_sample.setSize(2,samples_per_channel);
        //    audioProcessor.m_sample.copyFrom(0, 0, left_channel.data(), samples_per_channel);
        //    audioProcessor.m_sample.copyFrom(1, 0, right_channel.data(), samples_per_channel);

        //}

}