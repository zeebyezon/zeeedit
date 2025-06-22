// Created by David Bizon on 14/06/2025.
#pragma once
#include <vector>

namespace settings {
struct MidiConfig;
struct ValueRange;
}

namespace juce {
class MidiMessage;
class RangedAudioParameter;
}

/**
 * Map midi messages to parameters.
 */
class MidiParameterMap
{
public:
    MidiParameterMap();
    ~MidiParameterMap();

    void registerParameter(const settings::MidiConfig& midiConfig, juce::RangedAudioParameter& parameter);
    void setParameterValue(const juce::MidiMessage& midiMessage) const;

private:
    std::vector<std::vector<juce::RangedAudioParameter*>> m_midiCcMap; // by channel, then by CC number
};
