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
    ifstream file(name);
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
    ofstream file(name);
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
