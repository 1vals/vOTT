#pragma once

#include "PluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

/** UI ROADMAP
 *  1. implement desired wireframe
 *  2. implement dummy sliders & knobs
 *  3. implement band selection
 *  4. attach knobs dynamically to selected band
 *  5. create menu with input/output gain, and xover frequencies
 *
 *  LATER
 *  create meters for the bands
 *  design custom assets
 */

// TEMPORARY
struct CustomRotarySlider : juce::Slider {
    CustomRotarySlider() : Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                        juce::Slider::TextEntryBoxPosition::NoTextBox) { }
};

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
    OTTAudioProcessor& processorRef;
    melatonin::Inspector inspector { *this };

    CustomRotarySlider slider1, slider2, slider3, slider4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OTTAudioProcessorEditor)
};
