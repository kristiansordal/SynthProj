/*
  ==============================================================================

    ADSRData.h
    Created: 2 Oct 2023 6:08:57pm
    Author:  Kristian Sørdal

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using juce::ADSR;

class ADSRData : public ADSR {
  public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

  private:
    juce::ADSR::Parameters adsrParams;
};
