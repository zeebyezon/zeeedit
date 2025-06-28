#include "PluginProcessorBase.h"
#include "AudioStub.h"

//==============================================================================
PluginProcessorBase::PluginProcessorBase() :
    AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
{
}

PluginProcessorBase::~PluginProcessorBase() = default;

//==============================================================================
const juce::String PluginProcessorBase::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessorBase::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessorBase::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessorBase::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessorBase::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
void PluginProcessorBase::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback initialization that you need
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void PluginProcessorBase::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any spare memory, etc.
}

bool PluginProcessorBase::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void PluginProcessorBase::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Actually, the buffer has zero channels because this is a MIDI effect. So this call is not necessary.
    AudioStub::passthroughAudio(buffer, getTotalNumInputChannels(), getTotalNumOutputChannels());

    processMidiMessages(midiMessages);
}

//==============================================================================
bool PluginProcessorBase::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

//==============================================================================
void PluginProcessorBase::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ValueTree state = getParameters().copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessorBase::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(getParameters().state.getType()))
        {
            getParameters().replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}
