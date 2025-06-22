// Created by David Bizon on 09/06/2025.
#pragma once

#include "../ParameterMap.h" // WidgetUnit
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h" // juce::AudioProcessorValueTreeState

namespace settings {
struct WidgetPanel;
}

class IWidgetWithLabel;
class MidiParameterMap;

class WidgetPanel : public juce::Component
{
public:
    explicit WidgetPanel(const settings::WidgetPanel& panel, juce::AudioProcessorValueTreeState& valueTreeState, MidiParameterMap& midiParameterMap);
    ~WidgetPanel() override;

    int getLabelHeight() const;

    void paint(juce::Graphics& g) override;

private:
    void populateWidgets(const settings::WidgetPanel& panel, MidiParameterMap& midiParameterMap);

    std::unique_ptr<IWidgetWithLabel> createRotary(const std::string& parameterID) const;
    std::unique_ptr<IWidgetWithLabel> createToggle(const std::string& parameterID) const;
    std::unique_ptr<IWidgetWithLabel> createSelect(const std::string& parameterID, const std::vector<std::string>& labels) const;

private:
    juce::Label m_panelLabel;
    juce::AudioProcessorValueTreeState& m_valueTreeState;
    std::vector<std::unique_ptr<IWidgetWithLabel>> m_widgets;
    WidgetUnit m_configuredPanelWidth;
};
