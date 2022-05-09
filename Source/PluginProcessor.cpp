/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SineWaveVoice.h"
#include "SineWaveSound.h"

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
    
    m_synthesizer.addSound(new SineWaveSound());
    m_vibrato_waveform = Waveform::SQUARE;
    //m_synthesizer.addVoice(new SineWaveVoice());
    //dodajemy 16 glosow ktore mozemy odgrywac na raz
    for (int i = 0; i < 16; i++) {
        m_synthesizer.addVoice(new SineWaveVoice(this));
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