/*
Tanner Hermann
CS3800
HW1 filepermissions.cpp
*/

#include "filepermissions.h"



Permissions::Permissions() {
    for(int i = 0; i < PERM_LENGTH; i++) { permissions[i] = false; }
}

void Permissions::setPerms(int permCode) {

    int access[MODE_SIZE];
    if(checkPerms(permCode, access) == true) {
        string perm_string;
        for(int i = 0; i < MODE_SIZE; i++) {
            perm_string = perm_string + bitset<3>(access[i]).to_string();
        }
        for(int i = 0; i < PERM_LENGTH; i++) {
            permissions[i] = stoi(string(1, perm_string[i]));
        }
    }
    return;
}

string Permissions::getPermString() const {

    string permString = "---------";
    int mode;

    for(int i = 0; i < PERM_LENGTH; i++) {
        if(permissions[i]) {
            mode = i % MODE_SIZE;
            switch(mode) {
                case 0:
                    permString[i] = 'r';
                    break;
                case 1:
                    permString[i] = 'w';
                    break;
                case 2:
                    permString[i] = 'x';
                    break;
                default:
                    break;
            }
        }
        else {
            permString[i] = '-';
        }
    }

    return permString;
}

bool Permissions::checkPerms(int permCode, int access[MODE_SIZE]) {

    bool valid = true;

    if((to_string(permCode)).length() <= 3) {
        for(int i = MODE_SIZE-1; i >= 0; i--) {
            access[i] = permCode % 10;
            permCode /= 10;
        }
    }
    for(int i = 0; i < MODE_SIZE; i++) { if((access[i] < 0) || (access[i] > 7)) { valid = false; break; }}
    return valid;
}


bool Permissions::getPerms(int index) {
    return permissions[index];
}