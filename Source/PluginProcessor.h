/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Waveform.h"
#include "LowFrequencyOscillator.h"
#include <vector>
#include "Wavetable.h"
#include "OscillatorWaveform.h"

//==============================================================================
/**
*/
const int MIDI_KEY_COUNT = 128;
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void setAttack(float value);
    void setDecay(float value);
    void setSustain(float value);
    void setRelease(float value);
    void setVibratoRate(float hertz);
    void setVibratoDepth(float cents);
    void setVibratoWaveform(Waveform wave);
    Waveform getVibratoWaveform();
    float getAttack();
    float getVibratoHertz() const;
    float getVibratoDepth() const;
    void setOscillatorWaveform(OscillatorWaveform waveform);
    void generate_wavetables();
    OscillatorWaveform getOscillatorWaveform() const;
    //Vector na próbki fali w efekcie vibrato 
    std::vector<float> m_vibrato_waveform_samples;
    //general settings
    float m_attack = 10;
    float m_decay = 5;
    float m_sustain = 0.5;
    float m_release = 100;
    float m_vibrato_hertz = 2;
    float m_vibrato_cents = 20;
    std::array<Wavetable<float>,MIDI_KEY_COUNT> m_wavetable;
    std::array<juce::AudioBuffer<float>, MIDI_KEY_COUNT> m_samples;
    int m_wavetable_size = 4096; //
    juce::AudioBuffer<float> m_sample;
    void load_sample(std::string file_name);
    //0 lewy 1 prawy
    OscillatorWaveform m_oscillator_waveform;
    size_t m_sample_length;
    std::array<std::array<std::vector<float>, 2>, MIDI_KEY_COUNT> m_shifted_samples;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessor);
    juce::Synthesiser m_synthesizer;

    LowFrequencyOscillator<float> m_vibrato_oscillator;
    Waveform m_vibrato_waveform = Waveform::SQUARE;
    /// <summary>
    /// tablice z dzwiêkami dla wszystkich klawiszy 
    /// </summary>

  
    //general settings 
  /*  float m_attack = 10;
    float m_decay = 5;
    float m_sustain = 0.5;
    float m_release = 100;*/
};
