#include "gtest/gtest.h"
#include "../IniFile.h"

class IniFileFixture : public ::testing::Test
{
    protected:
        IniFile iniFile;

        void SetUp() override
        {
            iniFile.set("section1", "key1", "value1");
            iniFile.set("section1", "key2", "value2");
            iniFile.set("section2", "key1", "value1");
        }

        void TearDown() override
        {
            iniFile.deleteSection("section1");
            iniFile.deleteSection("section2");
        }
};

TEST_F(IniFileFixture, VerifyInitialSetup)
{
    EXPECT_EQ(iniFile.get("section1", "key1"), "value1");
    EXPECT_EQ(iniFile.get("section1", "key2"), "value2");
    EXPECT_EQ(iniFile.get("section2", "key1"), "value1");
}

TEST_F(IniFileFixture, ModifyValues)
{
    iniFile.set("section1", "key1", "new_value1");
    EXPECT_EQ(iniFile.get("section1", "key1"), "new_value1");
}

TEST_F(IniFileFixture, DeleteKeysAndSections)
{
    EXPECT_TRUE(iniFile.deleteKey("section1", "key1"));
    EXPECT_FALSE(iniFile.hasKey("section1", "key1"));

    EXPECT_TRUE(iniFile.deleteSection("section2"));
    EXPECT_FALSE(iniFile.hasSection("section2"));
}

TEST_F(IniFileFixture, SaveAndLoadWithFixture)
{
    const std::string testFileName = "fixture_test.ini";
    iniFile.save(testFileName);

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section1", "key1"), "value1");
    EXPECT_EQ(loadedIniFile.get("section1", "key2"), "value2");
    EXPECT_EQ(loadedIniFile.get("section2", "key1"), "value1");

    std::remove(testFileName.c_str());
}

TEST_F(IniFileFixture, CommentsHandling)
{
    iniFile.set("section1", "key1", "value1");
    EXPECT_TRUE(iniFile.setSectionComment("section1", "; This is a section comment\n"));
    EXPECT_TRUE(iniFile.setKeyComment("section1", "key1", "; This is a key comment\n"));

    const std::string testFileName = "comments_test.ini";
    iniFile.save(testFileName);

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section1", "key1"), "value1");
    EXPECT_EQ(loadedIniFile.getSectionComment("section1"), "; This is a section comment\n");
    EXPECT_EQ(loadedIniFile.getKeyComment("section1", "key1"), "; This is a key comment\n");

    std::remove(testFileName.c_str());
}

TEST_F(IniFileFixture, CheckKeyPresenceAcrossSections)
{
    std::vector<std::string> expectedSections1 = {"section1", "section2"};
    std::vector<std::string> expectedSections2 = {"section2"};
    std::vector<std::string> expectedSections3 = {};

    EXPECT_EQ(iniFile.hasKey("key1"), expectedSections1);
    EXPECT_TRUE(iniFile.deleteKey("section1", "key1"));
    EXPECT_EQ(iniFile.hasKey("key1"), expectedSections2);
    EXPECT_TRUE(iniFile.deleteKey("section2", "key1"));
    EXPECT_EQ(iniFile.hasKey("key1"), expectedSections3);
}
