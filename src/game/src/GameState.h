#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"

namespace game
{
class GameState : public State, public input::InputObserver
{
public:
    explicit GameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                       const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&);
    ~GameState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;
    void pause();

private:
    const input::InputStatus* inputStatus;
    bool paused;
    utils::Timer timer;
    const float timeAfterStateCouldBePaused;
    std::shared_ptr<components::core::ComponentOwner> player;
    std::shared_ptr<components::core::ComponentOwner> background;
};
}