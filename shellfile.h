/*
Tanner Hermann
CS3800
*/

#ifndef SHELLFILE_H
#define SHELLFILE_H

#include <sstream>
#include <algorithm>
#include <ctime>

#include "file.h"

#include "user.h"

using namespace std;

class Shell
{
    public:
    //private:
        File root;
        File* curDir;

        //new

        User default_user;
        User* current_user;

        std::vector<User> users;
        std::vector<std::string> s_groups;


        
    //public:
        Shell();

        void process(string& comm);

        void parseCommand(string comm, vector<string>& result);

        void ls(vector<string> comm);

        void cd(vector<string> comm);

        void pwd(vector<string> comm);

        void mkdir(vector<string> comm);

        void rmdir(vector<string> comm);

        void rm(vector<string> comm);

        void chmod(vector<string> comm);

        void touch(vector<string> comm);

        //new stuff

        void parseGroups(string comm, vector<string>& result);
        
        bool checkIfUserHasPermissions(File file_to_check, int access_mode);

        std::string getCurrentUser();


        //User commands
        void useradd(vector<string> comm);
        void usermod(vector<string> comm);
        void userdel(vector<string> comm);
        //changeuser
        void chuser(vector<string> comm);

        void chown(vector<string> comm);

        
        void chgrp(vector<string> comm);
        void groups(vector<string> comm);
        void groupadd(vector<string> comm);




        //shedule stuff
        void run(vector<string> comm);
        void kill(vector<string> comm);
        void ps(vector<string> comm);
        void schedHist(vector<string> comm);



        //wip
        void listusers(vector<string> comm);

        void groupdel(vector<string> comm);
};

#endif
