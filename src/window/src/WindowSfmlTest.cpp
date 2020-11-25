#include "WindowSfml.h"

#include "gtest/gtest.h"
#include "WindowObserverMock.h"
#include "WindowObservationHandlerMock.h"

using namespace ::testing;
using namespace window;

namespace
{
const utils::Vector2u windowSize{800, 600};
const utils::Vector2u windowSizeChanged{1000, 600};
const std::string windowTitle{"window"};
const Resolution resolution{50, 100};
const DisplayMode initialDisplayMode{DisplayMode::Window};
const Resolution initialResolution{windowSize.x, windowSize.y};
const bool initialVsync{true};
const unsigned int initialFrameLimit{120};
const DisplayMode changedDisplayMode{DisplayMode::Fullscreen};
const Resolution changedResolution{windowSizeChanged.x, windowSizeChanged.y};
const bool changedVsync{false};
const unsigned int changedFrameLimit{60};
const WindowSettings initialWindowSettings{initialDisplayMode, initialResolution, initialVsync,
                                           initialFrameLimit};
const WindowSettings windowSettingsWithDisplayModeChanged{changedDisplayMode, initialResolution, initialVsync,
                                           initialFrameLimit};
const WindowSettings windowSettingsWithResolutionChanged{initialDisplayMode, changedResolution, initialVsync,
                                                          initialFrameLimit};
const WindowSettings windowSettingsWithVsyncChanged{initialDisplayMode, initialResolution, changedVsync,
                                                          initialFrameLimit};
const WindowSettings windowSettingsWithFrameLimitChanged{initialDisplayMode, initialResolution, initialVsync,
                                                         changedFrameLimit};
}

class WindowSfmlTest : public Test
{
public:
    std::shared_ptr<StrictMock<WindowObserverMock>> observer1 =
    std::make_shared<StrictMock<WindowObserverMock>>();
    std::unique_ptr<WindowObservationHandlerMock> observationHandlerInit =
        std::make_unique<StrictMock<WindowObservationHandlerMock>>();
    WindowObservationHandlerMock* observationHandler = observationHandlerInit.get();
    WindowSfml window{windowSize, windowTitle, std::move(observationHandlerInit)};
};

//TODO: uncomment this test
//TEST_F(WindowSfmlTest, givenWindowChangeSizeEvent_shouldNotfiyObservers)
//{
//    EXPECT_CALL(*observationHandler, notifyObservers(windowSizeChanged));
//}

TEST_F(WindowSfmlTest, shouldRegisterObserver)
{
    EXPECT_CALL(*observationHandler, registerObserver(observer1.get()));

    window.registerObserver(observer1.get());
}

TEST_F(WindowSfmlTest, shouldRemoveObserver)
{
    EXPECT_CALL(*observationHandler, removeObserver(observer1.get()));

    window.removeObserver(observer1.get());
}

TEST_F(WindowSfmlTest, getWindowSettings)
{
    const auto actualWindowSettings = window.getWindowSettings();

    ASSERT_EQ(actualWindowSettings, initialWindowSettings);
}

TEST_F(WindowSfmlTest, setDisplayMode)
{
    window.setDisplayMode(changedDisplayMode);

    const auto actualWindowSettings = window.getWindowSettings();

    ASSERT_EQ(actualWindowSettings, windowSettingsWithDisplayModeChanged);
}

TEST_F(WindowSfmlTest, setResolution)
{
    window.setResolution(changedResolution);

    const auto actualWindowSettings = window.getWindowSettings();

    ASSERT_EQ(actualWindowSettings, windowSettingsWithResolutionChanged);
}

TEST_F(WindowSfmlTest, setVsync)
{
    window.setVerticalSync(changedVsync);

    const auto actualWindowSettings = window.getWindowSettings();

    ASSERT_EQ(actualWindowSettings, windowSettingsWithVsyncChanged);
}

TEST_F(WindowSfmlTest, setFramerateLimit)
{
    window.setFramerateLimit(changedFrameLimit);

    const auto actualWindowSettings = window.getWindowSettings();

    ASSERT_EQ(actualWindowSettings, windowSettingsWithFrameLimitChanged);
}

