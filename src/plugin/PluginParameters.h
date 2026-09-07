#pragma once

#include <../common/dsp/utils/ApvtsWrapper.h>

/**
 * plugin/PluginParameters.h
 *
 * This file exists to create a parameterlayout for the plugin
 *
 * todo: figure out a way where the parameterLayout.add() shit can be condensed into loops & have the naming work right
 */

class PluginParameters : public vOTT::ApvtsWrapper {
public:
    PluginParameters(juce::AudioProcessor& p) : ApvtsWrapper(p, createParameterLayout()) {}

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        using namespace juce;
        AudioProcessorValueTreeState::ParameterLayout layout;

        auto atkRlsRange = NormalisableRange<float>(0.f, 1000.f, 0.1f, 0.5f);

        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Frequency", 1),
                                                              "Low Crossover Frequency",
                                                              NormalisableRange<float>(40.f, 1000.f, 0.1f, 1.f),
                                                              88));

        // parameters for the first compressor
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Comp Threshold", 1),
                                                          "Low Compressor Threshold",
                                                          NormalisableRange<float>(-60.f, 12.f, 0.5f),
                                                          0));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Comp Attack", 1),
                                                            "Low Compressor Attack",
                                                            atkRlsRange,
                                                            50));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Comp Release", 1),
                                                            "Low Compressor Release",
                                                            atkRlsRange,
                                                            250));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Comp Ratio", 1),
                                                            "Low Compressor Ratio",
                                                            NormalisableRange<float>(1.f, 100.f, 0.5f, 0.5),
                                                            4));

        return layout;

    }
    // todo: define this function
};