/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 4 Mar 2024 7:36:32pm
    Author:  Justin  Lim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
public:
    //==============================================================================
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    // Start of Added Code
    /** Start audio playback */
    void start();
    
    /** Stop audio playback */
    void stop();
    
    /** Set loop state to true or false */
    void setLooping(bool shouldLoop);
    
    /** Load an audio URL and reads the data */
    void loadURL (juce::URL audioURL);
    
    /** Set the volume of the audio playback */
    void setGain (double gain);
    
    /** Set the speed of the audio playback  */
    void setSpeed (double ratio);
    
    /** Set reverb parameters based on user input **/
    void setReverbParameters(double parameter, double minValue, double maxValue, juce::String errorMessage);;
    
    /** Set the reverb room size */
    void setRoomSize (double size);
    
    /** Set the damping effect */
    void setDamping (double dampingRatio);
    
    /** Set the position in the audio playback */
    void setPosition (double posInSecs);
    
    /** Set the relative position of the playhead */
    void setPositionRelative (double pos);
    
    /** Get the relative position of the playhead */
    double getPositionRelative();
    
    /** Get the current position of the playback */
    double getCurrentPosition();
    
    /** Retrives the value from the zoom slider from the DeckGUI */
    void retrieveZoomValue (double amount);
    
    /** Retrieves the audio length of a song */
    double getAudioLength ();
    
    /** Store the song status of the DJ player */
    bool songIsPlaying;
    
    /** Store the zoom value from the zoom slider in a variable */
    double zoomValue;
    
    /** Store the current loop state */
    bool loopState = false;
    
private:
private:
    //Manages audio formats and determines which file to open
    juce::AudioFormatManager& formatManager;

    // Utilizes an AudioFormatReader to read audio data
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    // Takes a PositionableAudioSource and allows certain actions to be executed
    juce::AudioTransportSource transportSource;

    // Takes an input source and alter its sample rate
    juce::ResamplingAudioSource resampleSource {&transportSource, false, 2};

    // Applies reverb using the Reverb class to enhance another AudioSource
    juce::ReverbAudioSource reverbSource { &resampleSource, false };

    juce::Reverb::Parameters reverbParameters;
};

// End of Added Code
