//
// Created by samyb on 25/07/2024.
//

#include <filesystem>
#include <fstream>
#include "IniFile.h"

using namespace std;
namespace fs = filesystem;

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

void testLoadSaveComments()
{
    cout << "Test: Load and Save" << endl;

    IniFile ini("../test/test.ini");

    ini.set("General", "Name", "TestApp");
    ini.set("General", "Version", "1.0");
    ini.set("Network", "Host", "localhost");
    ini.set("Network", "Port", "8080");

    try
    {
        ini.save("../test/new_test.ini");
        ini.save();
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../test/new_test.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Cloned file without comments:" << endl;
    ini.print(false);

    IniFile loadedIni;

    try
    {
        loadedIni.load("../test/test.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Modified file with comments:" << endl;
    loadedIni.print(true);
    cout << endl;
}

void testLoadWithErrors()
{
    cout << "Test: Load with Errors" << endl;

    // Rimuove il file se esiste per simulare un errore di apertura
    if (fs::exists("non_existent_file.ini"))
        fs::remove("non_existent_file.ini");

    IniFile ini("../test/non_existent_file.ini");

    try
    {
        ini.load("../test/non_existent_file.ini");
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
    fs::create_directory("../test/read_only_dir");
    fs::path readonlyFilePath = "../test/read_only_dir/test_save.ini";

    // Crea un file temporaneo
    ofstream readonlyFile(readonlyFilePath);
    readonlyFile.close();

    // Rende il file di sola lettura
    fs::permissions(readonlyFilePath, fs::perms::none);

    IniFile ini("../test/read_only_dir/test_save.ini");
    ini.set("Test", "Key", "Value");

    try
    {
        ini.save("../test/read_only_dir/test_save.ini");
    }
    catch (const runtime_error& e)
    {
        cerr << "Caught error during save: " << e.what() << endl;
    }

    // Ripristina i permessi e rimuove il file e la directory temporanei
    fs::permissions(readonlyFilePath, fs::perms::all);
    fs::remove(readonlyFilePath);
    fs::remove("../test/read_only_dir");

    cout << endl;
}

void testAddSection()
{
    cout << "Test: Add Section" << endl;

    IniFile ini("../test/test_add_section.ini");
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
        ini.load("../test/test_add_section.ini");
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

    IniFile ini("../test/test_has_section.ini");
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
        ini.load("../test/test_has_section.ini");
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

    IniFile ini("../test/test_has_key.ini");
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
        ini.load("../test/test_has_key.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Has 'Section1.Key1': " << ini.hasKey("Section1", "Key1") << endl;
    cout << "Has 'Section1.Key3': " << ini.hasKey("Section1", "Key3") << endl;
    cout << "Has 'Key2' (in any section): " << ini.hasKey("Key2") << endl;
    cout << "Has 'Key3' (in any section): " << ini.hasKey("Key3") << endl;
    cout << endl;
}

void testGetAndSet()
{
    cout << "Test: Get and Set" << endl;

    IniFile ini("../test/test_get_set.ini");
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
        ini.load("../test/test_get_set.ini");
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

    IniFile ini("../test/test_delete_section.ini");
    ini.addSection("SectionToDelete");
    ini.set("SectionToDelete", "Key1", "Value1");
    ini.set("SectionToDelete", "Key2", "Value2");

    ini.save("../test/test_delete_section.ini");
    ini.deleteSection("SectionToDelete");

    try
    {
        ini.save("../test/test_delete_section_deleted.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    ini.load("../test/test_delete_section_deleted.ini");

    cout << "Has 'SectionToDelete': " << ini.hasSection("SectionToDelete") << endl;
    cout << endl;
}

void testDeleteKey()
{
    cout << "Test: Delete Key" << endl;

    IniFile ini("../test/test_delete_key.ini");
    ini.set("Section1", "KeyToDelete", "ValueToDelete");
    ini.set("Section2", "KeyToDelete", "ValueToDelete2");
    ini.set("Section1", "KeyToKeep", "ValueToKeep");

    ini.save("../test/test_delete_key.ini");

    ini.deleteKey("Section1", "KeyToDelete");
    try
    {
        ini.save("../test/test_delete_key_specific_deleted.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }
    ini.load("../test/test_delete_key_specific_deleted.ini");
    cout << "After deleting 'KeyToDelete' from 'Section1':" << endl;
    cout << "Has 'Section1.KeyToDelete': " << ini.hasKey("Section1", "KeyToDelete") << endl;
    cout << "Has 'Section2.KeyToDelete': " << ini.hasKey("Section2", "KeyToDelete") << endl;
    cout << "Has 'Section1.KeyToKeep': " << ini.hasKey("Section1", "KeyToKeep") << endl;

    ini.set("Section1", "KeyToDelete", "ValueToDelete");
    ini.save("../test/test_delete_key.ini");
    ini.load("../test/test_delete_key.ini");

    ini.deleteKey("KeyToDelete");
    try
    {
        ini.save("../test/test_delete_key_all_deleted.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }
    ini.load("../test/test_delete_key_all_deleted.ini");
    cout << "After deleting 'KeyToDelete' from all sections:" << endl;
    cout << "Has 'Section1.KeyToDelete': " << ini.hasKey("Section1", "KeyToDelete") << endl;
    cout << "Has 'Section2.KeyToDelete': " << ini.hasKey("Section2", "KeyToDelete") << endl;
    cout << "Has 'Section1.KeyToKeep': " << ini.hasKey("Section1", "KeyToKeep") << endl;
    cout << endl;
}

void testComments()
{
    cout << "Test: Comments Handling" << endl;

    IniFile ini("../test/test_comments.ini");

    cout << "Original file with comments:" << endl;
    ini.print(true);

    ini.set("General", "Weight", "10");
    ini.set("NewSection", "NewKey", "NewValue");
    ini.setSectionComment("NewSection", "; Comment for the new section\n");
    ini.setKeyComment("NewSection", "NewKey", "; Comment for the new key\n");

    try
    {
        ini.save("../test/test_comments_modified.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error saving INI file: " << e.what() << endl;
    }

    try
    {
        ini.load("../test/test_comments_modified.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Modified file with comments:" << endl;
    ini.print(true);
}
