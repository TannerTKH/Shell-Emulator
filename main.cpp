/*
Tanner Hermann
CS3800
*/


#include <iostream>
#include <string>
#include <vector>

#include "process.h"
#include "file.h"
#include "user.h"
#include "shellfile.h"

using namespace std;

void prompt(Shell newShell);
void run(string command);

int main() {

    //starting screen
    cout << "----------------------------------------------------------------------------" << endl;
    cout << "----------------------------------------------------------------------------" << endl;
    cout << "Shell Emulator by Tanner Hermann" << endl;
    
    cout << "Supported Commands:\n" << endl;
    
    cout << "ls <-l>" << endl;
    cout << "\tList contents of current directory, -l provides attribute details of contents\n" << endl;
    
    cout << "pwd" << endl;
    cout << "\tPrint the full path to the current working directory\n" << endl;
    
    cout << "cd DIRECTORY_NAME" << endl;
    cout << "\tChange directory\n" << endl;

    cout << "mkdir DIRECTORY_NAME" << endl;
    cout << "\tCreates a directory\n" << endl;

    cout << "rmdir DIRECTORY_NAME" << endl;
    cout << "\tRemoves a directory\n" << endl;

    cout << "rm FILE_NAME" << endl;
    cout << "\tRemoves file from current directory\n" << endl;

    cout << "chmod NUMERIC_PERMISSION_STRING FILE_OR_DIRECTORY_NAME" << endl;
    cout << "\tChanges permissions of a file or directory\n" << endl;

    cout << "touch FILE_NAME" << endl;
    cout << "\tUpdate modification date of a file or directory or create a new file\n" << endl;

    cout << "useradd <username>" << endl;
    cout << "\tCreates a new user and adds them to the Users group (and sets Users as their primary group)\n" << endl;

    cout << "useradd -G <group[,group]> <username>" << endl;
    cout << "\tCreates a new user and adds them to the Users group (and sets Users as their primary group). Also adds the user to the additional groups indicated\n" << endl;

    cout << "chuser <username>" << endl;
    cout << "\tChange the active user to the one indicated.\n" << endl;

    cout << "groupadd <group>" << endl;
    cout << "\tCreates a new group and adds the Root user to it.\n" << endl;

    /*
    // TODO: IMPLEMENT THIS FUNCTION

    cout << "usermod -g <group> <username>" << endl;
    cout << "\tSet the primary group for the indicated user.\n" << endl;
    */

    cout << "usermod -a -G <group> <username>" << endl;
    cout << "\tAdd the user to the group.\n" << endl;

    cout << "chown <username> <object>" << endl;
    cout << "\tChange the owner of the indicated object.\n" << endl;

    cout << "chgrp <group> <object>" << endl;
    cout << "\tChange the group of the indicated object to the indicated group.\n" << endl;

    cout << "userdel <username>" << endl;
    cout << "\tRemove the user from the system.\n" << endl;

    cout << "userdel -G <group> <username>" << endl;
    cout << "\tRemove the indicated user from the indicated group.\n" << endl;


    /*
    //!! NEED TO IMPLEMENT
    cout << "groupdel <group>" << endl;
    cout << "\tRemove the group from the system.\n" << endl;
    */

    cout << "groups <username>" << endl;
    cout << "\tList the groups that the indicated user is in.\n" << endl;

    cout << "users" << endl;
    cout << "\tLists all users of the system.\n" << endl;

    cout << "./<file>" << endl;
    cout <<"\t\"Runs\" the indicated file\n" << endl;

    cout << "quit | exit" << endl;
    cout << "\tExit the shell emulator.\n" << endl;

    cout << "----------------------------------------------------------------------------" << endl;
    cout << "----------------------------------------------------------------------------\n\n" << endl;

    Shell shell;
    prompt(shell);

    return 0;
}

void prompt(Shell newShell)
{


    // Shell variables
    string fname;
    vector<Process> procList;
    vector<ProcessHist> procHist;

    int curTime = 0, procIdx;
    int numProcs = 0;
    int timeQuantum = 1; //default time quantum of 1 for round robin scheduling
    bool done = true;
    string tempStr;

    int procHistSize = 0;

    string comm = "";

    bool runShell = true;

    //run shell until 'quit' or 'exit'
    while(runShell) {

        numProcs = procList.size();

        //idea: make this actually change with user
        string currentuser = newShell.current_user->getName();
        cout << currentuser << "$ "; getline(cin,comm);

        newShell.process(comm);

        /*

            All of the scheduling related commands were moved into main.cpp

        */
        if(comm.substr(0,2) == "./")
        {
            string fname = comm;
            fname = fname.substr(2);
            int index = -1;
            bool found = false;

            for(int i = 0; i < newShell.curDir->files.size(); i++) 
            {
                if((newShell.curDir->files[i].getName() == fname) && (newShell.curDir->files[i].getFile()))
                {
                    found = true;
                    index = i;
                    break;
                }
            }           
            if(found == false) { cout << "FILE DOES NOT EXIST" << endl; }
            else
            {
                if(newShell.checkIfUserHasPermissions(newShell.curDir->files[index], 2)) 
                { 
                    cout << fname << " executed" << endl;
                    numProcs = numProcs + 1; //increment the number of processes
                    done = false; //tells the scheduler that there is a process that needs to be scheduled
                    procList.resize(numProcs); 
                    int indexing = procList.size() - 1;
                    procList[indexing].id = fname; //sets the name of the process in the process list
                    procList[indexing].startTime = curTime; //start time of the process is set to when the process is scheduled
                    procList[indexing].totalTimeNeeded = 10; // Right now this is set to 10 until i figure out how to access the
                                                            // scheduled time from when the file was created 
                }
                else {cout << "invalid execute permissions" << endl; }
            }

        
        }
        else if(comm == "schedHist")
        {
            cout << endl;
            for(int i = 0; i < procHist.size(); i++)
            {
                cout << procHist[i].time << " - " << procHist[i].id << endl;
            }
        }


        if(!done) 
        { 
            procIdx = RoundRobin(curTime, procList, timeQuantum);

            if(procIdx >= 0 && procIdx < procList.size())
            {
                ++procList[procIdx].timeScheduled;
                if(procList[procIdx].totalTimeNeeded == procList[procIdx].timeScheduled)
                {
                    procList[procIdx].isDone = true;
                    procList[procIdx].timeFinished = curTime;
                }
                done = true;

                for(int i = 0; i < numProcs; ++i)
                {
                    done = done && procList[i].isDone;
                }

                procHistSize = procHistSize + 1;
                procHist.resize(procHistSize);
                procHist[procHist.size() - 1].time = curTime;
                procHist[procHist.size() -1].id = procList[procIdx].id;
            }
            
        }


        //exit shell
        if (comm == "quit") { runShell = false; }
        else if (comm == "exit") { runShell = false; }


        //increment the time by 1 each time a command is entered in the shell
        //for process scheduling purposes
        curTime = curTime + 1;


    }
   
}