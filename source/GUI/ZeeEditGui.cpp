#include "ZeeEditGui.h"

#include "WidgetPanel.h"
#include "LayoutProcessor.h"
#include "../ParameterMap.h"
#include "../PluginProcessorBase.h"
#include "../ThreadSafeQueue.h"

#include <melatonin_inspector/melatonin_inspector.h>

ZeeEditGui::ZeeEditGui(PluginProcessorBase& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState, ThreadSafeQueue<juce::MidiBuffer>& inputMidiMessageQueue) :
    AudioProcessorEditor(&pluginProcessor),
    m_inputMidiMessageQueue(inputMidiMessageQueue)
{
    // Create the widget panels
    for (const settings::WidgetPanel& panel : ParameterMap::getPanels())
    {
        m_widgetPanels.push_back(std::make_unique<WidgetPanel>(panel, valueTreeState, m_midiParameterMap));
        addAndMakeVisible(*m_widgetPanels.back());
    }

    setSize(1140, 800); // Set a default size for the editor
    setResizable (true, false);

#if JUCE_DEBUG
    m_inspector = std::make_unique<melatonin::Inspector>(*this);
    m_inspector->onClose = [this]() { m_inspector.reset(); };
    m_inspector->setVisible(true);
#endif
}

ZeeEditGui::~ZeeEditGui() = default;

void ZeeEditGui::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(g.getCurrentFont().withHeight(11.0f));
}

void ZeeEditGui::resized()
{
    LayoutProcessor layoutProcessor(getWidth());
    for (auto& panel : m_widgetPanels)
    {
        layoutProcessor.insert(*panel, panel->getLabelHeight());
    }
}

void ZeeEditGui::changeListenerCallback(juce::ChangeBroadcaster* /*source*/)
{
    m_inputMidiMessageQueue.popAll([this](const juce::MidiBuffer& midiBuffer)
    {
        for (const juce::MidiMessageMetadata& midiMessageMetadata : midiBuffer)
        {
            m_midiParameterMap.setParameterValue(midiMessageMetadata.getMessage());
        }
    });
}
