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

TEST_F(IniFileFixture, CommentsInFixture)
{
    EXPECT_TRUE(iniFile.setSectionComment("section1", "; This is a section comment\n"));
    EXPECT_TRUE(iniFile.setKeyComment("section1", "key2", "; This is a key comment\n"));

    const std::string testFileName = "fixture_comments.ini";
    iniFile.save(testFileName);

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section1", "key2"), "value2");

    std::remove(testFileName.c_str());
}

TEST_F(IniFileFixture, CheckKeyPresenceAcrossSections)
{
    EXPECT_TRUE(iniFile.hasKey("key1"));
    EXPECT_TRUE(iniFile.deleteKey("key1"));
    EXPECT_FALSE(iniFile.hasKey("key1"));
}

TEST_F(IniFileFixture, CheckSectionCommentsPersistence)
{
    EXPECT_TRUE(iniFile.setSectionComment("section1", "; Section1 comment\n"));
    const std::string testFileName = "section_comments.ini";
    iniFile.save(testFileName);

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section1", "key1"), "value1");

    std::remove(testFileName.c_str());
}

TEST_F(IniFileFixture, CheckKeyCommentsPersistence)
{
    EXPECT_TRUE(iniFile.setKeyComment("section1", "key1", "; Key1 comment\n"));
    const std::string testFileName = "key_comments.ini";
    iniFile.save(testFileName);

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section1", "key1"), "value1");

    std::remove(testFileName.c_str());
}
