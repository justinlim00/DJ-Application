/*
  ==============================================================================

    PlaylistComponent.h
    Created: 4 Mar 2024 7:37:49pm
    Author:  Justin  Lim

  ==============================================================================
*/

// Start of Added Code file "PlaylistComponent", everything in the code below is added on top of starter code
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "WaveformDisplay.h"
#include "Customisation.h"

class PlaylistComponent :   public juce::Component,
public juce::TableListBoxModel,
public juce::Button::Listener,
public juce::TextEditor::Listener
{
public:
    PlaylistComponent(juce::AudioFormatManager& _formatManager,
                      DJAudioPlayer*      _player,
                      DeckGUI*            _deckGUI1,
                      DeckGUI*            _deckGUI2);
    ~PlaylistComponent() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    /** Returns the number of rows in the table */
    int getNumRows() override;
    
    /** Provides graphic code that draws the background of a row in the table */
    void paintRowBackground (juce::Graphics &,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;
    
    /**  Graphical code which draws the contents of individual cells */
    void paintCell (juce::Graphics&,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    
    /** To create or update a custom component to go in a cell */
    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component* existingComponentToUpdate) override;
    
    /** Retrieve the audio duration of a song */
    void retrieveAudioDurationData();
    
    /** Compute the raw audio duration data to minutes and seconds */
    juce::String computeAudioDuration (double lengthInSeconds);
    
    /** Load existing songs*/
    void loadExistingSongsToLibrary();
    
    /** Add songs */
    void addNewSongsToLibrary();
    
    /** Search songs */
    void searchLibrary();
    
    /** Search the library when the return key is pressed */
    void textEditorReturnKeyPressed (juce::TextEditor &) override;
    
    /** Returns the text editor to its default state when user clicks away */
    void textEditorFocusLost (juce::TextEditor &) override;
    
    /** Parse the song URL to the deckGUI component */
    void loadSongToDeck (DeckGUI* deckGUI, int id);
    
    /** Delete sonr */
    void deleteSongFromLibrary (int id);
    
    /** Button listener to check if a button is clicked */
    void buttonClicked (juce::Button* button) override;
    
private:
    // A formatManager objects that handles audio formats and decide which one to use to open a file
    juce::AudioFormatManager& formatManager;
    
    // A player object that points to the DJAudioPlayer
    DJAudioPlayer* player;
    
    // Two deckGUI objects that points to the DeckGUI
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    
    // A customLookAndFeel object to manage visual UI elements
    Customisation customisation;
    juce::TextButton addButton {"Add song +"};
    juce::TextEditor searchBar;
    juce::TableListBox tableComponent;
    
    // Storing of files/folders/songs
    juce::File musicFolder = juce::File::getSpecialLocation (juce::File::userDesktopDirectory).getFullPathName() + "/music-folder";
        juce::Array<juce::File> audioFileArray;
    std::vector<juce::String> audioFileNames;
    std::vector<juce::String> audioFileDurations;
    juce::String songSelected;
    
    // Colours
    juce::Colour lightGrey = juce::Colour (61, 61, 61);
    juce::Colour darkGrey  = juce::Colour (39, 39, 39);
    juce::Colour green     = juce::Colour (0, 179, 0);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
// End of Added Code
