/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class SynthProjAudioProcessorEditor : public juce::AudioProcessorEditor {
  public:
    SynthProjAudioProcessorEditor(SynthProjAudioProcessor &);
    ~SynthProjAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    juce::ComboBox oscSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;
    SynthProjAudioProcessor &audioProcessor;
    ADSRComponent adsr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthProjAudioProcessorEditor)
};
