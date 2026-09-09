#pragma once

namespace vOTT {

inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
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

        // parameters for the first expander
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Exp Threshold", 1),
                                                          "Low Expander Threshold",
                                                          NormalisableRange<float>(-60.f, 12.f, 0.5f),
                                                          -40.f));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Exp Attack", 1),
                                                            "Low Expander Attack",
                                                            atkRlsRange,
                                                            50));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Exp Release", 1),
                                                            "Low Expander Release",
                                                            atkRlsRange,
                                                            250));
        layout.add(std::make_unique<AudioParameterFloat>(ParameterID("Low Exp Ratio", 1),
                                                            "Low Expander Ratio",
                                                            NormalisableRange<float>(1.f, 100.f, 0.5f, 0.5),
                                                            2.5f));

        return layout;
    }
} // vOTT