//
// Created by samyb on 25/07/2024.
//

#include <filesystem>
#include <fstream>
#include "IniFile.h"

using namespace std;
namespace fs = filesystem;

void testCreateIniFile();
void testLoadSaveComments();
void testLoadWithErrors();
void testSaveWithErrors();
void testAddSection();
void testHasSection();
void testHasKey();
void testGetAndSet();
void testDeleteSection();
void testDeleteKey();
void testComments();

int main()
{
    testCreateIniFile();
    testLoadSaveComments();

    testLoadWithErrors();
    testSaveWithErrors();

    testAddSection();

    testHasSection();

    testHasKey();

    testGetAndSet();

    testDeleteSection();
    testDeleteKey();

    testComments();

    return 0;
}

void testCreateIniFile()
{
    cout << "Test: Create INI File" << endl;

    IniFile ini;
    ini.set("General", "Name", "TestApp");
    ini.set("General", "Version", "1.0");
    ini.set("Network", "Host", "localhost");
    ini.set("Network", "Port", "8080");

    try
    {
        ini.save("../iniFiles/created.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    cout << "Created file:" << endl << ini.print(true) << endl;

    fs::remove("../iniFiles/created.ini");
}

void testLoadSaveComments()
{
    cout << "Test: Load and Save" << endl;

    IniFile ini("../iniFiles/test.ini");

    ini.set("General", "Name", "TestApp");
    ini.set("General", "Version", "1.0");
    ini.set("Network", "Host", "localhost");
    ini.set("Network", "Port", "8080");

    try
    {
        ini.save("../iniFiles/new_test.ini");
        ini.save();
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../iniFiles/new_test.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Cloned file without comments:" << endl << ini.print(false);

    IniFile loadedIni;

    try
    {
        loadedIni.load("../iniFiles/test.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Modified file with comments:" << endl << loadedIni.print(true) << endl;

    fs::remove("../iniFiles/new_test.ini");
}

void testLoadWithErrors()
{
    cout << "Test: Load with Errors" << endl;

    // Rimuove il file se esiste per simulare un errore di apertura
    if (fs::exists("non_existent_file.ini"))
        fs::remove("non_existent_file.ini");

    IniFile ini("../iniFiles/non_existent_file.ini");

    try
    {
        ini.load("../iniFiles/non_existent_file.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << endl;
}

void testSaveWithErrors()
{
    cout << "Test: Save with Errors" << endl;

    // Crea un file temporaneo per simulare un errore di scrittura
    fs::create_directory("../iniFiles/read_only_dir");
    fs::path readonlyFilePath = "../iniFiles/read_only_dir/test_save.ini";

    // Crea un file temporaneo
    ofstream readonlyFile(readonlyFilePath);
    readonlyFile.close();

    // Rende il file di sola lettura
    fs::permissions(readonlyFilePath, fs::perms::none);

    IniFile ini("../iniFiles/read_only_dir/test_save.ini");
    ini.set("Test", "Key", "Value");

    try
    {
        ini.save("../iniFiles/read_only_dir/test_save.ini");
    }
    catch (const runtime_error& e)
    {
        cerr << "Caught error during save: " << e.what() << endl;
    }

    // Ripristina i permessi e rimuove il file e la directory temporanei
    fs::permissions(readonlyFilePath, fs::perms::all);
    fs::remove(readonlyFilePath);
    fs::remove("../iniFiles/read_only_dir");

    cout << endl;
}

void testAddSection()
{
    cout << "Test: Add Section" << endl;

    IniFile ini("../iniFiles/test_add_section.ini");
    ini.addSection("NewSection");
    ini.set("NewSection", "Key1", "Value1");

    try
    {
        ini.save();
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../iniFiles/test_add_section.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "NewSection.Key1: " << ini.get("NewSection", "Key1") << endl;
    cout << endl;
}

void testHasSection()
{
    cout << "Test: Has Section" << endl;

    IniFile ini("../iniFiles/test_has_section.ini");
    ini.addSection("ExistingSection");

    try
    {
        ini.save();
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../iniFiles/test_has_section.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Has 'ExistingSection': " << ini.hasSection("ExistingSection") << endl;
    cout << "Has 'NonExistingSection': " << ini.hasSection("NonExistingSection") << endl;
    cout << endl;
}

void testHasKey()
{
    cout << "Test: Has Key" << endl;

    IniFile ini("../iniFiles/test_has_key.ini");
    ini.set("Section1", "Key1", "Value1");
    ini.set("Section1", "Key2", "Value2");

    try
    {
        ini.save();
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../iniFiles/test_has_key.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Has 'Section1.Key1': " << ini.hasKey("Section1", "Key1") << endl;
    cout << "Has 'Section1.Key3': " << ini.hasKey("Section1", "Key3") << endl;
    cout << "Has 'Key2' (in any section):" << endl;
    for (const auto& section : ini.hasKey("Key2"))
        cout << "  " << section << ".Key2" << endl;
    cout << "Has 'Key3' (in any section):" << endl;
    for (const auto& section : ini.hasKey("Key3"))
        cout << "  " << section << ".Key3" << endl;
    cout << endl;
}

void testGetAndSet()
{
    cout << "Test: Get and Set" << endl;

    IniFile ini("../iniFiles/test_get_set.ini");
    ini.set("Settings", "Language", "English");
    ini.set("Settings", "Theme", "Dark");

    try
    {
        ini.save();
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../iniFiles/test_get_set.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Settings.Language: " << ini.get("Settings", "Language") << endl;
    cout << "Settings.Theme: " << ini.get("Settings", "Theme") << endl;
    cout << endl;
}

void testDeleteSection()
{
    cout << "Test: Delete Section" << endl;

    IniFile ini("../iniFiles/test_delete_section.ini");
    ini.addSection("SectionToDelete");
    ini.set("SectionToDelete", "Key1", "Value1");
    ini.set("SectionToDelete", "Key2", "Value2");

    ini.save("../iniFiles/test_delete_section.ini");
    if (!ini.deleteSection("SectionToDelete"))
        cout << "SectionToDelete does not exist" << endl;
    if (!ini.deleteSection("NonExistingSection"))
        cout << "NonExistingSection does not exist" << endl;

    try
    {
        ini.save("../iniFiles/test_delete_section_deleted.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    ini.load("../iniFiles/test_delete_section_deleted.ini");

    cout << "Has 'SectionToDelete': " << ini.hasSection("SectionToDelete") << endl;
    cout << endl;

    fs::remove("../iniFiles/test_delete_section_deleted.ini");
}

void testDeleteKey()
{
    cout << "Test: Delete Key" << endl;

    IniFile ini("../iniFiles/test_delete_key.ini");
    ini.set("Section1", "KeyToDelete", "ValueToDelete");
    ini.set("Section2", "KeyToDelete", "ValueToDelete2");
    ini.set("Section1", "KeyToKeep", "ValueToKeep");

    ini.save("../iniFiles/test_delete_key.ini");

    if (!ini.deleteKey("Section1", "KeyToDelete"))
        cout << "KeyToDelete does not exist in Section1" << endl;
    if (!ini.deleteKey("Section2", "KeyToDelete"))
        cout << "KeyToDelete does not exist in Section2" << endl;

    try
    {
        ini.save("../iniFiles/test_delete_key_specific_deleted.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }
    ini.load("../iniFiles/test_delete_key_specific_deleted.ini");
    cout << "After deleting 'KeyToDelete' from 'Section1':" << endl;
    cout << "Has 'Section1.KeyToDelete': " << ini.hasKey("Section1", "KeyToDelete") << endl;
    cout << "Has 'Section2.KeyToDelete': " << ini.hasKey("Section2", "KeyToDelete") << endl;
    cout << "Has 'Section1.KeyToKeep': " << ini.hasKey("Section1", "KeyToKeep") << endl;
    cout << endl;

    fs::remove("../iniFiles/test_delete_key_specific_deleted.ini");
}

void testComments()
{
    cout << "Test: Comments Handling" << endl;

    IniFile ini("../iniFiles/test_comments.ini");

    cout << "Original file with comments:" << endl << ini.print(true) << endl;

    ini.set("General", "Weight", "10");
    ini.set("NewSection", "NewKey", "NewValue");
    if (!ini.setSectionComment("NewSection", "; Comment for the new section\n"))
        cout << "NewSection does not exist" << endl;
    cout << "NewSection comment: " << ini.getSectionComment("NewSection") << endl;
    if (!ini.setSectionComment("NonExistingSection", "; Comment for NonExistingSection\n"))
        cout << "NonExistingSection does not exist" << endl;
    cout << "NonExistingSection comment: " << ini.getSectionComment("NonExistingSection") << endl;
    if (!ini.setKeyComment("NewSection", "NewKey", "; Comment for the new key\n"))
        cout << "NewKey does not exist in NewSection" << endl;
    cout << "NewKey comment: " << ini.getKeyComment("NewSection", "NewKey") << endl;
    cout << "NonExistingKey comment: " << ini.getKeyComment("NewSection", "NonExistingKey") << endl;

    try
    {
        ini.save("../iniFiles/test_comments_modified.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../iniFiles/test_comments_modified.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Modified file with comments:" << endl << ini.print(true);

    fs::remove("../iniFiles/test_comments_modified.ini");
}
