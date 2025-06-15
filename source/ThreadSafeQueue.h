// Created by David Bizon on 15/06/2025.
#pragma once

#include "juce_audio_basics/juce_audio_basics.h" // MidiMessage
#include "juce_core/juce_core.h" // AbstractFifo

template<class T>
class ThreadSafeQueue
{
public:
    explicit ThreadSafeQueue(int queueSize) :
        m_abstractFifo(queueSize),
        m_midiMessageBuffer(queueSize)
    {
        jassert(queueSize > 0);
    }

    ~ThreadSafeQueue() = default;

    bool push(T t)
    {
        const auto scope = m_abstractFifo.write(1);

        if (scope.blockSize1 > 0)
            m_midiMessageBuffer[scope.startIndex1] = std::move(t);
        else if (scope.blockSize2 > 0)
            m_midiMessageBuffer[scope.startIndex2] = std::move(t);
        else
            return false;

        return true;
    }

    void popAll(std::function<void(T)> readFunction)
    {
        while (m_abstractFifo.getNumReady() > 0)
        {
            const auto scope = m_abstractFifo.read(1);

            if (scope.blockSize1 > 0)
                readFunction(std::move(m_midiMessageBuffer[scope.startIndex1]));
            if (scope.blockSize2 > 0)
                readFunction(std::move(m_midiMessageBuffer[scope.startIndex2]));
        }
    }

private:
    juce::AbstractFifo m_abstractFifo;
    std::vector<T> m_midiMessageBuffer;
};
