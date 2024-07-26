//
// Created by samyb on 25/07/2024.
//

#include "IniFile.h"
// TODO roba per non eliminare i commenti
IniFile::IniFile(string name) : fileName(std::move(name))
{
    try
    {
        load(fileName);
    }
    catch (const exception& e)
    {
        cerr << "Error loading INI file: " << e.what() << endl;
    }
}

void IniFile::load(const string& name)
{
    fileName = name;
    ifstream file(fileName); // apre il file in lettura

    if (!file.is_open())
        throw runtime_error("Unable to open file: " + fileName);

    string line;
    string section;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '[')
        {
            section = line.substr(1, line.size() - 2);
            continue;
        }

        size_t pos = line.find('=');
        if (pos == string::npos)
            continue;

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        data[section][key] = value;
    }

    if (file.bad()) // controlla se ci sono stati errori durante la lettura
        throw runtime_error("Error reading the file: " + fileName);
}

// Metodo per salvare il file INI
void IniFile::save(const string& name) const
{
    ofstream file(name); // sovrascrive il file

    if (!file.is_open())
        throw runtime_error("Unable to open file for writing: " + name);

    for (const auto& section : data)
    {
        file << '[' << section.first << ']' << std::endl;
        for (const auto& key : section.second)
            file << key.first << '=' << key.second << std::endl;
    }

    if (file.bad()) // controlla se ci sono stati errori durante la scrittura
        throw runtime_error("Error writing to the file: " + name);
}

// Metodo per salvare il file INI
void IniFile::save() const
{
    save(fileName);
}

// Metodo per ottenere un valore
string IniFile::get(const string& section, const string& key) const
{
    auto it = data.find(section);
    if (it == data.end())
        return "";

    auto it2 = it->second.find(key);
    if (it2 == it->second.end())
        return "";

    return it2->second;
}

// Metodo per impostare un valore
void IniFile::set(const string& section, const string& key, const string& value)
{
    data[section][key] = value; // se sezione o chiave non esistono vengono create
}

// Metodo per aggiungere una sezione
void IniFile::addSection(const string& section)
{
    data[section]; // se la sezione non esiste viene creata, altrimenti non fa nulla
}

// Metodo per verificare se una sezione esiste
bool IniFile::hasSection(const string& section) const
{
    return data.find(section) != data.end();
}

// Metodo per verificare se una chiave esiste in una sezione specifica
bool IniFile::hasKey(const string& section, const string& key) const
{
    auto it = data.find(section);
    if (it == data.end())
        return false;

    return it->second.find(key) != it->second.end();
}

// Metodo per verificare se una chiave esiste in qualsiasi sezione
bool IniFile::hasKey(const string& key) const
{
    // std::any_of ritorna true se almeno un elemento soddisfa il predicato
    return any_of(data.begin(), data.end(), [&](const auto& section) {
        return hasKey(section.first, key);
    });
}
