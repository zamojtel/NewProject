
#include "PluginProcessor.h"
#include <JuceHeader.h>
#include "SineWaveSound.h"
#include "SineWaveVoice.h"

SineWaveVoice::SineWaveVoice(NewProjectAudioProcessor *newProjectAudioProcessor) {
    m_plugin_processor = newProjectAudioProcessor;
}

bool SineWaveVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*> (sound) != nullptr;
}
                                //numer klawisza 
void SineWaveVoice::startNote(int midiNoteNumber, float velocity,
    juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    
    //m_attack jest milisekundach
    //milisekund zamieniamy na sample liczba sampli na milisekunde
    m_envelope.m_attack = m_plugin_processor->getAttack() *( getSampleRate() / 1000);
    //m_envelope.m_attack = 10000*getSampleRate()/1000;
    m_envelope.m_decay=m_plugin_processor->m_decay * (getSampleRate() / 1000);
    //m_envelope.m_decay = 10 * getSampleRate() / 1000;
    //m_envelope.m_sustain = 1; //wartosc
    m_envelope.m_sustain = m_plugin_processor->m_sustain;
    //m_envelope.m_release = 10*getSampleRate();
    m_envelope.m_release=m_plugin_processor->m_release*(getSampleRate() / 1000);
    m_envelope.start();

    //currentAngle = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    //auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    //auto cyclesPerSample = cyclesPerSecond / getSampleRate();

    //angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    m_wavetable = &m_plugin_processor->m_wavetable[midiNoteNumber];
    m_normalized_frequency =juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / getSampleRate();
    m_phase = 0;
}

void SineWaveVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        m_envelope.release();
    }
    else
    {
        // we're being told to stop playing immediately, so reset everything..
        clearCurrentNote();
        //angleDelta = 0.0;
        m_normalized_frequency = 0;
    }
}

void SineWaveVoice::pitchWheelMoved(int /*newValue*/) {}
void SineWaveVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/) {}


//kiedy gramy
void SineWaveVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (m_normalized_frequency != 0.0)
    {
        //numSamples to rozmiar bloku czyli liczba generowanych próbek 
        for (int i = 0; i < numSamples; i++) {
            float envelope = m_envelope.next();
            //generowanie dzwiêku 
            //auto currentSample = (float)(std::sin(m_phase * juce::MathConstants<float>::twoPi) * level) * envelope;
            /*auto currentSample = (float)(m_plugin_processor->m_wavetable.samples[int(m_phase*m_plugin_processor->m_wavetable.samples.size())] * level) * envelope;*/
            //z obecnego wavetable bierzemy wyliczne na podstawie phase'a
            //interpolacja liniowa
            float sample = m_phase * m_plugin_processor->m_wavetable_size;
            int j0 = (int)sample;
            int j1 = sample + 1;
            float coef = sample - j0;
            float val1 = m_wavetable->samples[j0];
            float val2 = m_wavetable->samples[j1];

            auto currentSample = (val1+coef*(val2-val1))* level * envelope;
            //auto currentSample=()
            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample(i, startSample, currentSample);
            //zmiana czêstotliwoœci odgrywanego dzwiêku
            float f =m_normalized_frequency*pow(2,
                m_plugin_processor->m_vibrato_waveform_samples[i]*m_plugin_processor->m_vibrato_cents/1200);
                m_phase+=f;
                if (m_phase >= 1) {
                    m_phase -= 1;
               }
            ++startSample;
            if (m_envelope.isFinished()) {
                clearCurrentNote();
                //informuje ze skonczyl sie dzwiek 
                break;
            }
        }
          
        //}
    }
}



/*using SynthesiserVoice::renderNextBlock;*/

//double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
