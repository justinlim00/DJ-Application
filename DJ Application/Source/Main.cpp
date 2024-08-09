#include <JuceHeader.h>
#include "MainComponent.h"

class OtoDecksApplication : public juce::JUCEApplication
{
public:
    OtoDecksApplication() {}

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String& commandLine) override
    {
        // Initialize the application.
        createMainWindow();
    }

    void shutdown() override
    {
        // Shutdown the application.
        mainWindow = nullptr; // (deletes our window)
    }

    void systemRequestedQuit() override
    {
        // Handle the system request to quit the application.
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        // Handle the case when another instance of the app is launched.
    }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow()
            : DocumentWindow("OtoDecks",
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                              .findColour(juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            initializeMainWindow();
        }

        void closeButtonPressed() override
        {
            // Handle the close button press.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }


// Start of Added Code segment
    private:
        void initializeMainWindow()
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
           #else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
           #endif

            setVisible(true);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    void createMainWindow()
    {
        mainWindow = std::make_unique<MainWindow>();
    }

    // End of Added Code segment
private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(OtoDecksApplication)
