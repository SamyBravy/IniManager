//
// Created by samyb on 25/07/2024.
//

#include <filesystem>
#include <fstream>
#include "IniFile.h"

using namespace std;
namespace fs = filesystem;

void testLoadAndSave();
void testLoadWithErrors();
void testSaveWithErrors();
void testAddSection();
void testHasSection();
void testHasKey();
void testGetAndSet();

int main()
{
    testLoadAndSave();

    testLoadWithErrors();
    testSaveWithErrors();

    testAddSection();

    testHasSection();

    testHasKey();

    testGetAndSet();

    return 0;
}

void testLoadAndSave()
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

    cout << "Cloned file:" << endl;
    cout << "General.Name: " << ini.get("General", "Name") << endl;
    cout << "General.Version: " << ini.get("General", "Version") << endl;
    cout << "Network.Host: " << ini.get("Network", "Host") << endl;
    cout << "Network.Port: " << ini.get("Network", "Port") << endl;

    IniFile loadedIni;

    try
    {
        loadedIni.load("../test/test.ini");
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }

    cout << "Modified file:" << endl;
    cout << "General.Name: " << loadedIni.get("General", "Name") << endl;
    cout << "General.Version: " << loadedIni.get("General", "Version") << endl;
    cout << "Network.Host: " << loadedIni.get("Network", "Host") << endl;
    cout << "Network.Port: " << loadedIni.get("Network", "Port") << endl;
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
