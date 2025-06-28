#include "ZeeEditGui.h"

#include "WidgetPanel.h"
#include "LayoutProcessor.h"
#include "../ParameterMap.h"
#include "../ProgramMap.h"
#include "../ZeeEdit.h"
#include "../ThreadSafeQueue.h"

#include <melatonin_inspector/melatonin_inspector.h>

constexpr int HEADER_HEIGHT = 30; // Height of the header component

ZeeEditGui::ZeeEditGui(ZeeEdit& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState) :
    AudioProcessorEditor(&pluginProcessor),
    m_midiChannelSelector(WidgetType::SELECT, valueTreeState, "$globalMidiChannel", [](juce::ComboBox& selector) {
        for (int i = 1; i <= 16; ++i)
        {
            selector.addItem(juce::String(i), i);
        }
    }),
    m_programSelector(WidgetType::SELECT, valueTreeState, "$programSelector", [](juce::ComboBox& selector) {
        const std::vector<Bank>& programMap = ProgramMap::getProgramMap();
        for (const Bank& bank : programMap)
        {
            int programIndex = 0;
            for (const std::string& programName : bank.programNames)
            {
                selector.addItem(bank.bankName + ": " + programName, 1 + 128 * (128 * bank.bankNumberMsb + bank.bankNumberLsb) + programIndex++);
            }
        }
    }),
    m_inputMidiMessageQueue(pluginProcessor.getInputMidiMessageQueue()),
    m_inputProgramChangeQueue(pluginProcessor.getInputProgramChangeQueue())
{
    m_midiChannelSelector.getLabel().setText("MIDI channel", juce::dontSendNotification);
    m_midiChannelSelector.getLabel().attachToComponent(&m_midiChannelSelector.getComponent(), true);
    addAndMakeVisible(m_midiChannelSelector.getComponent());
    addAndMakeVisible(m_midiChannelSelector.getLabel());

    m_programSelector.getLabel().setText("Program", juce::dontSendNotification);
    m_programSelector.getLabel().attachToComponent(&m_programSelector.getComponent(), true);
    addAndMakeVisible(m_programSelector.getComponent());
    addAndMakeVisible(m_programSelector.getLabel());
    m_programSelector.getComponent().onChange = [this, &pluginProcessor]() {
        int newValue = m_programSelector.getComponent().getSelectedId();
        --newValue; // Convert to zero-based index
        const int bankMsb = newValue / (128 * 128);
        const int bankLsb = (newValue / 128) % 128;
        const int programNumber = newValue % 128;

        pluginProcessor.sendProgramChange(bankMsb, bankLsb, programNumber);
    };

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
    headerBounds.removeFromLeft(100); // Leave space for the label
    m_programSelector.getComponent().setBounds(headerBounds.removeFromLeft(200));

    LayoutProcessor layoutProcessor(area.getY(), area.getWidth());
    for (auto& panel : m_widgetPanels)
    {
        layoutProcessor.insert(*panel, panel->getLabelHeight());
    }
}

void ZeeEditGui::timerCallback()
{
    m_inputProgramChangeQueue.popAll([this](const BankAndPG& pg) {
        m_programSelector.getComponent().setSelectedId(1 + 128 * (128 * pg.bankMsb + pg.bankLsb) + pg.programNumber, juce::dontSendNotification);
    });
    m_inputMidiMessageQueue.popAll([this](const juce::MidiMessage& midiMessage) {
        m_midiParameterMap.setParameterValue(midiMessage);
    });
}
