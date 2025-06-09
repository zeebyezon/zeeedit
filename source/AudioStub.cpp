// Created by David Bizon on 09/06/2025.

#include "AudioStub.h"

#include "juce_audio_basics/juce_audio_basics.h"

AudioStub::AudioStub() = default;

AudioStub::~AudioStub() = default;

void AudioStub::passthroughAudio(juce::AudioBuffer<float>& buffer, int nbInputChannels, int nbOutputChannels)
{
    juce::ScopedNoDenormals noDenormals;
    for (auto i = nbInputChannels; i < nbOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
}
