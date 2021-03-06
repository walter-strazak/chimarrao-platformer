#include "DefaultAnimatorSettingsRepository.h"

#include "gtest/gtest.h"

#include "AnimatorSettingsReaderMock.h"

#include "GetProjectPath.h"

using namespace ::testing;
using namespace animations;

class DefaultAnimatorSettingsRepositoryTest : public Test
{
public:
    const std::string projectPath{utils::getProjectPath("chimarrao-platformer")};
    const utils::FilePath settingsPath{projectPath + "config/animators.yaml"};
    const std::string existingAnimatorName{"existingAnimatorName"};
    const std::string nonExistingAnimatorName{"nonExistingAnimatorName"};
    const AnimatorsSettings emptyAnimatorsSettings{};
    const AnimatorSettings animatorSettings{existingAnimatorName, {}};
    const AnimatorsSettings animatorsSettings{{animatorSettings}};
    std::unique_ptr<StrictMock<AnimatorSettingsReaderMock>> settingsReaderInit =
        std::make_unique<StrictMock<AnimatorSettingsReaderMock>>();
    StrictMock<AnimatorSettingsReaderMock>* settingsReader = settingsReaderInit.get();
};

TEST_F(DefaultAnimatorSettingsRepositoryTest, creationOfRepository_shouldReadSettingsFromSettingsReader)
{
    EXPECT_CALL(*settingsReader, readAnimatorsSettings(settingsPath))
        .WillOnce(Return(emptyAnimatorsSettings));

    DefaultAnimatorSettingsRepository settingsRepository{std::move(settingsReaderInit)};
}

TEST_F(DefaultAnimatorSettingsRepositoryTest, givenNonExistingAnimatorName_shouldReturnNone)
{
    EXPECT_CALL(*settingsReader, readAnimatorsSettings(settingsPath)).WillOnce(Return(animatorsSettings));
    DefaultAnimatorSettingsRepository settingsRepository{std::move(settingsReaderInit)};

    const auto actualAnimatorSettings = settingsRepository.getAnimatorSettings(existingAnimatorName);

    ASSERT_EQ(*actualAnimatorSettings, animatorSettings);
}

TEST_F(DefaultAnimatorSettingsRepositoryTest, givenExistingAnimatorName_shouldReturnAnimatorSettings)
{
    EXPECT_CALL(*settingsReader, readAnimatorsSettings(settingsPath)).WillOnce(Return(animatorsSettings));
    DefaultAnimatorSettingsRepository settingsRepository{std::move(settingsReaderInit)};

    const auto actualAnimatorSettings = settingsRepository.getAnimatorSettings(nonExistingAnimatorName);

    ASSERT_EQ(actualAnimatorSettings, boost::none);
}