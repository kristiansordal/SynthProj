#pragma once
#include "SynthSound.h"
#include <JuceHeader.h>

class SynthVoice : public juce::SynthesiserVoice {
  public:
    bool canPlaySound(juce::SynthesiserSound *sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound,
                   int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;

  private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::AudioBuffer<float> synthBuffer;
    std::function<float(float)> sineWave = [](float x) { return std::sin(x); };
    std::function<float(float)> sawWave = [](float x) { return x / juce::MathConstants<float>::pi; };
    std::function<float(float)> squareWave = [](float x) { return x < 0.0f ? -1.0f : 1.0f; };
    bool isPrepared{false};

    // The Oscillator we will use to create a sine wave;
    juce::dsp::Oscillator<float> osc{sineWave};

    // Gain to adjust the volume so its not too loud
    juce::dsp::Gain<float> gain;
};
