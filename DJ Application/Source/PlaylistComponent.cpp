/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 4 Mar 2024 7:37:49pm
    Author:  Justin  Lim

  ==============================================================================
*/

// Start of Added Code file "PlaylistComponent", everything in the code below is added on top of starter code
#include <JuceHeader.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager,
                                     DJAudioPlayer*      _player,
                                     DeckGUI*            _deckGUI1,
                                     DeckGUI*            _deckGUI2
                                     ): formatManager(_formatManager),
                                        player(_player),
                                        deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2)
{
    formatManager.registerBasicFormats();
    
    // Create a new directory and store the music files if it does not exist
    musicFolder.createDirectory();
    
    // Load songs into the library
    loadExistingSongsToLibrary();
    
    // Making the table element visible
    addAndMakeVisible (tableComponent);
    addAndMakeVisible (addButton);
    addAndMakeVisible (searchBar);
    
    // Registers a listener to receive events when this button's state changes
    addButton.addListener (this);
    searchBar.addListener (this);
    
    // Add song button properties
    addButton.setLookAndFeel(&customisation);
    
    // Provide a pretext for the search bar (placeholder)
    searchBar.setText ("Search song...");
    
    // Adding column title to the table
    tableComponent.getHeader().addColumn ("Left", 1, 50);
    tableComponent.getHeader().addColumn ("Right", 2, 50);
    tableComponent.getHeader().addColumn ("Title", 3, 500);
    tableComponent.getHeader().addColumn ("Duration", 4, 100);
    tableComponent.getHeader().addColumn ("Remove", 5, 50);
    
    // Register the PlaylistComponent with the TableListBox as a TableListBoxModel
    tableComponent.setModel (this);
}

PlaylistComponent::~PlaylistComponent() {}


void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);   // placeholder text
}


// Adjusts the positions and sizes of UI elements in PlaylistComponent based on its dimensions.
void PlaylistComponent::resized()
{
    double rowH = getHeight() / 10;
    double columnW = getWidth() / 10;
    
    searchBar.setBounds      (0, 0, columnW * 5, rowH);
    
    addButton.setBounds      (columnW * 9, 0, columnW, rowH);
    
    tableComponent.setBounds (0, rowH, columnW * 10, rowH * 9);
}


// Returns the number of rows in the table
int PlaylistComponent::getNumRows()
{
    return static_cast<int>(audioFileNames.size());
}


// Graphics code for drawing of the background of a row in the table
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        // Background color for selected row
        g.fillAll(juce::Colours::darkblue);
    }
    else
    {
        // Alternate background colors for even and odd rows
        g.fillAll(rowNumber % 2 == 0 ? juce::Colours::grey : juce::Colours::darkgrey);
    }
}



// Graphic code that draws the contents of each cells
void PlaylistComponent::paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);

    // Show information based on column ID set
    switch (columnId)
    {
        case 3: // Show the name of inserted trackon clumn
            g.drawText(audioFileNames[rowNumber], 5, 0, width, height, juce::Justification::left, true);
            break;

        case 4: // Show the track duration on column
            g.drawText(audioFileDurations[rowNumber], 5, 0, width, height, juce::Justification::left, true);
            break;

        default:
            break;
    }
}


/*
    Refreshes the component for a given table cell, creating or updating buttons based on columnId.
    
    @param rowNumber             The row number of the cell.
    @param columnId              The column ID of the cell.
    @param isRowSelected         Indicates whether the row is selected.
    @param existingComponentToUpdate   The existing component to update or nullptr if creating a new one.
    
    @return                      The updated or new component for the specified cell.
*/
juce::Component* PlaylistComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    juce::String id {std::to_string (rowNumber)};
    
    if (columnId == 1)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* loadToDeckGUI1Button = new juce::TextButton ("<");
            existingComponentToUpdate = loadToDeckGUI1Button;

            loadToDeckGUI1Button -> addListener (this);
            loadToDeckGUI1Button -> setComponentID (id);
            loadToDeckGUI1Button -> setLookAndFeel (&customisation);
            
            if (rowNumber % 2 == 0)
                loadToDeckGUI1Button -> setColour (juce::TextButton::buttonColourId, lightGrey);
            else
                loadToDeckGUI1Button -> setColour (juce::TextButton::buttonColourId, darkGrey);
        }
    }
    else if (columnId == 2)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* loadToDeckGUI2Button = new juce::TextButton (">");
            existingComponentToUpdate = loadToDeckGUI2Button;

            loadToDeckGUI2Button -> addListener (this);
            loadToDeckGUI2Button -> setComponentID (id);
            loadToDeckGUI2Button -> setLookAndFeel (&customisation);
            
            if (rowNumber % 2 == 0)
                loadToDeckGUI2Button -> setColour (juce::TextButton::buttonColourId, lightGrey);
            else
                loadToDeckGUI2Button -> setColour (juce::TextButton::buttonColourId, darkGrey);
        }
    }
    else if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* removeButton = new juce::TextButton ("X");
            existingComponentToUpdate = removeButton;

            removeButton -> addListener (this);
            removeButton -> setComponentID (id);
            removeButton -> setLookAndFeel (&customisation);
            
            if (rowNumber % 2 == 0)
                removeButton -> setColour (juce::TextButton::buttonColourId, lightGrey);
            else
                removeButton -> setColour (juce::TextButton::buttonColourId, darkGrey);
        }
    }
    
    return existingComponentToUpdate;
}


// Retrieve the audio duration of a song from the meta data
void PlaylistComponent::retrieveAudioDurationData()
{
    for (auto s: audioFileArray)
    {
        if (auto reader = formatManager.createReaderFor (s))
        {
            auto lengthInSeconds = reader -> lengthInSamples / reader -> sampleRate;
            
            juce::String duration = computeAudioDuration (lengthInSeconds);
            
            audioFileDurations.push_back (duration);
            
            delete reader;
        }
    }
}


// Computing raw audio duration data to mins and secs
juce::String PlaylistComponent::computeAudioDuration (double lengthInSeconds)
{
    // Round to integers
    int roundedSeconds = std::round(lengthInSeconds);
    
    // Convert time to min and sec
    juce::String minutes = std::to_string (roundedSeconds / 60);
    juce::String seconds = std::to_string (roundedSeconds % 60);
    
    // Display the duration in format of (minutes:seconds)
    juce::String duration = minutes + " : " + seconds;
    
    return duration;
}


// Load songs from the music folder to the library folder
void PlaylistComponent::loadExistingSongsToLibrary()
{
    int numOfSongs;
    juce::String songName;
    juce::String songDuration;
    juce::URL audioURL;

    // Checks if the song library is a folder
    if (musicFolder.isDirectory())
    {
        // Find all mp3 files from music folder
        musicFolder.findChildFiles(audioFileArray, juce::File::findFiles, false, "*.mp3");
        
        // Find the number of audio files inside the music folder
        numOfSongs = musicFolder.getNumberOfChildFiles(juce::File::findFiles, "*.mp3");

        // Iterate through each file in audioFilesArray and get their file name
        for (int i = 0; i < numOfSongs; i++)
        {
            // Retrieve the song name and push it into a vector
            songName = audioFileArray[i].getFileNameWithoutExtension();
            audioFileNames.push_back (songName);
        }
        
        // Retrieve audio duration
        retrieveAudioDurationData();
    }
    else
    {
        std::cout << "No music folder found!" << std::endl;
    }
}


// Adds new songs to the library and saves them in the music folder
void PlaylistComponent::addNewSongsToLibrary()
{
    juce::String songTitle;

    // Create a file chooser dialog for selecting multiple audio files
    juce::FileChooser chooser{"Choose files to add into the music folder"};

    // Opens the file system dialog and allows the user to select multiple files
    // Added JUCE_MODAL_LOOPS_PERMITTED=1 into Proprocessors Definitions
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (juce::File &audioFile : chooser.getResults())
        {
            juce::File sourceFile(audioFile.getFullPathName());
            juce::File destinationFile(musicFolder.getFullPathName() + "/" + sourceFile.getFileName());

            // Copy the file to the music folder to persist across app resets
            if (sourceFile.copyFileTo(destinationFile))
            {
                DBG("File copied successfully");
            }
            else
            {
                DBG("Failed to copy file");
            }
            // Set song title for current audio
            songTitle = audioFile.getFileNameWithoutExtension();
            // Add song title to vector
            audioFileNames.push_back(songTitle);
            // Add audioFile to array
            audioFileArray.add(audioFile);
            // Retrieve and process data
            retrieveAudioDurationData();
        }
    }
}



// Searches the library for songs based on the search input
void PlaylistComponent::searchLibrary()
{
    juce::String searchInput = searchBar.getText();

    // Check if the song exists in the library
    bool doesSongExist = std::find(audioFileNames.begin(), audioFileNames.end(), searchInput) != audioFileNames.end();

    // Check for the location of the song
    auto songLocation = std::find(audioFileNames.begin(), audioFileNames.end(), searchInput);

    // When song exists
    if (doesSongExist)
    {
        std::cout << "searchInput: " << searchInput << ". Song found!" << std::endl;

        // Use std::distance to get the index
        int index = static_cast<int>(std::distance(audioFileNames.begin(), songLocation));

        tableComponent.selectRow(index);

        repaint();
    }
    else
    {
        std::cout << "searchInput: " << searchInput << ". No matching songs found." << std::endl;
    }
}



// Search the library when the return key is pressed
void PlaylistComponent::textEditorReturnKeyPressed (juce::TextEditor& searchBar)
{
    searchLibrary();
}


// Return the text editor to its default state when user clicks away
void PlaylistComponent::textEditorFocusLost (juce::TextEditor& searchBar)
{
    searchBar.setText ("Search song...");
}


// Parsing of song URL
void PlaylistComponent::loadSongToDeck(DeckGUI* deckGUI, int id)
{
    juce::URL path = juce::URL (audioFileArray[id]);
    deckGUI -> loadAudio (path);
    
    juce::String songSelected = audioFileNames[id];
    deckGUI -> updateSongNameLabel (songSelected);
    
    juce::String songDuration = audioFileDurations[id];
    deckGUI -> updateSongDurationLabel (songDuration);
}


// Delete song from library and music folder
void PlaylistComponent::deleteSongFromLibrary(int id)
{
    audioFileArray[id].moveToTrash();
    
    audioFileArray.remove (id);
    audioFileNames.erase (audioFileNames.begin() + id);
    audioFileDurations.erase (audioFileDurations.begin() + id);
}


// Button listener to handle button clicks
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // If the "Add" button is clicked
    if (button == &addButton)
    {
        addNewSongsToLibrary();
        tableComponent.updateContent();
    }
    
    // If the "Load to Left GUI" button is clicked
    if (button->getButtonText() == "<")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        loadSongToDeck(deckGUI1, id);
    }
    
    // If the "Load to Right GUI" button is clicked
    if (button->getButtonText() == ">")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        loadSongToDeck(deckGUI2, id);
    }
    
    // If the "Remove" button is clicked
    if (button->getButtonText() == "X")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        deleteSongFromLibrary(id);
        tableComponent.updateContent();
        DBG("PlaylistComponent::removeButton clicked " << id);
    }
}

// End of Added Code
