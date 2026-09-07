#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OTTAudioProcessor::OTTAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    // todo: make a function that does most of this initialization?
    compParams.attack = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Comp Attack"));
    jassert (compParams.attack != nullptr);
    compParams.release = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Comp Release"));
    jassert (compParams.release != nullptr);
    compParams.threshold = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Comp Threshold"));
    jassert (compParams.threshold != nullptr);
    compParams.ratio = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Comp Ratio"));
    jassert (compParams.ratio != nullptr);

    expParams.attack = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Exp Attack"));
    jassert (expParams.attack != nullptr);
    expParams.release = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Exp Release"));
    jassert (expParams.release != nullptr);
    expParams.threshold = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Exp Threshold"));
    jassert (expParams.threshold != nullptr);
    expParams.ratio = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Exp Ratio"));
    jassert (expParams.ratio != nullptr);

    // todo: refactor this to use a parameter listener, and use an atomic flag updated by parameterChanged()
    // this will require ApvtsWrapper to be updated to support listeners and that callback
    lowXoverFreq = dynamic_cast<juce::AudioParameterFloat*>(parameters.getParameter("Low Frequency"));
    jassert (lowXoverFreq != nullptr);
}

OTTAudioProcessor::~OTTAudioProcessor()
{
}

//==============================================================================
const juce::String OTTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OTTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OTTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OTTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OTTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OTTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OTTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OTTAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String OTTAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void OTTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void OTTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    compressor.prepare(static_cast<int>(sampleRate));
    compressor.forceUpdateAllParams(compParams);

    expander.prepare(static_cast<int>(sampleRate));
    expander.forceUpdateAllParams(expParams);

    lowXover.prepare(static_cast<int>(sampleRate), getTotalNumOutputChannels());

    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void OTTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool OTTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void OTTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    lowXover.setFilterFrequency(lowXoverFreq->get());

    compressor.setAttack(compParams.attack->get());
    compressor.setRelease(expParams.release->get());
    compressor.setThreshold(expParams.threshold->get());
    compressor.setRatio(expParams.ratio->get());

    expander.setAttack(expParams.attack->get());
    expander.setRelease(expParams.release->get());
    expander.setThreshold(expParams.threshold->get());
    expander.setRatio(expParams.ratio->get());

    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    // lowXover.process(context);

    expander.process(context);

    // compressor.process(context);
}

//==============================================================================
bool OTTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OTTAudioProcessor::createEditor()
{
    // return new OTTAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void OTTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void OTTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OTTAudioProcessor();
}
