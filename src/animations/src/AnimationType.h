#pragma once

#include <iostream>
#include <unordered_map>

#include "StringHelper.h"
#include "exceptions/AnimationTypeNotFound.h"

namespace animations
{
enum class AnimationType
{
    Idle,
    Walk,
    Jump
};

inline std::string toString(AnimationType animationType)
{
    std::unordered_map<AnimationType, std::string> animationTypeToString{
        {AnimationType::Idle, "Idle"}, {AnimationType::Walk, "Walk"}, {AnimationType::Jump, "Jump"}};

    try
    {
        return animationTypeToString.at(animationType);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::AnimationTypeNotFound{e.what()};
    }
}

inline AnimationType toAnimationType(const std::string& animationTypeAsString)
{
    const auto animationTypeLowerString = utils::getLowerCases(animationTypeAsString);

    std::unordered_map<std::string, AnimationType> stringToAnimationType{
        {"idle", AnimationType::Idle}, {"walk", AnimationType::Walk}, {"jump", AnimationType::Jump}};

    try
    {
        return stringToAnimationType.at(animationTypeLowerString);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::AnimationTypeNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, AnimationType animationType)
{
    return os << "AnimationType::" << toString(animationType);
}

}