#pragma once

#include <memory>
#include <unordered_map>

#include "Animation.h"
#include "AnimationDirection.h"
#include "AnimationType.h"
#include "Animator.h"
#include "AnimatorSettings.h"
#include "GraphicsId.h"
#include "RendererPool.h"

namespace animations
{
class PlayerAnimator : public Animator
{
public:
    PlayerAnimator(graphics::GraphicsId, std::shared_ptr<graphics::RendererPool>, const AnimatorSettings&,
                   AnimationType = AnimationType::Idle, AnimationDirection = AnimationDirection::Right);

    AnimationChanged update(const utils::DeltaTime&) override;
    void setAnimation(AnimationType) override;
    void setAnimation(AnimationType, AnimationDirection) override;
    void setAnimationDirection(AnimationDirection) override;
    AnimationType getAnimationType() const override;
    AnimationDirection getAnimationDirection() const override;

private:
    void initializeAnimations(const AnimationsSettings&);
    bool containsAnimation(const AnimationType&) const;
    bool animationChanged(TextureChanged) const;

    graphics::GraphicsId graphicsId;
    std::shared_ptr<graphics::RendererPool> rendererPool;
    std::unordered_map<AnimationType, Animation> animations;
    AnimationType currentAnimationType;
    AnimationDirection currentAnimationDirection;
    std::string animatorName;
    bool newAnimationTypeIsSet;
    bool newAnimationDirectionIsSet;
};
}