// Created by David Bizon on 09/06/2025.
#pragma once

namespace juce {
template <typename T>
class AudioBuffer;
}

class AudioStub
{
public:
    AudioStub();
    ~AudioStub();

    static void passthroughAudio(juce::AudioBuffer<float>& buffer, int nbInputChannels, int nbOutputChannels);
};
