#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Set the size of the component
    setSize(800, 600);

    // Request audio recording permissions if required
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [this](bool granted) { if (granted) setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels
        setAudioChannels(0, 2);
    }

    // Add child components and make them visible
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
}

MainComponent::~MainComponent()
{
    // Shutdown the audio device and clear the audio sources
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Add input sources to the mixer
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    // Prepare individual players
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Get the next audio block from the mixer source
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // Remove all inputs from the mixer and release resources
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();

    // Release resources for individual players
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    // Fill the component background with the window's background color
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // Calculate dimensions for child components based on the component size
    double columnW = getWidth() / 2;
    double rowH = getHeight() / 3;

    // Set bounds for child components
    deckGUI1.setBounds(0, 0, columnW, rowH * 2);
    deckGUI2.setBounds(columnW, 0, columnW, rowH * 2);
    playlistComponent.setBounds(0, rowH * 2, columnW * 2, rowH);
}
