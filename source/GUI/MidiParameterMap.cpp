// Created by David Bizon on 14/06/2025.
#include "MidiParameterMap.h"

#include "../ParameterMap.h"
#include "juce_audio_basics/juce_audio_basics.h" // juce::MidiMessage
#include "juce_audio_processors/juce_audio_processors.h" // juce::RangedAudioParameter

MidiParameterMap::MidiParameterMap() = default;

MidiParameterMap::~MidiParameterMap() = default;

void MidiParameterMap::registerParameter(const settings::MidiConfig& midiConfig, juce::RangedAudioParameter& parameter)
{
    if (midiConfig.midiMessageType == settings::MidiMessageType::CC)
    {
        if (m_midiCcMap.empty())
        {
            m_midiCcMap.resize(1 + 16); // global channel + 16 explicit channels
        }
        auto& ccNumberList = m_midiCcMap[midiConfig.channel]; // MIDI channels are 1-based, channel 0 is the global channel
        if (ccNumberList.empty())
        {
            ccNumberList.resize(128, nullptr); // 128 MIDI CC numbers
        }
        ccNumberList[midiConfig.ccNumber] = &parameter;
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

        juce::RangedAudioParameter* parameter;

        const std::vector<juce::RangedAudioParameter*>& ccList = m_midiCcMap[midiMessage.getChannel()];
        if (!ccList.empty())
        {
            parameter = ccList[midiMessage.getControllerNumber()];
        }
        else
        {
            const std::vector<juce::RangedAudioParameter*>& ccList0 = m_midiCcMap[0]; // global channel
            if (!ccList0.empty())
            {
                parameter = ccList0[midiMessage.getControllerNumber()];
            }
            else
            {
                return; // No CCs registered for this channel
            }
        }

        if (parameter == nullptr)
        {
            return; // No parameter registered for this CC on this channel
        }

        auto value = static_cast<float>(midiMessage.getControllerValue());
        value = parameter->convertTo0to1(value);
        parameter->setValueNotifyingHost(value);
    }
}