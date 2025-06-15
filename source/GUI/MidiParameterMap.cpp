// Created by David Bizon on 14/06/2025.
#include "MidiParameterMap.h"

#include "../ParameterMap.h"
#include "juce_audio_basics/juce_audio_basics.h" // juce::MidiMessage
#include "juce_audio_processors/juce_audio_processors.h" // juce::RangedAudioParameter

MidiParameterMap::MidiParameterMap() = default;

MidiParameterMap::~MidiParameterMap() = default;

void MidiParameterMap::registerParameter(const settings::MidiConfig& midiConfig, const settings::ValueRange& valueRange, juce::RangedAudioParameter& parameter)
{
    if (midiConfig.midiMessageType == settings::MidiMessageType::CC)
    {
        if (m_midiCcMap.empty())
        {
            m_midiCcMap.resize(16); // 16 MIDI channels
        }
        auto& ccNumberList = m_midiCcMap[midiConfig.channel - 1];
        if (ccNumberList.empty())
        {
            ccNumberList.resize(128, {}); // 128 MIDI CC numbers
        }
        ccNumberList[midiConfig.ccNumber] = { &parameter, static_cast<float>(valueRange.minValue) };
    }
}

void MidiParameterMap::setParameterValue(const juce::MidiMessage& midiMessage) const
{
    if (midiMessage.isController())
    {
        if (m_midiCcMap.empty())
        {
            return; // No MIDI CC registered
        }

        auto& ccList = m_midiCcMap[midiMessage.getChannel() - 1];
        if (ccList.empty())
        {
            return; // No CCs registered for this channel
        }

        MidiParameter parameter = ccList[midiMessage.getControllerNumber()];
        if (parameter.parameter == nullptr)
        {
            return; // No parameter registered for this CC on this channel
        }

        float value = static_cast<float>(midiMessage.getControllerValue());
        if (parameter.parameter->getNumSteps() > 1)
        {
            value -= parameter.minValue;
            value /= (parameter.parameter->getNumSteps() - 1);
        }
        else
        {
            value = 1.0f; // If only one step, set to maximum
        }
        parameter.parameter->setValueNotifyingHost(value);
    }
}