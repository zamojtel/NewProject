
#include "ADSR.h"

struct MySynthesizerVoice : public juce::SynthesiserVoice
{
    MySynthesizerVoice(NewProjectAudioProcessor *m_plugin_processor);

    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;

    void stopNote(float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved(int /*newValue*/) override;
    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    ADSR_envelope<float> m_envelope;

    using SynthesiserVoice::renderNextBlock;

private:
    //double currentAngle = 0.0, angleDelta = 0.0, 
    int midi_note_number;
    double level = 0.0, tailOff = 0.0;
    NewProjectAudioProcessor *m_plugin_processor;
    //0-1 gdzie 1 to czêstotliwoœæ próbkowania 
    //czestotliwosc generowanej fali dzwiekowej podstawowa (dopowiadajaca wcisnietemu klawiszowi)
    float m_normalized_frequency;
    float m_phase;
    Wavetable<float>* m_wavetable;
    int m_sample_position=0;
};