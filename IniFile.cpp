//
// Created by samyb on 25/07/2024.
//

#include "IniFile.h"

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
    ifstream file(fileName);    // apre il file in lettura

    if (!file.is_open())
        throw runtime_error("Unable to open file: " + fileName);

    string line;
    string section;
    string comment;

    for (int n = 0; getline(file, line); n++)
    {
        if (line.empty())
            continue;

        if (line[0] == ';')
        {
            comment += line + "\n";
            continue;
        }

        if (line[0] == '[')
        {
            section = line.substr(1, line.size() - 2);
            if (!comment.empty())
            {
                sectionComments[section] = comment;
                comment.clear();
            }
            continue;
        }

        size_t pos = line.find('=');
        if (pos == string::npos)
            continue;

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        data[section][key] = value;

        if (!comment.empty())
        {
            keyComments[section][key] = comment;
            comment.clear();
        }
    }

    if (file.bad()) // controlla se ci sono stati errori durante la lettura
        throw runtime_error("Error reading the file: " + fileName);
}

void IniFile::save(const string& name) const
{
    ofstream file(name);    // apre il file in scrittura (sovrascrive il file se esiste)

    if (!file.is_open())
        throw runtime_error("Unable to open file for writing: " + name);

    for (const auto& section : data)
    {
        auto sectionComment = sectionComments.find(section.first);
        if (sectionComment != sectionComments.end())
        {
            file << sectionComment->second;
        }

        file << '[' << section.first << ']' << std::endl;

        for (const auto& key : section.second)
        {
            auto keyCommentSection = keyComments.find(section.first);
            if (keyCommentSection != keyComments.end())
            {
                auto keyComment = keyCommentSection->second.find(key.first);
                if (keyComment != keyCommentSection->second.end())
                {
                    file << keyComment->second;
                }
            }
            file << key.first << '=' << key.second << std::endl;
        }
    }

    if (file.bad()) // controlla se ci sono stati errori durante la scrittura
        throw runtime_error("Error writing to the file: " + name);
}

void IniFile::save() const
{
    save(fileName);
}

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

void IniFile::set(const string& section, const string& key, const string& value)
{
    data[section][key] = value; // se sezione o chiave non esistono vengono create
}

void IniFile::addSection(const string& section)
{
    data[section]; // se la sezione non esiste viene creata, altrimenti non fa nulla
}

bool IniFile::hasSection(const string& section) const
{
    return data.find(section) != data.end();
}

bool IniFile::hasKey(const string& section, const string& key) const
{
    auto it = data.find(section);
    if (it == data.end())
        return false;

    return it->second.find(key) != it->second.end();
}

bool IniFile::hasKey(const string& key) const
{
    // std::any_of ritorna true se almeno un elemento soddisfa il predicato
    return any_of(data.begin(), data.end(), [&](const auto& section) {
        return section.second.find(key) != section.second.end();
    });
}

void IniFile::deleteSection(const string& section)
{
    data.erase(section);    // se la sezione non esiste non fa nulla
}

void IniFile::deleteKey(const string& section, const string& key)
{
    auto it = data.find(section);
    if (it == data.end())
        return;

    it->second.erase(key);
}

void IniFile::deleteKey(const string& key)
{
    for (auto& section : data)
        section.second.erase(key);
}

void IniFile::setSectionComment(const string& section, const string& comment)
{
    sectionComments[section] = comment;
}

void IniFile::setKeyComment(const string& section, const string& key, const string& comment)
{
    keyComments[section][key] = comment;
}

void IniFile::print(bool print_comments) const
{
    for (const auto& section : data)
    {
        if (print_comments)
        {
            auto sectionComment = sectionComments.find(section.first);
            if (sectionComment != sectionComments.end())
            {
                cout << sectionComment->second;
            }
        }

        cout << '[' << section.first << ']' << std::endl;

        for (const auto& key : section.second)
        {
            if (print_comments)
            {
                auto keyCommentSection = keyComments.find(section.first);
                if (keyCommentSection != keyComments.end())
                {
                    auto keyComment = keyCommentSection->second.find(key.first);
                    if (keyComment != keyCommentSection->second.end())
                    {
                        cout << keyComment->second;
                    }
                }
            }
            cout << key.first << '=' << key.second << std::endl;
        }
    }
}

