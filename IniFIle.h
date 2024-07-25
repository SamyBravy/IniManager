//
// Created by samyb on 25/07/2024.
//

#ifndef INIMANAGER_INIFILE_H
#define INIMANAGER_INIFILE_H

#include <string>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

class IniFIle
{
    public:
        IniFIle() = default;
        explicit IniFIle(string& name);
        ~IniFIle();
        bool load(const string& name);
        bool save(const string& name) const;
        bool save() const;
        string get(const string& section, const string& key) const;
        void set(const string& section, const string& key, const string& value);
        void addSection(const string& section);
        bool hasSection(const string& section) const;
        bool hasKey(const string& section, const string& key) const;
        bool hasKey(const string& key) const;

    private:
        string fileName;
        map<string, map<string, string>> data;
};


#endif //INIMANAGER_INIFILE_H
