#pragma once

#include "GameState.h"
#include "Window.h"
#include "Player.h"

namespace game
{
class MainGameState : public GameState
{
public:
    explicit MainGameState(std::shared_ptr<graphics::Window>, InputManager&);

    void update(const utils::DeltaTime&) override;
    void render(sf::RenderTarget*) override;
    void checkIfEnded() override;

private:
    std::unique_ptr<Entity> player;
};
}