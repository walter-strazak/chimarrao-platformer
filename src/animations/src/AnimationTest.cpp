#include "Animation.h"

#include "gtest/gtest.h"

#include "exceptions/TexturesNotInitialized.h"

using namespace ::testing;
using namespace animations;

class AnimationTest : public Test
{
public:
    std::vector<std::string> textures{"1", "2", "3"};
    std::vector<std::string> emptyTextures{};
    float timeBetweenTextures = 60;
    Animation animation{textures, timeBetweenTextures};
};

TEST_F(AnimationTest, creationOfAnimationWithEmptyTextures_shouldThrowTexturesNotInitialized)
{
    ASSERT_THROW(Animation(emptyTextures, timeBetweenTextures), exceptions::TexturesNotInitialized);
}

TEST_F(AnimationTest, newAnimationShouldReturnFirstFrame)
{
    ASSERT_EQ(textures[0], animation.getCurrentTexturePath());
}

TEST_F(AnimationTest, aniationShouldReturnNextTexture)
{
    const auto animationChanged = animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_TRUE(animationChanged);
    ASSERT_EQ(textures[1], animation.getCurrentTexturePath());
}

TEST_F(AnimationTest, animationShouldNotReturnNextTexture)
{
    const auto animationChanged = animation.update(utils::DeltaTime(timeBetweenTextures - 1));

    ASSERT_FALSE(animationChanged);
    ASSERT_EQ(textures[0], animation.getCurrentTexturePath());
}

TEST_F(AnimationTest, animationShouldReturnFirstTextureAfterLast)
{
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_EQ(textures[0], animation.getCurrentTexturePath());
}
