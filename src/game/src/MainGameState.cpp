#include "MainGameState.h"

#include "AnimationComponent.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "KeyboardMovementComponent.h"
#include "animation/AnimatorSettingsYamlReader.h"
#include "animation/PlayerAnimator.h"

namespace game
{

MainGameState::MainGameState(std::shared_ptr<graphics::Window> window,
                             std::shared_ptr<input::InputManager> inputManagerInit,
                             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                             std::shared_ptr<physics::PhysicsEngine> physicsEngineInit)
    : GameState{std::move(window), std::move(inputManagerInit), std::move(rendererPoolInit),
                std::move(physicsEngineInit)}
{
    //    auto graphicsId = rendererPool->acquire({5, 5}, {10, 10},
    //                                            utils::getProjectPath("chimarrao-platformer") +
    //                                                "resources/Player/Idle/idle-with-weapon-1.png");
    //    auto physicsId = physicsEngine->acquire({5, 5}, {10, 10});

    auto animatorsSettings = graphics::animation::AnimatorSettingsYamlReader::readAnimatorsSettings(
        utils::getProjectPath("chimarrao-platformer") + "config/animators.yaml");

    // TODO: add animatorsSettings base for all animators from vector to concrete animator
    // TODO: clean up
    if (animatorsSettings)
    {
        auto playerAnimatorSettings =
            std::find_if(animatorsSettings->cbegin(), animatorsSettings->cend(),
                         [](const graphics::animation::AnimatorSettings& animatorSettings) {
                             return animatorSettings.animatorName == "player";
                         });

        if (playerAnimatorSettings != animatorsSettings->cend())
        {
            player = std::make_shared<components::ComponentOwner>(utils::Vector2f{10, 10});
            auto graphicsComponent = player->addComponent<components::GraphicsComponent>(
                rendererPool, utils::Vector2f{5, 5}, utils::Vector2f{10, 10});
            auto graphicsId = graphicsComponent->getGraphicsId();
            player->addComponent<components::KeyboardMovementComponent>(inputManager);
            auto playerAnimator = std::make_shared<graphics::animation::PlayerAnimator>(
                graphicsId, rendererPool, *playerAnimatorSettings);
            player->addComponent<components::AnimationComponent>(playerAnimator);

//            player = std::make_unique<Player>(graphicsId, rendererPool, physicsId,
//            physicsEngine,
//                                              std::make_unique<graphics::animation::PlayerAnimator>(
//                                                  graphicsId, rendererPool,
//                                                  *playerAnimatorSettings));

//            auto* playerAsObserver = dynamic_cast<Player*>(player.get());
//            if (playerAsObserver)
//            {
//                inputManager.registerObserver(playerAsObserver);
//            }
        }
    }

    player->awake();
//    initialize();
}

void MainGameState::initialize()
{
    player->initialize();
}

void MainGameState::update(const utils::DeltaTime& deltaTime)
{
    player->update(deltaTime);
    physicsEngine->update(deltaTime);
}

void MainGameState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    player->lateUpdate(deltaTime);
}

void MainGameState::render()
{
    rendererPool->renderAll();
}

void MainGameState::checkIfEnded() {}

}
