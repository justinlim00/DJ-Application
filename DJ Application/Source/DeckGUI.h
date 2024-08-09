/*
  ==============================================================================

    DeckGUI.h
    Created: 4 Mar 2024 7:37:03pm
    Author:  Justin  Lim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "Customisation.h"

class DeckGUI : public juce::Component,
public juce::Button::Listener,
public juce::Slider::Listener,
public juce::FileDragAndDropTarget,
public juce::Timer
{
public:
    //==============================================================================
    DeckGUI(DJAudioPlayer*       player,
            juce::AudioFormatManager&  formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI();
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    // Below are added code on top of starter code
    /** Button listener to check if a button is clicked */
    void buttonClicked (juce::Button*) override;
    
    /** Slider listener to check if a slider is used */
    void sliderValueChanged (juce::Slider* slider) override;
    
    /** For DeckGUI component to register for and receive drag events */
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    
    /** For DeckGUI component to register for and receive drop events */
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    
    /** Show the current playback position and update the play head */
    void timerCallback() override;
    
    /** Loads  URL into the player and draws the waveform */
    void loadAudio (juce::URL audioURL);
    
    /** Update song name label */
    void updateSongNameLabel (juce::String songSelected);
    
    /** Update song duration label*/
    void updateSongDurationLabel (juce::String songDuration);
    
private:
    // ( Added code on top of starter code)
    void initializeUIElements();
    void initializeLookAndFeel();
    
    // Object that points to the DJAudioPlayer  ( Added code on top of starter code)
    DJAudioPlayer* player;
    
    // Manipulate the waveform display  ( Added code on top of starter code)
    WaveformDisplay waveformDisplay;
    
    // Manage visual UI elements  ( Added code on top of starter code)
    Customisation customisation;
    
    // Text buttons  ( Added code on top of starter code)
    juce::TextButton playStopButton {"PLAY"};
    juce::TextButton loopButton     {"ENABLE LOOP"};
    
    
    // Sliders ( Added code on top of starter code)
    juce::Slider volSlider            { juce::Slider::LinearVertical,   juce::Slider::NoTextBox };
    juce::Slider speedSlider          { juce::Slider::LinearVertical,   juce::Slider::NoTextBox };
    juce::Slider reverbSlider { juce::Slider::LinearVertical,   juce::Slider::NoTextBox };
    juce::Slider dampingSlider        { juce::Slider::LinearVertical,   juce::Slider::NoTextBox };
    
    // Labels  ( Added code on top of starter code)
    juce::Label volLabel            { {}, "Volume" };
    juce::Label speedLabel          { {}, "Speed"};
    juce::Label reverbLabel { {}, "Room size" };
    juce::Label dampingLabel        { {}, "Damping" };
    juce::Label songNameLabel;
    juce::Label songDurationLabel;

    // Colours  ( Added code on top of starter code)
    juce::Colour grey        = juce::Colour::fromFloatRGBA (0.42f, 0.42f, 0.42f, 1.0f);
    juce::Colour green       = juce::Colour (27, 126, 60);
    juce::Colour red         = juce::Colour (153, 102, 102);
    juce::Colour lightOrange = juce::Colour (179, 119, 0);
    juce::Colour darkOrange  = juce::Colour (102, 68, 0);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
