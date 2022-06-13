/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <complex>
#include <vector>
#include "ComplexFFT.h"
#include "ComplexInverseFFT.h"
#include "RealFFT.h"
#include "RealInverseFFT.h"
#include "SpectrumFFT.h"
#include "SpectrumInverseFFT.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MySynthesizerVoice.h"
#include "MySynthesizerSound.h"
#include "WaveformModel.h"
#include "WavetableBuilder.h"
#include "WaveformFunctions.h"
//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //std::vector<float> waveform(1024);
    //for (int i = 0; i < waveform.size(); i++) {
    //    float t = float(i) / float(waveform.size());

    //    waveform[i] =0.25f+sin(3.0f * t * juce::MathConstants<float>::twoPi);
    //}
    //
    //std::vector<float> amplitudes(513);
    //std::vector<float> phases(513);
    ////rozmiar sygnalu dlugosc analizowanego bloku 
    //SpectrumFFT<float> sfft(1024);
    //sfft.perform(waveform.data(), amplitudes.data(), phases.data());
    m_oscillator_waveform = OscillatorWaveform::SINE;
    double f[MIDI_KEY_COUNT];
    for (int i = 0; i < MIDI_KEY_COUNT; i++) {
        f[i] = 440 * pow(2, (i - 69) / 12.0);
    }
    // to co u dolu wrzucone do funkcji
    generate_wavetables();
   /* WavetableBuilder<double, float> builder;
    WaveformModel < double > model(1024);
    for (int i = 0; i < 1024; i++) {
        float_t t = (i / 1024.0);

        model.m_samples[i] = LowFrequencyOscillator<float>::sine(t);
        model.m_samples[i] = sin(juce::MathConstants<double>::twoPi * t);
    }*/
    //1 wavetable
    // build(const WaveformModel<T>* model, int count, const T* base_frequencies, size_t wavetable_size, Wavetable<U>* result, T sample_rate) {
    //budujemy 128 wavetables bo tyle jest klawiszy

  /*  builder.build(&model,MIDI_KEY_COUNT,f,m_wavetable_size,m_wavetable.data(), 44100);*/
    m_synthesizer.addSound(new MySynthesizerSound());
    m_vibrato_waveform = Waveform::SQUARE;
    //m_synthesizer.addVoice(new MySynthesizerVoice());
    //dodajemy 16 glosow ktore mozemy odgrywac na raz
    for (int i = 0; i < 16; i++) {
        m_synthesizer.addVoice(new MySynthesizerVoice(this));
    }

    for (size_t i = 0; i < MIDI_KEY_COUNT; i++) {

    }
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //ustawiamy sampleRate syntezatora
    m_synthesizer.setCurrentPlaybackSampleRate(sampleRate);
    m_vibrato_waveform_samples.resize(samplesPerBlock);
    m_vibrato_oscillator.start();
    m_vibrato_oscillator.set_sample_rate(getSampleRate());
    //czestotliwosc probkowania w jakiej dziala nasz syntezator
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;
    //setting values
    m_vibrato_oscillator.set_hertz(m_vibrato_hertz);
   
    m_vibrato_oscillator.setWaveform(m_vibrato_waveform);
    //TODO: wzi¹æ liczbe próbek z rozmiaru buffora 
    for (int i = 0; i < m_vibrato_waveform_samples.size(); i++) {
        //wrzucam probki 
        m_vibrato_waveform_samples[i] = m_vibrato_oscillator.next();
    }

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    m_synthesizer.renderNextBlock(buffer,midiMessages,0,buffer.getNumSamples());

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    //for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
      /*  buffer.clear (i, 0, buffer.getNumSamples());*/


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...
    //}
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
void NewProjectAudioProcessor::setAttack(float value) {
    m_attack = value;
}
float NewProjectAudioProcessor::getAttack() {
    return m_attack;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
void NewProjectAudioProcessor::setDecay(float value) {
    this->m_decay = value;
}

void NewProjectAudioProcessor::setSustain(float value) {
    this->m_sustain = value;
}

void NewProjectAudioProcessor::setRelease(float value) {
    this->m_release = value;
}

void NewProjectAudioProcessor::setVibratoRate(float hertz) {
    m_vibrato_hertz = hertz;
}

void NewProjectAudioProcessor::setVibratoDepth(float cents) {
    m_vibrato_cents = cents;
}

float NewProjectAudioProcessor::getVibratoHertz() const {
    return m_vibrato_hertz;
}
float NewProjectAudioProcessor::getVibratoDepth() const {
    return m_vibrato_cents;
}

void NewProjectAudioProcessor::setVibratoWaveform(Waveform wave) {
    m_vibrato_waveform = wave;
}

Waveform NewProjectAudioProcessor::getVibratoWaveform() {
    return m_vibrato_waveform;
}

void NewProjectAudioProcessor::generate_wavetables() {
    int model_size = 1024;
    double frequency = this->getSampleRate();
    //tylko na czas wypelniania wavetable

    WaveformModel < double > model(model_size);
    switch (m_oscillator_waveform) {
    case OscillatorWaveform::SINE:
        for (int i = 0; i < model_size; i++) {
            float_t t = (i /(float)model_size);
            //model square
            model.m_samples[i] =WaveformFunctions::sine<double>(t);
        }
        break;
    case OscillatorWaveform::SQUARE:
        for (int i = 0; i < model_size; i++) {
            float_t t = (i / (float)model_size);
            //model square
            model.m_samples[i] = WaveformFunctions::square<double>(t);
        }
        break;
    case OscillatorWaveform::PULSE025: 
        for (int i = 0; i < model_size; i++) {
            float_t t = (i / (float)model_size);
            //model square
            model.m_samples[i] = WaveformFunctions::pulse<double>(0.25,t);
        }
        break;
    case OscillatorWaveform::PULSE01:
        for (int i = 0; i < model_size; i++) {
            float_t t = (i / (float)model_size);
            //model square
            model.m_samples[i] = WaveformFunctions::pulse<double>(0.1, t);
        }
        break;
    case OscillatorWaveform::SAW:
        for (int i = 0; i < model_size; i++) {
            float_t t = (i / (float)model_size);
            //model square
            model.m_samples[i] =WaveformFunctions::saw<double>(t);
        }
        break;
    case OscillatorWaveform::TRIANGLE:
        for (int i = 0; i < model_size; i++) {
            float_t t = (i / (float)model_size);
            //model square
            model.m_samples[i] = WaveformFunctions::triangle<double>(t);
        }
        break;
    }

    double f[MIDI_KEY_COUNT];
    for (int i = 0; i < MIDI_KEY_COUNT; i++) {
        f[i] = 440 * pow(2, (i - 69) / 12.0);
    }
    WavetableBuilder<double, float> builder;
    builder.build(&model, MIDI_KEY_COUNT,f, m_wavetable_size, m_wavetable.data(),frequency);
}

void NewProjectAudioProcessor::setOscillatorWaveform(OscillatorWaveform oscillator_wave_form) {
    m_oscillator_waveform = oscillator_wave_form;
    generate_wavetables();
}

OscillatorWaveform NewProjectAudioProcessor::getOscillatorWaveform() const {
    return m_oscillator_waveform;
}

void NewProjectAudioProcessor::load_sample(std::string file_name) {
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

    std::vector<float> left_channel;
    std::vector<float> right_channel;

    //juce::File file("C:\\Users\\zamoj\\OneDrive\\Pulpit\\ttsMP3.com_VoiceText_2022-5-20_20_36_1.wav");
    juce::File file(file_name);
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    juce::ScopedPointer<juce::AudioFormatReader> reader = formatManager.createReaderFor(file);
    if (reader != 0)
    {

        int samples_per_channel = reader->lengthInSamples / reader->numChannels;
        left_channel.resize(samples_per_channel);
        right_channel.resize(samples_per_channel);
        if (reader->numChannels == 1) {
            float* temp = left_channel.data();
            reader->read(&temp, 1, 0, samples_per_channel);
            memcpy(right_channel.data(), temp, samples_per_channel * sizeof(float));
        }
        else {
            float* data[] = { left_channel.data(),right_channel.data() };
            reader->read(data, 2, 0, samples_per_channel * 2);
        }

        /* for (size_t i = 0; i < samples_per_channel; i++) {
             float t = (float)i / samples_per_channel;
             float value = sin(200 * t * 3.14 * 2);
             left_channel[i] = value;
             right_channel[i] = value;
         }*/
        //uzupe³nianie zerami do potegi 2 
        int power_of_two = 2;
        
        for (; power_of_two < samples_per_channel; power_of_two *= 2) {}
        m_sample_length = power_of_two;
        SpectrumFFT<float> sfft(power_of_two);
       
        //left_channel
        /*SpectrumFFT*/
        left_channel.resize(power_of_two,0);
        right_channel.resize(power_of_two,0);
        std::vector<float> amplitudes[2];
        std::vector<float> phases[2];
        m_sample.setSize(2, samples_per_channel);
        m_sample.copyFrom(0, 0, left_channel.data(), samples_per_channel);
        m_sample.copyFrom(1, 0, right_channel.data(), samples_per_channel);
        //druga po³owa jest symetryczna 
        size_t spectrum_length = power_of_two / 2 + 1;
        amplitudes[0].resize(spectrum_length);
        amplitudes[1].resize(spectrum_length);
        phases[0].resize(spectrum_length);
        phases[1].resize(spectrum_length);
        sfft.perform(left_channel.data(), amplitudes[0].data(), phases[0].data());
        sfft.perform(right_channel.data(), amplitudes[1].data(), phases[1].data());
        int shift_per_key = 100;

        std::vector<float> shifted_amplitudes[2];
        shifted_amplitudes[0].resize(spectrum_length);
        shifted_amplitudes[1].resize(spectrum_length);
        std::vector<float> shifted_phases[2];
        shifted_phases[0].resize(spectrum_length);
        shifted_phases[1].resize(spectrum_length);
        int shift_buckets;
        SpectrumInverseFFT<float> sifft(power_of_two);

        for (size_t key = 0; key < MIDI_KEY_COUNT; key++) {
            shift_buckets = (key - MIDI_KEY_COUNT / 2) * shift_per_key;
            if (shift_buckets < 0) {
                for (size_t i = 1; i < spectrum_length+shift_buckets; i++) {

                    shifted_amplitudes[0][i] = amplitudes[0][i - shift_buckets];
                    shifted_amplitudes[1][i] = amplitudes[1][i - shift_buckets];
                    shifted_phases[0][i] = phases[0][i - shift_buckets];
                    shifted_phases[1][i] = phases[1][i - shift_buckets];
                }
            }
            else {
                for (size_t i = spectrum_length-1; i > shift_buckets; i--) {
                    shifted_amplitudes[0][i] = amplitudes[0][i - shift_buckets];
                    shifted_amplitudes[1][i] = amplitudes[1][i - shift_buckets];
                    shifted_phases[0][i] = phases[0][i - shift_buckets];
                    shifted_phases[1][i] = phases[1][i - shift_buckets];

                }
            }
            //1 kanal
            m_shifted_samples[key][0].resize(power_of_two);
            //2
            m_shifted_samples[key][1].resize(power_of_two);
            sifft.perform(shifted_amplitudes[0].data(), shifted_phases[0].data(),
                m_shifted_samples[key][0].data());

            m_shifted_samples[key][1].resize(power_of_two);
            sifft.perform(shifted_amplitudes[1].data(), shifted_phases[1].data(),
                m_shifted_samples[key][1].data());

        }
        
    }

}