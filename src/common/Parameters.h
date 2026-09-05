#pragma once

/**
 * common/Parameters.h
 * this class serves as a wrapper for juce's AudioProcessorValueTreeState
 *
 */

#include <juce_audio_processors/juce_audio_processors.h>

namespace vOTT {
class Parameters {
public:
    using APVTS = juce::AudioProcessorValueTreeState;

    Parameters(juce::AudioProcessor& p, APVTS::ParameterLayout layout)
        : apvts(p, nullptr, "Parameters", std::move(layout)) { }

    virtual ~Parameters() = default;

    virtual void replaceState(const juce::ValueTree& newState) { apvts.replaceState(newState); }
    juce::ValueTree copyState() { return apvts.copyState(); }

    void createAndAddParameter(std::unique_ptr<juce::RangedAudioParameter> param) {
        apvts.createAndAddParameter(std::move(param));
    }

    juce::RangedAudioParameter* getParameter(const juce::String& id) {
        return apvts.getParameter(id);
    }

    /** todo:
     *  1. create wrappers for parameter attachments
     *  2. create wrappers for parameter listeners
     *  3. create wrappers for the getter functions inside of juce::APVTS
     */

private:
    APVTS apvts;
};
} // vOTT