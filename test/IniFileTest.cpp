#include "gtest/gtest.h"
#include "../IniFile.h"

TEST(IniFileTest, CreateEmptyIniFile)
{
    IniFile iniFile;
    EXPECT_TRUE(iniFile.get("nonexistent_section", "nonexistent_key").empty());
}

TEST(IniFileTest, SetAndGetValues)
{
    IniFile iniFile;
    iniFile.set("section", "key", "value");
    EXPECT_EQ(iniFile.get("section", "key"), "value");
}

TEST(IniFileTest, AddAndDeleteSections)
{
    IniFile iniFile;
    iniFile.addSection("new_section");
    EXPECT_TRUE(iniFile.hasSection("new_section"));
    EXPECT_TRUE(iniFile.deleteSection("new_section"));
    EXPECT_FALSE(iniFile.hasSection("new_section"));
}

TEST(IniFileTest, AddAndDeleteKeys)
{
    IniFile iniFile;
    iniFile.set("section", "key", "value");
    EXPECT_TRUE(iniFile.hasKey("section", "key"));
    EXPECT_TRUE(iniFile.deleteKey("section", "key"));
    EXPECT_FALSE(iniFile.hasKey("section", "key"));
}

TEST(IniFileTest, SaveAndLoadFile)
{
    const string testFileName = "test.ini";
    {
        IniFile iniFile;
        iniFile.set("section", "key", "value");
        iniFile.save(testFileName);
    }

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section", "key"), "value");

    remove(testFileName.c_str());
}

TEST(IniFileTest, CommentsHandling)
{
    IniFile iniFile;

    iniFile.set("section", "key", "value");
    EXPECT_TRUE(iniFile.setSectionComment("section", "; This is a section comment\n"));
    EXPECT_TRUE(iniFile.setKeyComment("section", "key", "; This is a key comment\n"));

    const string testFileName = "test_comments.ini";
    iniFile.save(testFileName);

    IniFile loadedIniFile(testFileName);
    EXPECT_EQ(loadedIniFile.get("section", "key"), "value");
    EXPECT_TRUE(loadedIniFile.hasSection("section"));
    EXPECT_TRUE(loadedIniFile.hasKey("section", "key"));
    EXPECT_EQ(loadedIniFile.getSectionComment("section"), "; This is a section comment\n");
    EXPECT_EQ(loadedIniFile.getKeyComment("section", "key"), "; This is a key comment\n");

    remove(testFileName.c_str());
}

TEST(IniFileTest, HandleEmptyLinesAndComments)
{
    IniFile iniFile;
    iniFile.set("section", "key", "value");
    const string testFileName = "test_empty_lines.ini";

    ofstream file(testFileName);
    file << "\n; This is a comment\n[section]\nkey=value\n";
    file.close();

    iniFile.load(testFileName);
    EXPECT_EQ(iniFile.get("section", "key"), "value");

    remove(testFileName.c_str());
}

TEST(IniFileTest, CaseInsensitiveKeys)
{
    IniFile iniFile;
    iniFile.set("section", "Key", "value");
    EXPECT_EQ(iniFile.get("section", "key"), "value");
    EXPECT_EQ(iniFile.get("section", "KEY"), "value");
}

TEST(IniFileTest, InvalidFileHandling)
{
    IniFile iniFile;
    EXPECT_THROW(iniFile.load("non_existent_file.ini"), runtime_error);
}

TEST(IniFileTest, MalformedLines)
{
    IniFile iniFile;
    const string testFileName = "test_malformed.ini";

    ofstream file(testFileName);
    file << "[section]\nmalformed_line_without_equals_sign\nkey=value\n";
    file.close();

    iniFile.load(testFileName);
    EXPECT_EQ(iniFile.get("section", "key"), "value");
    EXPECT_TRUE(iniFile.get("section", "malformed_line_without_equals_sign").empty());

    remove(testFileName.c_str());
}

TEST(IniFileTest, NonExistingElements)
{
    IniFile iniFile;
    iniFile.set("section", "Key", "value");
    EXPECT_FALSE(iniFile.deleteKey("NonExistingSection", "NonExistingKey"));
    EXPECT_FALSE(iniFile.deleteSection("NonExistingSection"));
    EXPECT_FALSE(iniFile.setKeyComment("NonExistingSection", "NonExistingKey", "KeyComment"));
    EXPECT_FALSE(iniFile.setSectionComment("NonExistingSection", "SectionComment"));
    EXPECT_TRUE(iniFile.getKeyComment("NonExistingSection", "NonExistingKey").empty());
    EXPECT_TRUE(iniFile.getSectionComment("NonExistingSection").empty());
}
