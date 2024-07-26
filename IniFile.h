//
// Created by samyb on 25/07/2024.
//

#ifndef INIMANAGER_INIFILE_H
#define INIMANAGER_INIFILE_H

#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

using namespace std;

class IniFile
{
    public:
        IniFile() = default;
        explicit IniFile(string name);
        void load(const string& name);
        void save(const string& name) const;
        void save() const;
        string get(const string& section, const string& key) const;
        void set(const string& section, const string& key, const string& value);
        void addSection(const string& section);
        bool hasSection(const string& section) const;
        bool hasKey(const string& section, const string& key) const;
        bool hasKey(const string& key) const;
        void deleteSection(const string& section);
        void deleteKey(const string& section, const string& key);
        void deleteKey(const string& key);
        void print(bool print_comments) const;

    private:
        string fileName;
        map<string, map<string, string>> data;
        map<int, string> comments;
};


#endif //INIMANAGER_INIFILE_H
