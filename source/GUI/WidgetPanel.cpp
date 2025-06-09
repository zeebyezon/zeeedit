// Created by David Bizon on 09/06/2025.

#include "WidgetPanel.h"

WidgetPanel::WidgetPanel(juce::AudioProcessorValueTreeState& valueTreeState) :
    m_valueTreeState(valueTreeState)
{
    addAndMakeVisible(m_cutoffSlider);
    m_cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_cutoffSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    m_cutoffSlider.setSize(60, 80);
    m_cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(m_valueTreeState, "cutoff", m_cutoffSlider);

    addAndMakeVisible(m_cutoffLabel);
    m_cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    m_cutoffLabel.attachToComponent(&m_cutoffSlider, false);

    setSize(400, 300);
}

void WidgetPanel::resized()
{
    int top = 30;
    int left = 20;
    m_cutoffSlider.setTopLeftPosition(left, top);
}
