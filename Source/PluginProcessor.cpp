/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.cpp"

//==============================================================================
SynthProjAudioProcessor::SynthProjAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    for (int i = 0; i < 5; i++) {

        synth.addVoice(new SynthVoice());
    }
}

SynthProjAudioProcessor::~SynthProjAudioProcessor() {}

//==============================================================================
const juce::String SynthProjAudioProcessor::getName() const { return JucePlugin_Name; }

bool SynthProjAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SynthProjAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SynthProjAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SynthProjAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SynthProjAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int SynthProjAudioProcessor::getCurrentProgram() { return 0; }

void SynthProjAudioProcessor::setCurrentProgram(int index) {}

const juce::String SynthProjAudioProcessor::getProgramName(int index) { return {}; }

void SynthProjAudioProcessor::changeProgramName(int index, const juce::String &newName) {}

void SynthProjAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SynthProjAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthProjAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SynthProjAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice *>(synth.getVoice(i))) {

            auto &attack = *apvts.getRawParameterValue("ATTACK");
            auto &decay = *apvts.getRawParameterValue("DECAY");
            auto &sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto &release = *apvts.getRawParameterValue("RELEASE");

            voice->update(attack.load(), decay.load(), sustain.load(), release.load());
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthProjAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *SynthProjAudioProcessor::createEditor() { return new SynthProjAudioProcessorEditor(*this); }

//==============================================================================
void SynthProjAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthProjAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new SynthProjAudioProcessor(); }

juce::AudioProcessorValueTreeState::ParameterLayout SynthProjAudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Oscillators
    // params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator",
    //                                                               juce::StringArray{"Sine", "Saw", "Square"}, 0));

    // ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"ATTACK", 1}, "Attack", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"DECAY", 1}, "Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"SUSTAIN", 1}, "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"RELEASE", 1}, "Release", juce::NormalisableRange<float>{0.1f, 3.0f, 0.1f}, 0.4f));

    // params.push_back(std::make_unique<juce::AudioParameterFloat>(
    //     "ATTACK", "Attack", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    // params.push_back(std::make_unique<juce::AudioParameterFloat>(
    //     "DECAY", "Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 0.1f));
    // params.push_back(std::make_unique<juce::AudioParameterFloat>(
    //     "SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.1f}, 1.0f));
    // params.push_back(std::make_unique<juce::AudioParameterFloat>(
    //     "RELEASE", "Release", juce::NormalisableRange<float>{0.1f, 3.0f, 0.1f}, 0.4f));

    return {params.begin(), params.end()};
}
