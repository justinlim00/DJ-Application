/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 4 Mar 2024 7:36:51pm
    Author:  Justin  Lim

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay (DJAudioPlayer*       player,
                                  juce::AudioFormatManager&  formatManagerToUse,
                                  juce::AudioThumbnailCache& cacheToUse
                                  ): fileLoaded(false),
position(0),
player(player),
thumbnail(1000, formatManagerToUse, cacheToUse)
{
    // Audio thumbnail
    thumbnail.addChangeListener(this);
    
    // Position marker
    addAndMakeVisible (currentPositionMarker);
    currentPositionMarker.setFill (juce::Colours::white.withAlpha (0.85f));
}

WaveformDisplay::~WaveformDisplay() {}

// Added font on top of the starter code
void WaveformDisplay::paint(juce::Graphics& g)
{
    // Clear the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw a grey outline around the component
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    // Set the font for the waveform display
    g.setFont(juce::Font("Verdana", 16.0f, juce::Font::bold));

    // Set the colour of the waveform
    g.setColour(juce::Colours::lightblue);

    if (fileLoaded)
    {
        thumbnail.drawChannel(g, getLocalBounds(), 0, thumbnail.getTotalLength(), 0, 1.0f);
        updateCursorPosition();
    }
    else
    {
        g.setFont(juce::Font("Verdana", 20.0f, juce::Font::bold));

        g.drawFittedText("No audio file selected", getLocalBounds(), juce::Justification::centred, 2);
    }
}


void WaveformDisplay::resized() {}


// Added this function to change the listener call back
// Repaints the UI when waveform display changes
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster* source)
{
    std::cout << "wfd: change received!" << std::endl;
    
    repaint();
}


// Loads an audio URL and reads the data
void WaveformDisplay::loadURL(juce::URL audioURL)
{
    thumbnail.clear();

    fileLoaded = thumbnail.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded)
    {
        std::cout << "wfd: loaded!" << std::endl;

        juce::Range<double> newRange(0.0, thumbnail.getTotalLength());

        setRange(newRange);

        repaint();
    }
    else
    {
        std::cout << "wfd: not loaded!" << std::endl;
    }
}


// Added function on top of the starter code
// Set the playhead position relative to the waveform
void WaveformDisplay::setPositionRelative(double relativePosition)
{
    if (relativePosition != position && !std::isnan(relativePosition))
    {
        position = relativePosition;
        repaint();
    }
}


// Added function on top of the starter code
// Set the visible range for the waveform display
void WaveformDisplay::setRange(juce::Range<double> newRange)
{
    visibleRange = newRange;
    updateCursorPosition();
    repaint();
}


// Added function on top of the starter code
void WaveformDisplay::mouseDown(const juce::MouseEvent& e)
{
    mouseDrag(e);
}

// Added function on top of the starter code
void WaveformDisplay::mouseDrag(const juce::MouseEvent& e)
{
    player->setPosition(fmax(0.0, xToTime(static_cast<float>(e.x))));
}

// Added function on top of the starter code
void WaveformDisplay::mouseUp(const juce::MouseEvent&)
{
    player->start();
}

// Added function on top of the starter code
float WaveformDisplay::timeToX(const double time) const
{
    // Ensure a valid visible range to avoid division by zero
    if (visibleRange.getLength() <= 0)
        return 0;

    // Calculate and return the X position relative to the visible range
    return static_cast<float>(getWidth() * ((time - visibleRange.getStart()) / visibleRange.getLength()));
}

// Added function on top of the starter code
double WaveformDisplay::xToTime(const float x) const
{
    // Calculate and return the time based on the X position and visible range
    return (x / static_cast<float>(getWidth())) * visibleRange.getLength() + visibleRange.getStart();
}

// Added function on top of the starter code
void WaveformDisplay::updateCursorPosition()
{
    // Calculate the X position based on the current playback position
    float x = timeToX(player->getCurrentPosition());
    float y = 0;
    float width = 1.5f;
    float height = static_cast<float>(getHeight());

    // Set the visibility and position of the cursor marker
    currentPositionMarker.setVisible(player->songIsPlaying || isMouseButtonDown());
    currentPositionMarker.setRectangle(juce::Rectangle<float>(x, y, width, height));
}

