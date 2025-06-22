// Created by David Bizon on 12/06/2025.
#pragma once

#include <string>
#include <functional>

namespace juce {
class Component;
class Label;
class AudioProcessorValueTreeState;
}

enum class WidgetType
{
    ROTARY,
    TOGGLE,
    SELECT
};

class IWidgetWithLabel
{
public:
    virtual ~IWidgetWithLabel() = default;
    [[nodiscard]] virtual juce::Component& getComponent() = 0;
    [[nodiscard]] virtual juce::Label& getLabel() = 0;
    [[nodiscard]] virtual WidgetType getWidgetType() const = 0;
};

template<class Component, class Label, class Attachment>
struct WidgetWithLabel final : IWidgetWithLabel
{
    Component widget;
    Label label;
    Attachment attachment;
    WidgetType widgetType;

    WidgetWithLabel(WidgetType type, juce::AudioProcessorValueTreeState& valueTreeState, const std::string& parameterID) :
        attachment(valueTreeState, parameterID, widget),
        widgetType(type)
    {
    }

    /**
     * Provide a function to initialize the widget before passing it to the attachment.
     */
    WidgetWithLabel(WidgetType type, juce::AudioProcessorValueTreeState& valueTreeState, const std::string& parameterID, std::function<void(Component& widget)> initWidget) :
        attachment(valueTreeState, parameterID, (initWidget(widget), widget)),
        widgetType(type)
    {}

    ~WidgetWithLabel() override = default;
    [[nodiscard]] juce::Component& getComponent() override { return widget; }
    [[nodiscard]] juce::Label& getLabel() override { return label; }
    [[nodiscard]] WidgetType getWidgetType() const override { return widgetType; }
};
