/*
Tanner Hermann
CS3800
file.cpp
*/

#include "file.h"

void File::setName(const string& name) { property.name = name; return; }

string File::getName() const { return property.name; }

void File::setFile(const bool& type) { property.isFile = type; return; }

bool File::getFile() const { return property.isFile; }

Properties* File::getProp() { return &property; }

void File::setParent(File* curDir) { parent = curDir; return; }

File* File::getParent() { return parent; }


//
void File::setRunTime(const int& runtime) { property.runtime = runtime; return; }
int File::getRunTime() const {return property.runtime;}

//setup file info
string File::fileInfo() {

    string info;

    // get time and format it
	struct tm * timeInfo;
	
	timeInfo = localtime(&property.timestamp);
	string curTime = asctime(timeInfo);

	curTime.erase(remove(curTime.begin(), curTime.end(), '\n'), curTime.end());

    //format file info for ls -l
    info = info + property.permissions.getPermString() + " " + to_string(property.links) +
            " " + property.owner + " " + property.group + " " + to_string(property.fileSize) + 
            " " + curTime + " " + property.name;

    if(property.isFile) { info = "-" + info; }
    else { info = "d" + info + "\\"; }

    return info;
}
