#include "EditorMenuState.h"

#include "GetProjectPath.h"
#include "SaveMapState.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HitboxComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto buttonSize = utils::Vector2f{25, 5};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
}

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<input::InputManager>& inputManagerInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditor{false},
      shouldBackToMenu{false},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    createBackground();
    createEditorTitle();
    createBackToEditorButton();
    createLoadMapButton();
    createNewMapButton();
    createSaveMapButton();
    createMenuButton();

    initialize();
}

EditorMenuState::~EditorMenuState()
{
    inputManager->removeObserver(this);
}

void EditorMenuState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
        button->getComponent<components::core::ClickableComponent>()->disable();
    }

    possibleLeaveFromStateTimer.start();
}

void EditorMenuState::update(const utils::DeltaTime& deltaTime)
{
    if (buttonsActionsFrozen &&
        freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
    }

    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToEditor = true;
    }

    if (shouldBackToEditor)
    {
        backToEditor();
        return;
    }

    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }

    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void EditorMenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

std::string EditorMenuState::getName() const
{
    return "Editor menu state";
}

void EditorMenuState::activate()
{
    active = true;

    for (auto& button : buttons)
    {
        button->enable();
        if (auto clickableButton = button->getComponent<components::core::ClickableComponent>())
        {
            clickableButton->disable();
        }
    }

    background->enable();
    title->enable();

    possibleLeaveFromStateTimer.restart();
}

void EditorMenuState::deactivate()
{
    active = false;

    hideGraphics();

    buttonsActionsFrozen = true;
}

void EditorMenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void EditorMenuState::hideGraphics()
{
    for (auto& button : buttons)
    {
        button->disable();
    }

    background->disable();
    title->disable();
}

void EditorMenuState::unfreezeButtons()
{
    buttonsActionsFrozen = false;
    for (auto& button : buttons)
    {
        button->getComponent<components::core::ClickableComponent>()->enable();
    }
}

void EditorMenuState::backToEditor()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void EditorMenuState::backToMenu()
{
    while (not states.empty() && states.top()->getName() != "Menu state")
    {
        states.pop();
    }

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void EditorMenuState::createEditorTitle()
{
    const auto textPausePosition = utils::Vector2f{27.5, 7};
    title = std::make_unique<components::core::ComponentOwner>(textPausePosition);
    title->addComponent<components::core::TextComponent>(rendererPool, textPausePosition, "Editor Menu",
                                                         fontPath, 40, graphics::Color::White,
                                                         utils::Vector2f{0, 0});
}

void EditorMenuState::createBackground()
{
    const auto backgroundColor = graphics::Color{172};
    background = std::make_unique<components::core::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::core::GraphicsComponent>(rendererPool, utils::Vector2f{31, 50},
                                                                  utils::Vector2f{25, 5}, backgroundColor,
                                                                  graphics::VisibilityLayer::Second);
}

void EditorMenuState::createBackToEditorButton()
{
    const auto backToGameButtonPosition = utils::Vector2f{28, 15};

    addButton(backToGameButtonPosition, "Back to editor", utils::Vector2f{1.5, 0.75},
              [this] { shouldBackToEditor = true; });
}

void EditorMenuState::createLoadMapButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 23};

    addButton(backToMenuButtonPosition, "Load map", utils::Vector2f{7, 0.75},
              [this] { std::cout << "load map\n"; });
}

void EditorMenuState::createNewMapButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 31};

    addButton(backToMenuButtonPosition, "New map", utils::Vector2f{7, 0.75},
              [this] { std::cout << "new map\n"; });
}

void EditorMenuState::createSaveMapButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 39};

    auto runSaveMapState = [&] {
        buttonsActionsFrozen = true;

        states.top()->deactivate();
        states.push(std::make_unique<SaveMapState>(window, inputManager, rendererPool, states));
    };
    addButton(backToMenuButtonPosition, "Save map", utils::Vector2f{6, 0.75}, runSaveMapState);
}

void EditorMenuState::createMenuButton()
{
    const auto backToMenuButtonPosition = utils::Vector2f{28, 47};

    addButton(backToMenuButtonPosition, "Back to menu", utils::Vector2f{2.75, 0.75},
              [this] { shouldBackToMenu = true; });
}

void EditorMenuState::addButton(const utils::Vector2f& position, const std::string& text,
                                const utils::Vector2f& textOffset, std::function<void(void)> clickAction)
{
    auto button = std::make_unique<components::core::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::core::GraphicsComponent>(
        rendererPool, buttonSize, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::core::TextComponent>(rendererPool, position, text, fontPath, 27,
                                                          textColor, textOffset);
    button->addComponent<components::core::HitboxComponent>(buttonSize);
    button->addComponent<components::core::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::core::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                               changeColorOnMouseOut);
    buttons.push_back(std::move(button));
}

}