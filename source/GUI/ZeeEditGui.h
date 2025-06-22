#pragma once

#include "MidiParameterMap.h"
#include "WidgetWithLabel.h"
#include <juce_audio_processors/juce_audio_processors.h>

namespace melatonin {
class Inspector;
}

class PluginProcessorBase;
class WidgetPanel;
template<class T> class ThreadSafeQueue;

//==============================================================================
class ZeeEditGui : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    ZeeEditGui(PluginProcessorBase& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState, ThreadSafeQueue<juce::MidiBuffer>& inputMidiMessageQueue);
    ~ZeeEditGui() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void timerCallback() override;

private:
    // Header
    WidgetWithLabel<juce::ComboBox, juce::Label, juce::AudioProcessorValueTreeState::ComboBoxAttachment> m_midiChannelSelector;

    // Widgets
    std::vector<std::unique_ptr<WidgetPanel>> m_widgetPanels;
    ThreadSafeQueue<juce::MidiBuffer>& m_inputMidiMessageQueue;
    MidiParameterMap m_midiParameterMap;

    std::unique_ptr<melatonin::Inspector> m_inspector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZeeEditGui)
};
