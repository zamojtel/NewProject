

struct MySynthesizerSound : public juce::SynthesiserSound
{
    MySynthesizerSound() {}

    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
    
};