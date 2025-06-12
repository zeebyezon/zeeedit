// Created by David Bizon on 12/06/2025.
#include "LayoutProcessor.h"

#include "WidgetWithLabel.h"
#include "juce_gui_basics/juce_gui_basics.h"

constexpr int WIDGET_UNIT = 36; // Widget unit in pixels
constexpr int VALUE_HEIGHT = 20; // Height of a text value
constexpr int LABEL_HEIGHT = 10; // Height of a text label
constexpr int H_SPACING = 12; // Horizontal spacing between widgets
constexpr int V_SPACING = 8; // Vertical spacing between widgets

int LayoutProcessor::getValueTextBoxHeight()
{
    return VALUE_HEIGHT;
}

LayoutProcessor::LayoutProcessor(WidgetUnit panelWidth) :
    LayoutProcessor((H_SPACING + WIDGET_UNIT) * panelWidth.getRawValue() + H_SPACING)
{
}

LayoutProcessor::LayoutProcessor(int panelWidth) :
    m_configuredPanelWidth(panelWidth),
    m_topLeft(H_SPACING, V_SPACING),
    m_maxBottomRight(m_topLeft)
{
}

void LayoutProcessor::reset()
{
    m_topLeft = {H_SPACING, V_SPACING};
    m_maxBottomRight = m_topLeft;
}

void LayoutProcessor::insert(IWidgetWithLabel& widget)
{
    int widgetWidth;
    int widgetHeight;

    switch (widget.getWidgetType())
    {
    case WidgetType::ROTARY:
        widgetWidth = 2 * WIDGET_UNIT;
        widgetHeight = 2 * WIDGET_UNIT + VALUE_HEIGHT;
        if (const auto slider = dynamic_cast<juce::Slider*>(&widget.getComponent()))
        {
            slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, widgetWidth, VALUE_HEIGHT);
        }
        break;
    case WidgetType::TOGGLE:
        widgetWidth = WIDGET_UNIT;
        widgetHeight = WIDGET_UNIT;
        break;
    case WidgetType::SELECT:
        widgetWidth = 2 * WIDGET_UNIT;
        widgetHeight = VALUE_HEIGHT;
        break;
    default:
        return; // Unsupported widget type, do nothing
    }

    widget.getComponent().setSize(widgetWidth, widgetHeight);
    insert(widget.getComponent(), widget.getLabel().getHeight());
}

void LayoutProcessor::insert(juce::Component& component, int labelHeight)
{
    auto bottomRight = m_topLeft.translated(component.getWidth() + H_SPACING, component.getHeight() + labelHeight + V_SPACING);
    if (bottomRight.x > m_configuredPanelWidth)
    {
        m_topLeft.x = H_SPACING;
        m_topLeft.y = m_maxBottomRight.y;
        bottomRight = m_topLeft.translated(component.getWidth() + H_SPACING, component.getHeight() + labelHeight + V_SPACING);
    }

    component.setTopLeftPosition(m_topLeft + juce::Point<int>(0, labelHeight));

    if (m_maxBottomRight.x < bottomRight.x)
    {
        m_maxBottomRight.x = bottomRight.x;
    }
    if (m_maxBottomRight.y < bottomRight.y)
    {
        m_maxBottomRight.y = bottomRight.y;
    }

    m_topLeft.x = bottomRight.x;
}

juce::Point<int> LayoutProcessor::getSize() const
{
    return m_maxBottomRight;
}
