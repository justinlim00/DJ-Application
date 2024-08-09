/*
  ==============================================================================

    Customisation.h
    Created: 4 Mar 2024 7:37:18pm
    Author:  Justin  Lim

  ==============================================================================
*/

// Start of Added Code file "Customisation", everything in the code below is added on top of starter code
#pragma once

#include <JuceHeader.h>

class Customisation :   public juce::LookAndFeel_V4
{
public:
    Customisation(){};
    ~Customisation(){};
    
    /** Customisation linear sliders */
    void drawLinearSlider (juce::Graphics& g,
                           int       x,
                           int       y,
                           int       width,
                           int       height,
                           float     sliderPos,
                           float     minSliderPos,
                           float     maxSliderPos,
                           const     juce::Slider::SliderStyle,
                           juce::Slider&   slider
                           ) override;
    
    /** Customisation linear slider thmb */
    void drawLinearSliderThumb (juce::Graphics& g,
                                int       x,
                                int       y,
                                int       width,
                                int       height,
                                float     sliderPos,
                                float     minSliderPos,
                                float     maxSliderPos,
                                const     juce::Slider::SliderStyle style,
                                juce::Slider&   slider
                                ) override;
    
    /** Customisation linear slider background */
    void drawLinearSliderBackground (juce::Graphics& g,
                                     int       x,
                                     int       y,
                                     int       width,
                                     int       height,
                                     float     /*sliderPos*/,
                                     float     /*minSliderPos*/,
                                     float     /*maxSliderPos*/,
                                     const     juce::Slider::SliderStyle /*style*/,
                                     juce::Slider&   slider
                                     ) override;
    
    /** Customisation any button background */
    void drawButtonBackground (juce::Graphics& g,
                               juce::Button&   button,
                               const     juce::Colour& backgroundColour,
                               bool      isMouseOverButton,
                               bool      isButtonDown
                               ) override;

private:
    //==============================================================================
    // Colours
    juce::Colour offWhite  = juce::Colour::fromFloatRGBA (0.83f, 0.84f, 0.9f,  1.0f);
    juce::Colour grey      = juce::Colour::fromFloatRGBA (0.42f, 0.42f, 0.42f, 1.0f);
    juce::Colour orange    = juce::Colour(255, 165, 0);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Customisation);
};
// End of Added Code
