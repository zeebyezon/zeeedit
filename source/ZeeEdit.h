// Created by David Bizon on 09/06/2025.
#pragma once

#include "PluginProcessorBase.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "ThreadSafeQueue.h"

namespace juce {
class AudioProcessor;
class AudioPlayHead;
class MidiBuffer;
}

struct BankAndPG
{
    int bankMsb;
    int bankLsb;
    int programNumber;
};

class ZeeEdit : public PluginProcessorBase
{
public:
    ZeeEdit();
    ~ZeeEdit() override;

    juce::AudioProcessorEditor* createEditor() override;
    ThreadSafeQueue<juce::MidiMessage>& getInputMidiMessageQueue() { return m_inputMidiMessageQueue; }
    ThreadSafeQueue<BankAndPG>& getInputProgramChangeQueue() { return m_inputProgramChangeQueue; }

    void pushOutputMessage(juce::MidiMessage message);
    void sendProgramChange(int bankMsb, int bankLsb, int programNumber);

    int getGlobalMidiChannel() const;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

protected:
    juce::AudioProcessorValueTreeState& getParameters() override { return m_parameters; }
    void processMidiMessages(juce::MidiBuffer& midiMessages) override;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void createParameterListeners();
    juce::AudioProcessorValueTreeState::Listener& createParameterListener(int channel, int ccNumber);

private:
    juce::AudioProcessorValueTreeState m_parameters;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::Listener>> m_parameterListeners;
    ThreadSafeQueue<juce::MidiMessage> m_outputMidiMessageQueue;

    ThreadSafeQueue<juce::MidiMessage> m_inputMidiMessageQueue;

    ThreadSafeQueue<BankAndPG> m_inputProgramChangeQueue;
    int m_receivedBankMsb = 0;
    int m_receivedBankLsb = 0;
};
