/*
  ==============================================================================

    Customisation.cpp
    Created: 4 Mar 2024 7:37:18pm
    Author:  Justin  Lim

  ==============================================================================
*/

// Start of Added Code file "Customisation", everything in the code below is added on top of starter code
#include "Customisation.h"

// Customisation linear sliders
void Customisation::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider &slider)
{
    // Set the colour (slider background)
    g.fillAll (slider.findColour (juce::Slider::backgroundColourId));
    
    // Set the colour (slider indictaor)
    g.setColour (offWhite);
    
    juce::Path p;
    
    // Draw the indicators on the slider
    for (int i = 0; i < 10; i++) {
        p.addRectangle(x + 10, y + 20 * i, width - 20, 1);
        g.fillPath(p);
    }
    
    // Draw the remaining components of the slider
    drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    drawLinearSliderThumb      (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}


// Customisation linear slider thmb
void Customisation::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
                                              float sliderPos, float minSliderPos, float maxSliderPos,
                                              const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto sliderRadius = (float) getSliderThumbRadius (slider);

    bool isDownOrDragging = slider.isEnabled() && (slider.isMouseOverOrDragging() || slider.isMouseButtonDown());

    auto knobColour = slider.findColour (juce::Slider::thumbColourId)
                            .withMultipliedSaturation ((slider.hasKeyboardFocus (false) || isDownOrDragging) ? 1.3f : 0.9f)
                            .withMultipliedAlpha (slider.isEnabled() ? 1.0f : 0.7f);

    g.setColour(knobColour);
    

    float kx, ky;

    kx = (float) x + (float) width * 0.5f;
    ky = sliderPos;
    g.fillRect (juce::Rectangle<float> (kx - sliderRadius, ky - 2.5f, sliderRadius * 2.0f, 15.0f));
    
    g.setColour (offWhite);
    g.fillRect  (juce::Rectangle<float> (kx - sliderRadius, ky + 5.0f, sliderRadius * 2.0f, 1.0f));
}


// Draw a custom look and feel for any linear slider background
void Customisation::drawLinearSliderBackground(juce::Graphics& g, int x, int y, int width, int height,
                                                    float /*sliderPos*/,
                                                    float /*minSliderPos*/,
                                                    float /*maxSliderPos*/,
                                                    const juce::Slider::SliderStyle /*style*/, juce::Slider& slider)
{
    auto sliderRadius = (float) getSliderThumbRadius(slider) - 5.0f;
    juce::Path on, off;

    // Draws the slider that is below thumb
    if (slider.isHorizontal())
    {
        auto iy = (float) y + (float) height * 0.5f - sliderRadius * 0.5f;
        juce::Rectangle<float> r((float) x - sliderRadius * 0.5f, iy, (float) width + sliderRadius, sliderRadius);
        auto onW = r.getWidth() * ((float) slider.valueToProportionOfLength(slider.getValue()));

        on.addRectangle(r.removeFromLeft(onW));
        off.addRectangle(r);
    }
    else
    {
        auto ix = (float) x + (float) width * 0.5f - sliderRadius * 0.5f;
        juce::Rectangle<float> r(ix, (float) y - sliderRadius * 0.5f, sliderRadius, (float) height + sliderRadius);
        auto onH = r.getHeight() * ((float) slider.valueToProportionOfLength(slider.getValue()));

        on.addRectangle(r.removeFromBottom(onH));
        off.addRectangle(r);
    }

    // Set the slider path below the thumb to be orange
    g.setColour(orange);
    g.fillPath(on);

    // Set the slider path above the thumb to be grey
    g.setColour(grey);
    g.fillPath(off);
}



/*
    Draws a customized button background with enhanced aesthetics.

    @param g                    The graphics context for drawing.
    @param button               The button to customize.
    @param backgroundColour     The base background colour of the button.
    @param isMouseOverButton    Indicates whether the mouse is over the button.
    @param isButtonDown         Indicates whether the button is currently pressed.
*/
void Customisation::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                        bool isMouseOverButton, bool isButtonDown)
{
    auto baseColour = button.findColour(juce::TextButton::buttonColourId)
        .withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
        .withMultipliedAlpha(button.isEnabled() ? 0.9f : 0.5f);

    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.1f);

    auto flatOnLeft = button.isConnectedOnLeft();
    auto flatOnRight = button.isConnectedOnRight();
    auto flatOnTop = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    auto width = static_cast<float>(button.getWidth()) - 1.0f;
    auto height = static_cast<float>(button.getHeight()) - 1.0f;

    if (width > 0 && height > 0)
    {
        auto cornerSize = fmin(15.0f, fmin(width, height) * 0.45f);
        auto lineThickness = cornerSize * 0.1f;
        auto halfThickness = lineThickness * 0.5f;

        juce::Path outline;
        outline.addRoundedRectangle(0.5f + halfThickness, 0.5f + halfThickness, width - lineThickness, height - lineThickness,
                                    cornerSize, cornerSize,
                                    !(flatOnLeft || flatOnTop),
                                    !(flatOnRight || flatOnTop),
                                    !(flatOnLeft || flatOnBottom),
                                    !(flatOnRight || flatOnBottom));

        auto outlineColour = button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
            : juce::TextButton::textColourOffId);

        g.setColour(baseColour);
        g.fillPath(outline);

        if (!button.getToggleState())
        {
            g.setColour(outlineColour);
        }
    }
}

