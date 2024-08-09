/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 4 Mar 2024 7:36:32pm
    Author:  Justin  Lim

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer (juce::AudioFormatManager& _formatManager) : formatManager(_formatManager) {}

DJAudioPlayer::~DJAudioPlayer() {}


void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay  (samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay    (samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    reverbSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    // Start of Added Code
    reverbSource.releaseResources();
    // End of Added Code
}

// Start of Added Code
// Start audio playback
void DJAudioPlayer::start()
{
    transportSource.start();
    
    songIsPlaying = true;
}


// Stop audio playback
void DJAudioPlayer::stop()
{
    transportSource.stop();
    
    songIsPlaying = false;
}


// Looping
void DJAudioPlayer::setLooping(bool shouldLoop)
{
    if (readerSource == nullptr)
    {
        return; 
    }

    readerSource->setLooping(shouldLoop);
    loopState = shouldLoop;
}



// Load an audio URL and reads the data. Accepts an URL as parameter
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    juce::URL::InputStreamOptions options(juce::URL::ParameterHandling::inAddress);  // Adjust with the appropriate parameter
    // Set any required options for the input stream

    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(options));

    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));

        newSource->setLooping(loopState);

        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

        readerSource.reset(newSource.release());
    }
}


// Set the volume of the audio playback
void DJAudioPlayer::setGain (double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain (gain);
    }
}


// Set the speed of the audio playback
void DJAudioPlayer::setSpeed (double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio (ratio);
    }
}


// Set reverb parameters based on user input
void DJAudioPlayer::setReverbParameters(double parameter, double minValue, double maxValue, juce::String errorMessage)
{
    // Check if the parameter is within the valid range
    if (parameter < minValue || parameter > maxValue)
    {
        DBG(errorMessage);
    }
    else
    {
        // Update the corresponding reverb parameter and apply changes
        reverbParameters.roomSize = parameter;
        reverbSource.setParameters(reverbParameters);
    }
}

// Set the reverb room size
void DJAudioPlayer::setRoomSize(double size)
{
    setReverbParameters(size, 0.0, 1.0, "DJAudioPlayer::setRoomSize size should be between 0 and 1.0");
}

// Set the damping effect
void DJAudioPlayer::setDamping(double dampingRatio)
{
    setReverbParameters(dampingRatio, 0.0, 1.0, "DJAudioPlayer::setDamping amount should be between 0 and 1.0");
}

// Set the position in the audio playback
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

// Set the relative position of the playhead
void DJAudioPlayer::setPositionRelative(double pos)
{
    // Check if pos is within valid range [0, 1.0]
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        // Calculate the absolute position in seconds based on the relative position
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        // Set the calculated absolute position
        setPosition(posInSecs);
    }
}

// Get the relative position of the playhead
double DJAudioPlayer::getPositionRelative()
{
    // Return the relative position of the playhead
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}



// Get the current position of the playback
double DJAudioPlayer::getCurrentPosition()
{
    return transportSource.getCurrentPosition();
}

// Retrives the value from the zoom slider from the DeckGUI
void DJAudioPlayer::retrieveZoomValue (double amount)
{
    if (amount < 0 || amount > 1.0)
    {
        std::cout << "DJAudioPlayer::retrieveZoomValue amount should be between 0 and 1" << std::endl;
    }
    else
    {
        zoomValue = amount;
    }
}


// Retrieves the audio length of a song
double DJAudioPlayer::getAudioLength()
{
    return transportSource.getLengthInSeconds();
}
// End of Added Code
