/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SynthProjAudioProcessorEditor::SynthProjAudioProcessorEditor(SynthProjAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p), adsr(audioProcessor.apvts) {

    setSize(400, 300);

    // oscSelectAttachment =
    // std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts,
    //                                                                                                "OSC",
    //                                                                                                oscSelector);
    //
    // make adsr visible
    addAndMakeVisible(adsr);
}

SynthProjAudioProcessorEditor::~SynthProjAudioProcessorEditor() {}

//==============================================================================
void SynthProjAudioProcessorEditor::paint(juce::Graphics &g) { g.fillAll(juce::Colours::black); }
void SynthProjAudioProcessorEditor::resized() { adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight()); }
