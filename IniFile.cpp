//
// Created by samyb on 25/07/2024.
//

#include "IniFile.h"

string IniFile::toLower(const string& str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

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
            section = toLower(line.substr(1, line.size() - 2));
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

        string key = toLower(line.substr(0, pos));
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
    auto it = data.find(toLower(section));
    if (it == data.end())
        return "";

    auto it2 = it->second.find(toLower(key));
    if (it2 == it->second.end())
        return "";

    return it2->second;
}

void IniFile::set(const string& section, const string& key, const string& value)
{
    data[toLower(section)][toLower(key)] = value; // se sezione o chiave non esistono vengono create
}

void IniFile::addSection(const string& section)
{
    data[toLower(section)]; // se la sezione non esiste viene creata, altrimenti non fa nulla
}

bool IniFile::hasSection(const string& section) const
{
    return data.find(toLower(section)) != data.end();
}

bool IniFile::hasKey(const string& section, const string& key) const
{
    auto it = data.find(toLower(section));
    if (it == data.end())
        return false;

    return it->second.find(toLower(key)) != it->second.end();
}

vector<string> IniFile::hasKey(const string& key) const
{
    string lowerKey = toLower(key);

    vector<string> sections;
    for (const auto& section : data)
    {
        if (section.second.find(lowerKey) != section.second.end())
            sections.push_back(section.first);
    }

    return sections;
}

bool IniFile::deleteSection(const string& section)
{
    if (!hasSection(section))
        return false;
    data.erase(toLower(section));
    return true;
}

bool IniFile::deleteKey(const string& section, const string& key)
{
    auto it = data.find(toLower(section));
    if (it == data.end())
        return false;

    if (it->second.find(toLower(key)) == it->second.end())
        return false;

    it->second.erase(toLower(key));
    return true;
}

bool IniFile::setSectionComment(const string& section, const string& comment)
{
    if (!hasSection(section))
        return false;

    sectionComments[toLower(section)] = comment;
    return true;
}

bool IniFile::setKeyComment(const string& section, const string& key, const string& comment)
{
    if (!hasKey(section, key))
        return false;

    keyComments[toLower(section)][toLower(key)] = comment;
    return true;
}

string IniFile::print(bool print_comments) const
{
    string output;

    for (const auto& section : data)
    {
        if (print_comments)
        {
            auto sectionComment = sectionComments.find(section.first);
            if (sectionComment != sectionComments.end())
                output += sectionComment->second;
        }

        output += '[' + section.first + ']' + '\n';

        for (const auto& key : section.second)
        {
            if (print_comments)
            {
                auto keyCommentSection = keyComments.find(section.first);
                if (keyCommentSection != keyComments.end())
                {
                    auto keyComment = keyCommentSection->second.find(key.first);
                    if (keyComment != keyCommentSection->second.end())
                        output += keyComment->second;
                }
            }
            output += key.first + '=' + key.second + '\n';
        }
    }

    return output;
}

string IniFile::getSectionComment(const string &section) const
{
    auto it = sectionComments.find(toLower(section));
    if (it == sectionComments.end())
        return "";

    return it->second;
}

string IniFile::getKeyComment(const string &section, const string &key) const
{
    auto it = keyComments.find(toLower(section));
    if (it == keyComments.end())
        return "";

    auto it2 = it->second.find(toLower(key));
    if (it2 == it->second.end())
        return "";

    return it2->second;
}

