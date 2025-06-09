// Created by David Bizon on 09/06/2025.
#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_audio_processors/juce_audio_processors.h" // juce::AudioProcessorValueTreeState

class WidgetPanel : public juce::Component
{
public:
    explicit WidgetPanel(juce::AudioProcessorValueTreeState& valueTreeState);

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& m_valueTreeState;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider m_cutoffSlider;
    juce::Label m_cutoffLabel;
    std::unique_ptr<SliderAttachment> m_cutoffAttachment;
};
