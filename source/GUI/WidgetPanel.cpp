// Created by David Bizon on 09/06/2025.

#include "WidgetPanel.h"

#include "LayoutProcessor.h"
#include "WidgetWithLabel.h"
#include "MidiParameterMap.h"
#include "../ParameterMap.h"

WidgetPanel::WidgetPanel(const settings::WidgetPanel& panel, juce::AudioProcessorValueTreeState& valueTreeState, MidiParameterMap& midiParameterMap) :
    m_valueTreeState(valueTreeState),
    m_configuredPanelWidth(panel.panel.width)
{
    m_panelLabel.setText(panel.panel.name, juce::dontSendNotification);
    m_panelLabel.setJustificationType(juce::Justification::centred);
    m_panelLabel.attachToComponent(this, false);

    populateWidgets(panel, midiParameterMap);

    LayoutProcessor layoutProcessor(m_configuredPanelWidth);
    for (const std::unique_ptr<IWidgetWithLabel>& widget : m_widgets)
    {
        layoutProcessor.insert(*widget);
    }

    setSize(layoutProcessor.getSize().getX(), layoutProcessor.getSize().getY());
}

WidgetPanel::~WidgetPanel() = default;

int WidgetPanel::getLabelHeight() const
{
    return m_panelLabel.getHeight();
}

void WidgetPanel::paint(juce::Graphics& g)
{
    // Draw a border
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10, 2);
}

void WidgetPanel::populateWidgets(const settings::WidgetPanel& panel, MidiParameterMap& midiParameterMap)
{
    m_widgets.reserve(panel.widgets.size());
    for (const auto& widget : panel.widgets)
    {
        const std::string& parameterID = ParameterMap::generateParameterID(panel.panel.name, widget.name);
        std::unique_ptr<IWidgetWithLabel> widgetWithLabel;

        if (!widget.valueRange.labels.empty())
        {
            widgetWithLabel = createSelect(parameterID, widget.valueRange.labels);
        }
        else if ((widget.valueRange.maxValue - widget.valueRange.minValue) / widget.valueRange.increment == 1)
        {
            widgetWithLabel = createToggle(parameterID);
        }
        else
        {
            widgetWithLabel = createRotary(parameterID);
        }

        addAndMakeVisible(widgetWithLabel->getLabel());
        widgetWithLabel->getLabel().setText(widget.name, juce::dontSendNotification);
        widgetWithLabel->getLabel().setJustificationType(juce::Justification::centred);
        widgetWithLabel->getLabel().attachToComponent(&widgetWithLabel->getComponent(), false);

        // register fo modification on input midi messages
        midiParameterMap.registerParameter(widget.midiConfig, widget.valueRange, *m_valueTreeState.getParameter(parameterID));

        m_widgets.push_back(std::move(widgetWithLabel));
    }
}

std::unique_ptr<IWidgetWithLabel> WidgetPanel::createRotary(const std::string& parameterID)
{
    auto widgetWithLabel = std::make_unique<WidgetWithLabel<juce::Slider, juce::Label, juce::AudioProcessorValueTreeState::SliderAttachment>>
        (WidgetType::ROTARY, m_valueTreeState, parameterID);

    addAndMakeVisible(widgetWithLabel->widget);
    widgetWithLabel->widget.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

    return widgetWithLabel;
}

std::unique_ptr<IWidgetWithLabel> WidgetPanel::createToggle(const std::string& parameterID)
{
    auto widgetWithLabel = std::make_unique<WidgetWithLabel<juce::ToggleButton, juce::Label, juce::AudioProcessorValueTreeState::ButtonAttachment>>
        (WidgetType::TOGGLE, m_valueTreeState, parameterID);

    addAndMakeVisible(widgetWithLabel->widget);

    return widgetWithLabel;
}

std::unique_ptr<IWidgetWithLabel> WidgetPanel::createSelect(const std::string& parameterID, const std::vector<std::string>& labels)
{
    auto widgetWithLabel = std::make_unique<WidgetWithLabel<juce::ComboBox, juce::Label, juce::AudioProcessorValueTreeState::ComboBoxAttachment>>
        (WidgetType::SELECT, m_valueTreeState, parameterID, [&labels](juce::ComboBox& selector) {
        int index = 0;
        for (const std::string& item : labels)
        {
            if (item.empty())
                continue;
            selector.addItem(item, ++index); // Add items with 1-based index
        }
    });

    addAndMakeVisible(widgetWithLabel->widget);

    return widgetWithLabel;
}
