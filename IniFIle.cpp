//
// Created by samyb on 25/07/2024.
//

#include "IniFIle.h"

IniFIle::IniFIle(string& name) : fileName(name)
{
    load(fileName);
}

IniFIle::~IniFIle()
{
    save(fileName);
}

bool IniFIle::load(const string& name)
{
    ifstream file(name);    // apre il file in lettura
    if (!file.is_open())
        return false;

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

    file.close();
    return true;
}

bool IniFIle::save(const string& name) const
{
    ofstream file(name);    // sovrascrive il file
    if (!file.is_open())
        return false;

    for (const auto& section : data)
    {
        file << '[' << section.first << ']' << endl;
        for (const auto& key : section.second)
            file << key.first << '=' << key.second << endl;
    }

    file.close();
    return true;
}

bool IniFIle::save() const
{
    return save(fileName);
}

string IniFIle::get(const string& section, const string& key) const
{
    auto it = data.find(section);
    if (it == data.end())
        return "";

    auto it2 = it->second.find(key);
    if (it2 == it->second.end())
        return "";

    return it2->second;
}

void IniFIle::set(const string& section, const string& key, const string& value)
{
    data[section][key] = value; // se sezione o chiave non esistono vengono create
}

void IniFIle::addSection(const string& section)
{
    data[section];
}

bool IniFIle::hasSection(const string& section) const
{
    return data.find(section) != data.end();
}

bool IniFIle::hasKey(const string& section, const string& key) const
{
    auto it = data.find(section);
    if (it == data.end())
        return false;

    return it->second.find(key) != it->second.end();
}

bool IniFIle::hasKey(const string& key) const
{
    // std::any_of ritorna true se almeno un elemento soddisfa il predicato
    return any_of(data.begin(), data.end(), [&](const auto& section) {
        return hasKey(section.first, key);
    });
}
