#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginParameters.h"
#include <dsp/dsp.h>
#include <common/BandStrip.h>
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
    // vOTT::Filters::SecondOrderButterworth lowXover;
    // vOTT::Compressor compressor;
    // vOTT::UpwardsCompressor expander;

    vOTT::BandStrip strip1;

    PluginParameters parameters {*this};

    juce::AudioParameterFloat* lowXoverFreq { nullptr };
    float lastFrequency;

    vOTT::Dynamics::ParamPtrs expParams;
    vOTT::Dynamics::ParamPtrs compParams;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OTTAudioProcessor)
};
