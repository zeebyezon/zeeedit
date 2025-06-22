#include "ZeeEditGui.h"

#include "WidgetPanel.h"
#include "LayoutProcessor.h"
#include "../ParameterMap.h"
#include "../PluginProcessorBase.h"
#include "../ThreadSafeQueue.h"

#include <melatonin_inspector/melatonin_inspector.h>

constexpr int HEADER_HEIGHT = 30; // Height of the header component

ZeeEditGui::ZeeEditGui(PluginProcessorBase& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState, ThreadSafeQueue<juce::MidiBuffer>& inputMidiMessageQueue) :
    AudioProcessorEditor(&pluginProcessor),
    m_midiChannelSelector(WidgetType::SELECT, valueTreeState, "$globalMidiChannel", [](juce::ComboBox& selector) {
        for (int i = 1; i <= 16; ++i)
        {
            selector.addItem(juce::String(i), i);
        }
    }),
    m_inputMidiMessageQueue(inputMidiMessageQueue)
{
    m_midiChannelSelector.getLabel().setText("MIDI channel", juce::dontSendNotification);
    m_midiChannelSelector.getLabel().attachToComponent(&m_midiChannelSelector.getComponent(), true);
    addAndMakeVisible(m_midiChannelSelector.getComponent());
    addAndMakeVisible(m_midiChannelSelector.getLabel());

    // Create the widget panels
    for (const settings::WidgetPanel& panel : ParameterMap::getPanels())
    {
        m_widgetPanels.push_back(std::make_unique<WidgetPanel>(panel, valueTreeState, m_midiParameterMap));
        addAndMakeVisible(*m_widgetPanels.back());
    }

    setSize(1140, 800 + HEADER_HEIGHT); // Set a default size for the editor
    setResizable (true, false);

    startTimer(100); // Start a timer to poll the input MIDI message queue

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
    auto area = getLocalBounds();
    auto headerBounds = area.removeFromTop(HEADER_HEIGHT);
    headerBounds.reduce(5, 5); // Reduce the header bounds for padding
    headerBounds.removeFromLeft(100); // Leave space for the label
    m_midiChannelSelector.getComponent().setBounds(headerBounds.removeFromLeft(60));

    LayoutProcessor layoutProcessor(area.getY(), area.getWidth());
    for (auto& panel : m_widgetPanels)
    {
        layoutProcessor.insert(*panel, panel->getLabelHeight());
    }
}

void ZeeEditGui::timerCallback()
{
    m_inputMidiMessageQueue.popAll([this](const juce::MidiBuffer& midiBuffer) {
        for (const juce::MidiMessageMetadata& midiMessageMetadata : midiBuffer)
        {
            m_midiParameterMap.setParameterValue(midiMessageMetadata.getMessage());
        }
    });
}
