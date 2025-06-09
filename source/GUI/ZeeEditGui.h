#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class PluginProcessorBase;
class WidgetPanel;

//==============================================================================
class ZeeEditGui : public juce::AudioProcessorEditor
{
public:
    ZeeEditGui(PluginProcessorBase& pluginProcessor, juce::AudioProcessorValueTreeState& valueTreeState);
    ~ZeeEditGui() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<WidgetPanel> m_widgetPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZeeEditGui)
};
