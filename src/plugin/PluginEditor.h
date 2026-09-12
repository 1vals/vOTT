#pragma once

#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class OTTAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit OTTAudioProcessorEditor (OTTAudioProcessor&);
    ~OTTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OTTAudioProcessor& processorRef;
    melatonin::Inspector inspector { *this };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OTTAudioProcessorEditor)
};
