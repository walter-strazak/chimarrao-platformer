#include "PlayerAnimator.h"

#include "AnimationsFromSettingsLoader.h"
#include "GetProjectPath.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/InvalidAnimatorConfigFile.h"

namespace graphics::animation
{

PlayerAnimator::PlayerAnimator(graphics::GraphicsId graphicsIdInit,
                               std::shared_ptr<graphics::RendererPool> rendererPoolInit,
                               const AnimatorSettings& animatorSettings, AnimationType animationTypeInit,
                               AnimationDirection animationDirectionInit)
    : graphicsId{graphicsIdInit},
      rendererPool{std::move(rendererPoolInit)},
      currentAnimationType{animationTypeInit},
      currentAnimationDirection{animationDirectionInit},
      animatorName{"player"}
{
    if (animatorSettings.animatorName != animatorName)
    {
        throw exceptions::InvalidAnimatorConfigFile{"Invalid settings for " + animatorName + ": " +
                                                    animatorSettings.animatorName};
    }

    initializeAnimations(animatorSettings.animationsSettings);

    if (not containsAnimation(currentAnimationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(currentAnimationType) +
                                                    " is not supported in " + animatorName};
    }

    rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTexturePath());
}

void PlayerAnimator::update(const utils::DeltaTime& deltaTime)
{
    const auto animationMovedToAnotherTexture = animations.at(currentAnimationType).update(deltaTime);

    if (textureNeedsToBeChanged(animationMovedToAnotherTexture))
    {
        const utils::Vector2f scale = (currentAnimationDirection == AnimationDirection::Left) ?
                                          utils::Vector2f(-1.5f, 1.5f) :
                                          utils::Vector2f(1.5f, 1.5f);
        rendererPool->setTexture(graphicsId, animations.at(currentAnimationType).getCurrentTexturePath(),
                                 scale);
        newAnimationTypeIsSet = false;
        newAnimationDirectionIsSet = false;
    }
}
bool PlayerAnimator::textureNeedsToBeChanged(const AnimationChanged animationMovedToAnotherTexture) const
{
    return animationMovedToAnotherTexture || newAnimationTypeIsSet || newAnimationDirectionIsSet;
}

void PlayerAnimator::setAnimation(AnimationType animationType)
{
    setAnimation(animationType, currentAnimationDirection);
}

void PlayerAnimator::setAnimation(AnimationType animationType, AnimationDirection animationDirection)
{
    if (not containsAnimation(animationType))
    {
        throw exceptions::AnimationTypeNotSupported{"Animation of type: " + toString(animationType) +
                                                    " is not supported in " + animatorName};
    }

    if (currentAnimationType != animationType)
    {
        currentAnimationType = animationType;
        animations.at(currentAnimationType).reset();
        newAnimationTypeIsSet = true;
    }

    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
        newAnimationDirectionIsSet = true;
    }
}

void PlayerAnimator::setAnimationDirection(AnimationDirection animationDirection)
{
    // TODO: test this method
    if (currentAnimationDirection != animationDirection)
    {
        currentAnimationDirection = animationDirection;
        animations.at(currentAnimationType).reset();
        newAnimationDirectionIsSet = true;
    }
}

AnimationType PlayerAnimator::getAnimationType() const
{
    return currentAnimationType;
}

void PlayerAnimator::initializeAnimations(const AnimationsSettings& animationsSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, animationsSettings);
}

bool PlayerAnimator::containsAnimation(const AnimationType& animationType) const
{
    return animations.count(animationType);
}

}