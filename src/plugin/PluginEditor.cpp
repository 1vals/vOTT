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

    slider1.addAndMakeVisible(*this);
    slider2.addAndMakeVisible(*this);
    slider3.addAndMakeVisible(*this);
    slider4.addAndMakeVisible(*this);
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
    auto bounds = getLocalBounds();
    int height = bounds.getHeight(), width = bounds.getWidth();

    slider1.setBounds(knobset2Bounds.removeFromLeft(bounds.getWidth() / 4));
    slider2.setBounds(knobset2Bounds.removeFromLeft(bounds.getWidth() / 3));
    slider3.setBounds(knobset2Bounds.removeFromLeft(bounds.getWidth() / 2));
    slider4.setBounds(knobset2Bounds);
}
