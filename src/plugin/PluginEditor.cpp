#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OTTAudioProcessorEditor::OTTAudioProcessorEditor (OTTAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (400, 500);

    inspector.setVisible(true);
    inspector.toggle(true);
}

OTTAudioProcessorEditor::~OTTAudioProcessorEditor()
{
}

//==============================================================================
void OTTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OTTAudioProcessorEditor::resized()
{

}
