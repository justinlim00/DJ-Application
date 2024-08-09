/*
  ==============================================================================

    WaveformDisplay.h
    Created: 4 Mar 2024 7:36:51pm
    Author:  Justin  Lim

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

class WaveformDisplay : public juce::Component,
//added component ChangeListener,ChangeBroadcaster
public juce::ChangeListener,
public juce::ChangeBroadcaster
{
public:
    WaveformDisplay (DJAudioPlayer*       player,
                     juce::AudioFormatManager&  formatManagerToUse,
                     juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();
    void paint(juce::Graphics&) override;
    void resized() override;

    /** Repaints the UI when there is a change in the waveform display */
    void changeListenerCallback (ChangeBroadcaster *source) override;
    
    /** Loads an audio URL and reads the data. Uses an URL as parameter */
    void loadURL (juce::URL audioURL);
    
    /** Set the relative position of the playhead. Accepts a double as parameter */
    void setPositionRelative (double pos);
    
    // Start of added code
    /** Set a range */
    void setRange (juce::Range<double> newRange);
    
    /** When user holds down the mouse button */
    void mouseDown (const juce::MouseEvent& e) override;
    
    /** When user holds down the mouse button and drags the cursor */
    void mouseDrag (const juce::MouseEvent& e) override;
    
    /** When user releases the mouse button */
    void mouseUp (const juce::MouseEvent&) override;
    
    /** Calculate the relative X position of the track based on the time */
    float timeToX (const double time) const;
    
    double xToTime (const float x) const;
    void updateCursorPosition();
    
private:
    bool fileLoaded;
    bool isFollowingTransport = false;
    double position;
    
    // Point to DJAudioPlayer
    DJAudioPlayer* player;
    
    juce::AudioThumbnail thumbnail;
    juce::Range<double> visibleRange;
    juce::DrawableRectangle currentPositionMarker;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
// End of added code 
