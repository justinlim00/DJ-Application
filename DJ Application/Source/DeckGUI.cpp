/*
  ==============================================================================

    DeckGUI.cpp
    Created: 4 Mar 2024 7:37:03pm
    Author:  Justin  Lim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse)
    : player(_player),
      waveformDisplay(_player, formatManagerToUse, cacheToUse),
        customisation()
{
    initializeUIElements();
    initializeLookAndFeel();

    startTimer(500);
}
// Start of Added Code
DeckGUI::~DeckGUI()
{
    stopTimer();
}

// I have added extra UI elements on top of the one that is provided in the starter code below (label, sliders, volumn, speed, loop, damping and reverb)
void DeckGUI::initializeUIElements()
{
    addAndMakeVisible(songNameLabel);
    addAndMakeVisible(songDurationLabel);
    addAndMakeVisible(playStopButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(reverbLabel);
    addAndMakeVisible(reverbSlider);
    addAndMakeVisible(dampingLabel);
    addAndMakeVisible(dampingSlider);
    addAndMakeVisible(waveformDisplay);

    playStopButton.addListener(this);
    loopButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener          (this);
    reverbSlider.addListener (this);
    dampingSlider.addListener        (this);
    
    // Song name label properties
    songNameLabel.setText              ("No track loaded", juce::dontSendNotification);
    songNameLabel.setFont              (juce::Font("Verdana", 21.00f, juce::Font::plain));
    songNameLabel.setJustificationType (juce::Justification::left);
    songNameLabel.setEditable          (false, false, false);
    songNameLabel.setColour            (juce::Label::textColourId, juce::Colours::white);
    
    // Song name label properties
    songDurationLabel.setText              ("", juce::dontSendNotification);
    songDurationLabel.setFont              (juce::Font ("Verdana", 21.00f, juce::Font::plain));
    songDurationLabel.setJustificationType (juce::Justification::right);
    songDurationLabel.setEditable          (false, false, false);
    songDurationLabel.setColour            (juce::Label::textColourId, juce::Colours::white);
    
    // Play/stop button properties
    playStopButton.setLookAndFeel(&customisation);
    playStopButton.setColour(juce::TextButton::buttonColourId, green);
    playStopButton.setWantsKeyboardFocus(false);

    // Loop button properties
    loopButton.setLookAndFeel (&customisation);
    loopButton.setColour      (juce::TextButton::buttonColourId, lightOrange);
    
    // Volume slider properties
    volSlider.setLookAndFeel (&customisation);
    volSlider.setColour      (juce::Slider::thumbColourId, grey);
    volSlider.setRange       (0.01, 1.0);
    
    // Volume label properties
    volLabel.setText               ("Vol", juce::NotificationType::dontSendNotification);
    volLabel.setFont               (juce::Font ("Verdana", 10.00f, juce::Font::plain));
    volLabel.setJustificationType  (juce::Justification::centred);
    volLabel.setEditable           (false, false, false);
    
    // Speed slider properties
    speedSlider.setLookAndFeel (&customisation);
    speedSlider.setColour      (juce::Slider::thumbColourId, grey);
    speedSlider.setRange       (0.5, 3.0);
    
    // Speed label properties
    speedLabel.setText               ("Speed", juce::NotificationType::dontSendNotification);
    speedLabel.setFont               (juce::Font ("Verdana", 10.00f, juce::Font::plain));
    speedLabel.setJustificationType  (juce::Justification::centred);
    speedLabel.setEditable           (false, false, false);
    
    // Reverb slider properties
    reverbSlider.setLookAndFeel (&customisation);
    reverbSlider.setColour      (juce::Slider::thumbColourId, grey);
    reverbSlider.setRange       (0.0, 1.0);
    
    // Reverb properties
    reverbLabel.setText               ("Room size", juce::NotificationType::dontSendNotification);
    reverbLabel.setFont               (juce::Font ("Verdana", 10.00f, juce::Font::plain));
    reverbLabel.setJustificationType  (juce::Justification::centred);
    reverbLabel.setEditable           (false, false, false);
    
    // Damping slider properties
    dampingSlider.setLookAndFeel (&customisation);
    dampingSlider.setColour      (juce::Slider::thumbColourId, grey);
    dampingSlider.setRange       (0.0, 1.0);
    
    // Damping label properties
    dampingLabel.setText               ("Damping", juce::NotificationType::dontSendNotification);
    dampingLabel.setFont               (juce::Font ("Verdana", 10.00f, juce::Font::plain));
    dampingLabel.setJustificationType  (juce::Justification::centred);
    dampingLabel.setEditable           (false, false, false);
    
    // Starts the timer and set the length of interval to 500
    startTimer (500);
}

void DeckGUI::initializeLookAndFeel()
{
    playStopButton.setLookAndFeel(&customisation);
    playStopButton.setColour(juce::TextButton::buttonColourId, green);
    playStopButton.setWantsKeyboardFocus(false);

    loopButton.setLookAndFeel(&customisation);
    loopButton.setColour(juce::TextButton::buttonColourId, lightOrange);

    volSlider.setLookAndFeel(&customisation);
    volSlider.setColour(juce::Slider::thumbColourId, grey);
    volSlider.setRange(0.01, 1.0);
}

// Edited the DeckGUI from the starter code to cater to the needs of my DJ application
void DeckGUI::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("", getLocalBounds(), juce::Justification::centred, true);
}

// Change the resized values from the starter code to cater to the application
void DeckGUI::resized()
{
    // Start of Added Code
    double rowH = getHeight() / 11;
    double columnW = getWidth() / 11;
    
    songNameLabel.setBounds        (0, 0, columnW * 5, rowH);
    
    songDurationLabel.setBounds    (columnW * 9, 0, columnW * 2, rowH);
    
    waveformDisplay.setBounds      (0, rowH, columnW * 11, rowH * 2);
    
    playStopButton.setBounds       (0, rowH * 4, columnW * 2, rowH * 2);
    
    loopButton.setBounds           (0, rowH * 6, columnW * 2, rowH * 2);
    
    volSlider.setBounds            (columnW * 3, rowH * 4, columnW, rowH * 6);
    
    volLabel.setBounds             (columnW * 3, rowH * 10, columnW, rowH);
    
    speedSlider.setBounds          (columnW * 5, rowH * 4, columnW, rowH * 6);
    
    speedLabel.setBounds           (columnW * 5, rowH * 10, columnW, rowH);
    
    reverbSlider.setBounds (columnW * 7, rowH * 4, columnW, rowH * 6);
    
    reverbLabel.setBounds  (columnW * 7, rowH * 10, columnW, rowH);
    
    dampingSlider.setBounds        (columnW * 9, rowH * 4, columnW, rowH * 6);
    
    dampingLabel.setBounds         (columnW * 9, rowH * 10, columnW, rowH);
}

// Added certain additional function on top of the starter code
// Button listener
void DeckGUI::buttonClicked(juce::Button* button)
{
    // When play/stop button is clicked
    if (button == &playStopButton)
    {
        // Checks if a song is playing
        if (player -> songIsPlaying)
        {
            // Stop the DJ player & draw button
            player -> stop();
            playStopButton.setColour (juce::TextButton::buttonColourId, green);
            playStopButton.setButtonText ("PLAY");
        }
        else
        {
            // Starts the DJ player
            player -> start();
            
            // Draw the stop button
            playStopButton.setColour (juce::TextButton::buttonColourId, red);
            playStopButton.setButtonText ("STOP");
        }
    }
    
    // Loop button is clicked
    if (button == &loopButton)
    {
        // If the current loop state is false
        if ((player -> loopState) == false)
        {
            // Set looping to true
            player -> setLooping (true);
            
            // Draw disable loop button
            loopButton.setButtonText ("DISABLE LOOP");
            loopButton.setColour (juce::TextButton::buttonColourId, darkOrange);
        }
        else if ((player -> loopState) == true)
        {
            // Set loop to false
            player -> setLooping (false);
            
            // Draw enable loop button
            loopButton.setButtonText ("ENABLE LOOP");
            loopButton.setColour (juce::TextButton::buttonColourId, lightOrange);
        }
    }
}


// Slider listener to check if a slider is used
void DeckGUI::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player -> setGain (slider -> getValue());
    }
    if (slider == &speedSlider)
    {
        player -> setSpeed (slider -> getValue());
    }
    if (slider == &reverbSlider)
    {
        player -> setRoomSize (slider -> getValue());
    }
    if (slider == &dampingSlider)
    {
        player -> setDamping (slider -> getValue());
    }
}

// DeckGUI to register for and receive drag events
bool DeckGUI::isInterestedInFileDrag (const juce::StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    
    return true;
}

// Added the function filDropped,timerCallback,loadAudio,updateSongNameLabel, updateSongDurationLabel on top of the starter DeckGUI code
// DeckGUI to register for and receive drop events
void DeckGUI::filesDropped (const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    
    // Checks only 1 file is present and load into player
    if (files.size() == 1)
    {
        player -> loadURL (juce::URL { juce::File { files[0] } });
    }
}

// Ask the DJAudioPlayer the current playback position and update the play head
void DeckGUI::timerCallback()
{
    auto currentPlaybackPosition = player -> getPositionRelative();
    waveformDisplay.setPositionRelative (currentPlaybackPosition);
}

// Loads  URL into the player and draws the waveform
void DeckGUI::loadAudio (juce::URL audioURL)
{
    player -> loadURL (audioURL);
    waveformDisplay.loadURL (audioURL);
}

// Update song name label
void DeckGUI::updateSongNameLabel (juce::String songSelected)
{
    songNameLabel.setText (songSelected, juce::dontSendNotification);
}

// Update song duration label
void DeckGUI::updateSongDurationLabel (juce::String songDuration)
{
    songDurationLabel.setText (songDuration, juce::dontSendNotification);
}
// End of Added Code
