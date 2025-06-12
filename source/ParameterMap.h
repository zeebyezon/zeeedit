// Created by David Bizon on 10/06/2025.
#pragma once
#include "NumericType.h"
#include <string>
#include <vector>

/**
 * WidgetUnit is used to define the width of a widget in a panel.
 * A rotary widget is 2 units wide, a button is 1 unit wide, ...
 */
NUMERIC_TYPE(WidgetUnit, int)

inline int toPixels(WidgetUnit unit, int scale)
{
    return unit.getRawValue() * scale;
}

namespace settings {

enum class MidiMessageType
{
    CC
};

struct MidiConfig
{
    MidiMessageType midiMessageType{MidiMessageType::CC};
    int channel{0}; // MIDI channel
    int ccNumber{0}; // MIDI CC number
};

struct ValueRange
{
    int minValue{0};
    int maxValue{127};
    int increment{1};
    int defaultValue{0};
    std::vector<std::string> labels; // Optional labels for values
};

struct WidgetDescription
{
    std::string name;
    MidiConfig midiConfig;
    ValueRange valueRange;
};

struct PanelDescription
{
    std::string name;
    WidgetUnit width{8}; // Maximum width of a widget line on this panel: when reached, a new line is started
};

struct WidgetPanel
{
    PanelDescription panel;
    std::vector<WidgetDescription> widgets;
};

}

class ParameterMap
{
public:
    static const std::vector<settings::WidgetPanel>& getPanels();
    static std::string generateParameterID(const std::string& panelName, const std::string& widgetName);

private:
    static std::string removeSpaces(const std::string& input);
};
