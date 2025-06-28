#pragma once

#include "MidiParameterMap.h"
#include "WidgetWithLabel.h"
#include <juce_audio_processors/juce_audio_processors.h>

namespace melatonin {
class Inspector;
}

class ZeeEdit;
class WidgetPanel;
struct BankAndPG;
template<class T> class ThreadSafeQueue;

//==============================================================================
class ZeeEditGui : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    ZeeEditGui(ZeeEdit& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState);
    ~ZeeEditGui() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void timerCallback() override;

private:
    // Header
    WidgetWithLabel<juce::ComboBox, juce::Label, juce::AudioProcessorValueTreeState::ComboBoxAttachment> m_midiChannelSelector;
    WidgetWithLabel<juce::ComboBox, juce::Label, juce::AudioProcessorValueTreeState::ComboBoxAttachment> m_programSelector;

    // Widgets
    std::vector<std::unique_ptr<WidgetPanel>> m_widgetPanels;
    ThreadSafeQueue<juce::MidiMessage>& m_inputMidiMessageQueue;
    ThreadSafeQueue<BankAndPG>& m_inputProgramChangeQueue;
    MidiParameterMap m_midiParameterMap;

    std::unique_ptr<melatonin::Inspector> m_inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZeeEditGui)
};
