// Created by David Bizon on 12/06/2025.
#pragma once

#include "../ParameterMap.h" // WidgetUnit
#include <juce_graphics/juce_graphics.h>

namespace juce {
class Component;
}

class IWidgetWithLabel;

class LayoutProcessor
{
public:
    explicit LayoutProcessor(WidgetUnit panelWidth);
    explicit LayoutProcessor(int panelWidth);

    void reset();
    void insert(IWidgetWithLabel& widget);
    void insert(juce::Component& component, int labelHeight);
    [[nodiscard]] juce::Point<int> getSize() const;

    static int getValueTextBoxHeight();

private:
    int m_configuredPanelWidth = 0; // Width of the panel in pixels, used to determine when to start a new line
    juce::Point<int> m_topLeft;
    juce::Point<int> m_maxBottomRight;
};
