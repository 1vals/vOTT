#pragma once

#include <common/Parameters.h>

/**
 * plugin/PluginParameters.h
 *
 * This file exists to create a parameterlayout for the plugin
 *
 * todo: figure out a way where the parameterLayout.add() shit can be condensed into loops & have the naming work right
 */

class PluginParameters : public vOTT::Parameters {
public:
    PluginParameters(juce::AudioProcessor& p) : Parameters(p, createParameterLayout()) {}

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        using namespace juce;
        AudioProcessorValueTreeState::ParameterLayout layout;

        auto atkRlsRange = NormalisableRange<float>(0.f, 1000.f, 0.1f, 0.5f);

        // parameters for the first compressor
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Threshold", 1),
                                                          "Threshold",
                                                          NormalisableRange<float>(-60.f, 12.f, 0.5f),
                                                          0));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Attack", 1),
                                                            "Attack",
                                                            atkRlsRange,
                                                            50));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Release", 1),
                                                            "Release",
                                                            atkRlsRange,
                                                            250));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Ratio", 1),
                                                            "Ratio",
                                                            NormalisableRange<float>(1.f, 100.f, 0.5f, 0.5),
                                                            4));

        return layout;

    }
    // todo: define this function
};