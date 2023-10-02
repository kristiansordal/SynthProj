/*
  ==============================================================================

    ADSRData.cpp
    Created: 2 Oct 2023 6:08:57pm
    Author:  Kristian Sørdal

  ==============================================================================
*/

#include "ADSRData.h"

void ADSRData::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    setParameters(adsrParams);
}
