#include "ZeeEditGui.h"

#include "../PluginProcessorBase.h"
#include "WidgetPanel.h"

ZeeEditGui::ZeeEditGui(PluginProcessorBase& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState) :
    AudioProcessorEditor(&pluginProcessor)
{
    m_widgetPanel = std::make_unique<WidgetPanel>(valueTreeState);
    addAndMakeVisible(*m_widgetPanel);

    setSize(400, 300);
}

ZeeEditGui::~ZeeEditGui() = default;

void ZeeEditGui::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must fill the background with a solid color)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ZeeEditGui::resized()
{
}
