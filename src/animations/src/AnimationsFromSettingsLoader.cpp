#include "AnimationsFromSettingsLoader.h"

#include "GetProjectPath.h"
#include "IncrementalFilePathsCreator.h"
#include "exceptions/AnimationsFromSettingsNotFound.h"

namespace animations
{

void AnimationsFromSettingsLoader::loadAnimationsFromSettings(
    std::unordered_map<AnimationType, Animation>& animations, const AnimationsSettings& animationsSettings)
{
    if (animationsSettings.empty())
    {
        throw exceptions::AnimationsFromSettingsNotFound{"Animations not found in settings"};
    }

    animations.clear();
    animations.reserve(animationsSettings.size());

    for (const auto& animationSettings : animationsSettings)
    {
        const auto animationType = toAnimationType(animationSettings.animationType);
        const auto texturePaths = utils::IncrementalFilePathsCreator::createFilePaths(
            utils::getProjectPath("chimarrao-platformer") + animationSettings.firstTexturePath,
            animationSettings.numberOfTextures);
        const auto timeBetweenTextures = animationSettings.timeBetweenTexturesInSeconds;
        animations.insert({animationType, Animation{texturePaths, timeBetweenTextures}});
    }
}
}