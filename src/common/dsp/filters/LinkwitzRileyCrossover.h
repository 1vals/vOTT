#pragma once

#include <juce_dsp/juce_dsp.h>
#include "SecondOrderButterworth.h"

namespace vOTT::Filters {

class LinkwitzRileyCrossover {
public:
    LinkwitzRileyCrossover() = default;

    void setFilterType(Filters::SecondOrderButterworth::FilterType type) {
        stage1.setFilterType(type);
        stage2.setFilterType(type);
    }

    void setFilterFrequency(float newFreq) {
        stage1.setFilterFrequency(newFreq);
        stage2.setFilterFrequency(newFreq);
    }

    void prepare(int sampleRate, int numChannels) {
        stage1.prepare(sampleRate, numChannels);
        stage2.prepare(sampleRate, numChannels);
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context) {
        stage1.process(context);
        stage2.process(context);
    }
private:
    Filters::SecondOrderButterworth stage1, stage2;
};

} // vOTT::Filters