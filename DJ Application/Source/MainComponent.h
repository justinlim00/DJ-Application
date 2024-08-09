#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Format manager for handling audio formats
    juce::AudioFormatManager formatManager;

    // Audio thumbnail cache managing other audio thumbnail objects
    juce::AudioThumbnailCache thumbCache{ 100 };

    // Mixer source for handling the output mixing of other AudioSources
    juce::MixerAudioSource mixerSource;

    // DJAudioPlayer instances
    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };
    DJAudioPlayer player{ formatManager };

    // DeckGUI instances
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    // PlaylistComponent instance
    PlaylistComponent playlistComponent{ formatManager, &player, &deckGUI1, &deckGUI2 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
