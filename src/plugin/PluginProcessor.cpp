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
                       ),
    strip1(vOTT::BandType::low),
    apvts(*this, nullptr, "Parameters", vOTT::createParameterLayout())
{
    // todo: make a function that does most of this initialization?
    lowBandState.compParams.attack = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Comp Attack"));
    jassert (lowBandState.compParams.attack != nullptr);
    lowBandState.compParams.release = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Comp Release"));
    jassert (lowBandState.compParams.release != nullptr);
    lowBandState.compParams.threshold = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Comp Threshold"));
    jassert (lowBandState.compParams.threshold != nullptr);
    lowBandState.compParams.ratio = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Comp Ratio"));
    jassert (lowBandState.compParams.ratio != nullptr);

    lowBandState.upwdCompParams.attack = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Exp Attack"));
    jassert (lowBandState.upwdCompParams.attack != nullptr);
    lowBandState.upwdCompParams.release = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Exp Release"));
    jassert (lowBandState.upwdCompParams.release != nullptr);
    lowBandState.upwdCompParams.threshold = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Exp Threshold"));
    jassert (lowBandState.upwdCompParams.threshold != nullptr);
    lowBandState.upwdCompParams.ratio = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Exp Ratio"));
    jassert (lowBandState.upwdCompParams.ratio != nullptr);

    // todo: refactor this to use a parameter listener, and use an atomic flag updated by parameterChanged()
    // this will require ApvtsWrapper to be updated to support listeners and that callback
    lowBandState.frequency = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Low Frequency"));
    jassert (lowBandState.frequency != nullptr);
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
    strip1.prepare((int)sampleRate, getTotalNumOutputChannels());
    strip1.forceUpdateCompParams(lowBandState);

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

    strip1.updateParamsFromState(lowBandState);
    
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    strip1.process(context);
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
