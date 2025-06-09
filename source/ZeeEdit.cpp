// Created by David Bizon on 09/06/2025.

#include "ZeeEdit.h"

ZeeEdit::ZeeEdit() :
    PluginProcessorBase(),
    m_parameters(*this, nullptr, "parameters", createParameterLayout())
{
    m_parameters.addParameterListener("cutoff", &addParameterListener(7, 73));
}

ZeeEdit::~ZeeEdit() = default;

juce::AudioProcessorValueTreeState::ParameterLayout ZeeEdit::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    params.add(std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 0.0f, 120.0f, 60.0f));
    return params;
}

class ParameterListener : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ParameterListener(ZeeEdit& zeeEdit, int channel, int ccNumber) :
        m_parent(zeeEdit),
        m_channel(channel),
        m_ccNumber(ccNumber)
    {
    }

    ~ParameterListener() override = default;

    void parameterChanged(const juce::String& /*parameterID*/, float newValue) override
    {
        m_parent.pushMessage(juce::MidiMessage::controllerEvent(m_channel, m_ccNumber, static_cast<int>(newValue)));
    }

private:
    ZeeEdit& m_parent;
    int m_channel;
    int m_ccNumber;
};

juce::AudioProcessorValueTreeState::Listener& ZeeEdit::addParameterListener(int channel, int ccNumber)
{
    m_parameterListeners.push_back(std::make_unique<ParameterListener>(*this, channel, ccNumber));
    return *m_parameterListeners.back();
}

void ZeeEdit::pushMessage(juce::MidiMessage message)
{
    m_midiMessageQueue.push(std::move(message));
}

void ZeeEdit::processMidiMessages(juce::MidiBuffer& midiMessages)
{
    m_midiMessageQueue.popAll([&midiMessages](const juce::MidiMessage& midiMessage)
    {
        midiMessages.addEvent(midiMessage, 0);
    });
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZeeEdit();
}
