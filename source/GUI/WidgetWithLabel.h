// Created by David Bizon on 12/06/2025.
#pragma once

namespace juce {
class Component;
class Label;
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

template<class ComponentType, class AttachmentType>
struct WidgetWithLabel final : IWidgetWithLabel
{
    ComponentType widget;
    juce::Label label;
    std::unique_ptr<AttachmentType> attachment;
    WidgetType widgetType;

    explicit WidgetWithLabel(WidgetType type) : widgetType(type) {}
    ~WidgetWithLabel() override = default;
    [[nodiscard]] juce::Component& getComponent() override { return widget; }
    [[nodiscard]] juce::Label& getLabel() override { return label; }
    [[nodiscard]] WidgetType getWidgetType() const override { return widgetType; }
};
