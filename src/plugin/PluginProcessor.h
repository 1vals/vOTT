#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginParameters.h"
#include <common/dsp/dynamics/dynamics.h>
#include <common/dsp/filters/LinkwitzRileyCrossover.h>

//==============================================================================
class OTTAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    OTTAudioProcessor();
    ~OTTAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    // vOTT::Filters::LinkwitzRileyCrossover lowXover;
    vOTT::Filters::SecondOrderButterworth lowXover;
    vOTT::Compressor compressor;

    PluginParameters parameters {*this};

    juce::AudioParameterFloat* frequency { nullptr };
    float lastFrequency;

    struct DynamicsParameters {
        juce::AudioParameterFloat* attack { nullptr };
        juce::AudioParameterFloat* release { nullptr };
        juce::AudioParameterFloat* threshold { nullptr };
        juce::AudioParameterFloat* ratio { nullptr };
    };

    // when multiple bands are added, these can be converted into vectors
    DynamicsParameters compParams;
    DynamicsParameters expParams;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OTTAudioProcessor)
};
