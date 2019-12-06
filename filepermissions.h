/*
Tanner Hermann
CS3800
HW1 - filepermissions.h
*/

#ifndef FILEPERMISSIONS_H
#define FILEPERMISSIONS_H

#include <string>
#include <vector>
#include <bitset>
#include <iostream>

using namespace std;


class Permissions
{
    private:
        static const int MODE_SIZE = 3;
        static const int PERM_LENGTH = 9;

        bool permissions[PERM_LENGTH];

        enum modes { READ = 0, WRITE = 1, EXECUTE = 2 };

    public:
        Permissions();
        void setPerms(int permCode);
        string getPermString() const;
        bool checkPerms(int permCode, int access[MODE_SIZE]);

        bool getPerms(int index);

};

#endif