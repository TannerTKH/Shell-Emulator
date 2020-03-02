/*
Tanner Hermann
CS3800
file.h
*/

#ifndef FILE_H
#define FILE_H

#include <vector>
#include <ctime>
#include <string>
#include <algorithm>

#include "filepermissions.h"

using namespace std;

struct Properties
{
    Permissions permissions;
    int links;
    string owner;
    string group;
    int fileSize;
    time_t timestamp;
    string name;
    bool isFile;
    int runtime;

    Properties() {
        //defualt file properties

        //default linux permissions 
        permissions.setPerms(755); //changed from 644 to 755
        links = 1;
        owner = "tanner";
        group = "users";
        fileSize = 512;
        time(&timestamp);
        isFile = false;
        runtime = 10;
    }
};

class File
{
    private:
        Properties property;

        File* parent;

    public:
        vector<File> files;

        File() {}

        File(string name, bool type) { property.name = name; property.isFile = type; }

        File(string name, bool type, string owner, string group)
        {
            property.name = name;
            property.isFile = type;
            property.owner = owner;
            property.group = group;
        }

        void setFile(const bool& type);
        bool getFile() const;

        void setName(const string& name);
        string getName() const;

        Properties* getProp();

        void setParent(File* curDir);
        File* getParent();

        string fileInfo();

        void setRunTime(const int& runtime);
        int getRunTime() const;
};

#endif
