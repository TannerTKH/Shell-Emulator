/*
Tanner Hermann
CS3800
*/

#include "shellfile.h"

#include <iostream>
#include <iomanip>
#include <vector>

string invalid_ls = "ERROR invalid ls command.";
string invalid_cd = "ERROR invalid cd command.";
string invalid_pwd = "ERROR invalid pwd command.";
string invalid_mkdir = "ERROR invalid mkdir command.";
string invalid_rmdir = "ERROR invalid rmdir command.";
string invalid_rm = "ERROR invalid rm command.";
string invalid_touch = "ERROR invalid touch command.";
string invalid_chmod = "ERROR invalid chmod command.";

string invalid_useradd = "ERROR invalid useradd command.";
string invalid_usermod = "ERROR invalid usermod command.";
string invalid_userdel = "ERROR invalid userdel command.";
string invalid_chuser = "ERROR invalid chuser command.";
string invalid_chown = "ERROR invalid chown command.";
string invalid_chgrp = "ERROR invalid chgrp command.";
string invalid_groups = "ERROR invalid groups command.";
string invalid_groupadd = "ERROR invalid groupadd command.";

string invalid_groupdel = "ERROR invalid groupdel command.";

string invalid_command = "ERROR invalid command.";

//sets up initial directory and structure
Shell::Shell() {

    //setup root user properties
    root.setName("root");
    root.setFile(false);
    root.setParent(NULL);
    curDir = &root;

    //set up starting user properties
    default_user.setName("tanner");
    current_user = &default_user;
    users.push_back(default_user);
    s_groups.push_back("users");

}

void Shell::process(string& comm) {

    vector<string> cmd;

    parseCommand(comm, cmd);

    if(cmd.size() == 0) { return; }
    else {
        if(cmd[0] == "ls") { ls(cmd); }
        else if(cmd[0] == "cd") { cd(cmd); }
        else if(cmd[0] == "pwd") { pwd(cmd); }
        else if(cmd[0] == "mkdir") { mkdir(cmd); }
        else if(cmd[0] == "rmdir") { rmdir(cmd); }
        else if(cmd[0] == "rm") { rm(cmd); }
        else if(cmd[0] == "touch") { touch(cmd); }
        else if(cmd[0] == "chmod") { chmod(cmd); }
        else if(cmd[0] == "useradd") {useradd(cmd); }
        else if(cmd[0] == "usermod" ) { usermod(cmd); }
        else if(cmd[0] == "userdel") {userdel(cmd); }
        else if(cmd[0] == "chuser") {chuser(cmd); }
        else if(cmd[0] == "chown" ) { chown(cmd); }
        else if(cmd[0] == "chgrp" ) { chgrp(cmd); }

        

        else if(cmd[0] == "groups") { groups(cmd); }
        else if(cmd[0] == "groupadd") {groupadd(cmd);}

        else if(cmd[0] == "users") { listusers(cmd); }


        //scheduling commands moved to main.cpp
        else if(cmd[0] == "kill") { return; }
        else if(cmd[0] == "ps") { return; }
        else if(cmd[0] == "schedHist") { return; }
        else if(cmd[0].substr(0,2) == "./") { return; }

        else if(cmd[0] == "quit" ) { return; }
        else if(cmd[0] == "exit" ) { return; }
        
        else { cout << invalid_command << endl; }
    }


    return;
}

//breaks up the command inputs
void Shell::parseCommand(string comm, vector<string>& result) 
{
    istringstream stream(comm);
    for(string comm; stream >> comm;) { result.push_back(comm); }

    return;
}


void Shell::parseGroups(string comm, vector<string>& result)
{
  istringstream stream(comm);
  string group;

  while(stream.good()) 
  {
    string substring; 
    getline(stream, substring, ','); 
    result.push_back(substring); 
  }

  return;
}


string Shell::getCurrentUser() { return current_user->getName(); }

bool Shell::checkIfUserHasPermissions(File file_to_check, int access_mode)
{
  bool access = false;

  access = file_to_check.getProp()->permissions.getPerms(access_mode+6);

  if((file_to_check.getProp()->owner) == (current_user->getName()))
  {
    access = file_to_check.getProp()->permissions.getPerms(access_mode);
  }
  else if((current_user->matchGroup(file_to_check.getProp()->group)) && !access) 
  {
    access = file_to_check.getProp()->permissions.getPerms(access_mode+3);
  }

  return access;

}


/*

          |  COMMANDS |


*/

void Shell::ls(vector<string> comm) 
{
    // ls comm
    if(comm.size() == 1) {
        for(int d = 0; d < curDir->files.size(); d++) {
          if(checkIfUserHasPermissions(curDir->files[d], 0))
          {
            cout << curDir->files[d].getName() << "\t";
          }
        }
        cout << endl;
    }

    // ls -l comm
    else if(comm.size() == 2) {
        if(comm[1] == "-l") {
            for(int d = 0; d < curDir->files.size(); d++) {
              if(checkIfUserHasPermissions(curDir->files[d], 0))
              {
                cout << curDir->files[d].fileInfo() << endl;
              }
            }
        }
        else { cout << invalid_ls  << endl; }
    }

    else if(comm.size() > 2) { cout << invalid_ls << endl; }

    return;
}



void Shell::cd(vector<string> comm)
{
  if(comm.size() == 2)
  {
    if(comm[1] == "..")
    {
      if((curDir->getName() == "root")) { cout << invalid_cd << endl; }
      else { curDir = curDir->getParent(); }
    }
    else
    {
      File* target = NULL;
      string targetDir = comm[1];
      bool dirFound = false;


      for(int d = 0; d < curDir->files.size(); d++)
      {
        if((curDir->files[d].getName() == targetDir) && (curDir->files[d].getFile() == false))
        {
          dirFound = true;
          target = &(curDir->files[d]);
        }
      }
      if(dirFound == false) { cout << invalid_cd << endl; }
      else
      {
        if(checkIfUserHasPermissions(*target, 2))
        {
          target->setParent(curDir);
          curDir = target;
        }

      }
    }
  }
  else
  { cout << invalid_cd << endl; return; }
  return;
}



void Shell::pwd(vector<string> comm) {

    if(comm.size() == 1) {
        string filePath = "";
        File* target = curDir;
        while((target->getParent()) != NULL) { 
            filePath = "/" + target->getName() + filePath; target = target->getParent(); }
        filePath = "root" + filePath;
        cout << filePath << endl;
    }
    else {cout << invalid_pwd << endl; }
    return;
}



void Shell::mkdir(vector<string> comm) {

    if(comm.size() == 2) {
        string fileName = comm[1];
        bool dirExists = false;

        for(int d = 0; d < curDir->files.size(); d++) {
            if(curDir->files[d].getName() == fileName) { dirExists = true; }
        }
        if(dirExists == true) { cout << invalid_mkdir << endl; return; }
        else 
        { 
          File new_file(fileName, false, current_user->getName(), current_user->getGroup()); 
          new_file.setParent(curDir); 
          curDir->files.push_back(new_file); 
          return; 
        }
    }

    else { cout << invalid_mkdir << endl; return; }
}

void Shell::rmdir(vector<string> comm) {

    if(comm.size() == 2) {
        string targetDir = comm[1];
        vector<File>::iterator x;
        x = find_if(curDir->files.begin(), curDir->files.end(), [&targetDir](File const& target){
            return((target.getName() == targetDir) && (target.getFile() == false)); });

        if(x == curDir->files.end()) { cout << invalid_rmdir << endl; }
        else {
            if(checkIfUserHasPermissions(*x, 1))
            {
              int targetDir = distance(curDir->files.begin(), x);
              curDir->files.erase(curDir->files.begin() + targetDir);
            }
        }
    }
    else { cout << invalid_rmdir << endl; }



    return;
}

void Shell::rm(vector<string> comm) {

    if(comm.size() == 2) {
        string fileToRemove = comm[1];
        vector<File>::iterator x;
        x = find_if(curDir->files.begin(), curDir->files.end(), [&fileToRemove](File const& target)
            {return((target.getName() == fileToRemove) && (target.getFile() == true)); });

        if(x != curDir->files.end()) {
            if(checkIfUserHasPermissions(*x, 1))
            {
              int indexing = distance(curDir->files.begin(), x);
              curDir->files.erase(curDir->files.begin() + indexing);
            }
        }
    }
    else { cout << invalid_rm << endl; }

    return;
}

void Shell::chmod(vector<string> comm) {

    if(comm.size() == 3) {
        string modFile = comm[2];
        vector<File>::iterator x;

        x = find_if(curDir->files.begin(), curDir->files.end(), [&modFile](File const& target)
            { return (target.getName() == modFile); }  );
        
        if(x != curDir->files.end()) {
            if(checkIfUserHasPermissions(*x, 1)) 
            {
              int indexing = distance(curDir->files.begin(), x);
              curDir->files[indexing].getProp()->permissions.setPerms(stoi(comm[1]));
            }
        }
    }
    else { cout << invalid_chmod << endl; }
    return;
}

void Shell::touch(vector<string> comm) {

    if(comm.size() == 2) {
        Properties* target;
        string fileName = comm[1];
        bool fileExists = false;

        for(int d = 0; d < curDir->files.size(); d++) {
            if(curDir->files[d].getName() == fileName) {
              if(checkIfUserHasPermissions(curDir->files[d], 1))
              {
                fileExists = true;
                target = curDir->files[d].getProp();
              }
            }
        }
        if(fileExists == true) { time(&target->timestamp); }
        else{
            //Generate a random number between 10-50 to set as the runtime for a newly created file
            srand(time(NULL));
            int runtime = rand()%40 + 10;
            //cout << runtime << endl;
            File new_file(fileName, true, current_user->getName(), current_user->getGroup());
            new_file.setParent(curDir);
            new_file.setRunTime(runtime);
            curDir->files.push_back(new_file);
        }
        return;
    }
    else { cout << invalid_touch << endl; return; }

}




/*

    USER commands


*/ 


void Shell::useradd(vector<string> comm)
{

  if(comm.size() != 1)
  {
    if(comm[1] == "-G")
    {
      if(comm.size() != 4) { cout << invalid_useradd << endl; return; }

      string username = comm[3];

      vector<User>::iterator x;
      x = find_if(users.begin(), users.end(), [&username](User const& target) { return (target.getName() == username); });

      if(x == users.end())
      {
        vector<string> result;
        parseGroups(comm[2], result);
        /// need to check if group actually exists
        /// TODO: !!!
        /// can be done using iterator and find_if
        users.push_back(User(username, result));
      }
      else { cout << " USER ALREADY EXISTS " << endl; }
      
    }
    else 
    {
      if(comm.size() != 2) { cout << invalid_useradd << endl; return; }

      string username = comm[1];

      vector<User>::iterator x;
      x = find_if(users.begin(), users.end(), [&username](User const& target) { return (target.getName() == username); });
      if(x == users.end()) { users.push_back(User(username)); }
      else { cout << " USER ALREADY EXISTS " << endl; return; }
    }

  }
  else if(comm.size() == 1) { cout << invalid_useradd << endl; return; }

  return; 

}


// !!! CURRENTLY DOES NOT SUPPORT usermod -g  <group> <user>
void Shell::usermod(vector<string> comm)
{
  if(comm.size() == 4) 
  {
    if((comm[1] == "-a") && (comm[2] == "-G"))
    {
      string addgrp = comm[3];
      vector<string>::iterator x;
      x = find_if(s_groups.begin(), s_groups.end(), [&addgrp](string const& target) { return(target == addgrp); });

      if(x != s_groups.end()) { current_user->setGroup(addgrp); }
      else {cout << "GROUP DOES NOT EXIST" << endl; }
    }
    else { cout << invalid_usermod << endl; return; }
  }
  else if(comm.size() != 4) { cout << invalid_usermod << endl; return; }

  return;

}

void Shell::userdel(vector<string> comm)
{
  if( comm.size() > 1)
  {
    if(comm[1] == "-G")
    {
      if(comm.size() != 4) {cout << invalid_userdel << endl; return; }

      string username = comm[3];
      vector<User>::iterator x;

      x = find_if(users.begin(), users.end(), [&username](User const& target) { return (target.getName() == username); });


      if( x != users.end())
      {
        string remgrp = comm[2];
        for(int i = 0; i < current_user->user_groups.size(); i++)
        {
          if(current_user->user_groups[i] == remgrp)
          {
            current_user->user_groups.erase(current_user->user_groups.begin()+i);
          }
        }
      }
      else { cout << "USER DOES NOT EXIST" << endl; return;} 
    }
    else
    {
      if(comm.size() != 2) { cout << invalid_userdel << endl; return; }

      string username = comm[1]; 
      vector<User>::iterator x;
      x = find_if(users.begin(), users.end(), [&username](User const& target) { return (target.getName() == username); });

      if(x != users.end()) { users.erase(users.begin()+(distance(users.begin(), x))); }
    }


  }

  else {cout << invalid_userdel << endl; return; }

  return;
}

void Shell::chuser(vector<string> comm)
{
  if(comm.size() == 2)
  {
    string username = comm[1];
    vector<User>::iterator x;
    x = find_if(users.begin(), users.end(), [&username](User const& target) { return (target.getName() == username); });

    if( x != users.end()) 
    {
      User &new_user = *x;
      current_user = &new_user;
    }
    else { cout << "USER DOES NOT EXIST" << endl; return; }

  }
  else { cout << invalid_chuser << endl; return; }

  return;  
}


void Shell::listusers(vector<string> comm)
{
  if(comm.size() == 1) 
  {
    for(int u = 0; u < users.size(); u++)
    {
      cout << users[u].getName() << "\t";
    }
    cout << endl; 
  }
  else { cout << "ERROR" << endl; return; }

  return;
}

/* 

    | GROUP COMMANDS


    TODO: ADD groupdel
*/



void Shell::groupadd(vector<string> comm)
{
  if(comm.size() == 2)
  {
    string addgrp = comm[1];
    vector<string>::iterator x;

    x = find_if(s_groups.begin(), s_groups.end(),[&addgrp](string const& target) {return (target == addgrp); });

    if(x == s_groups.end()) { s_groups.push_back(addgrp); }
    else { cout << invalid_groupadd << endl; return; }
  }
  else { cout << invalid_groupadd << endl; return; }

  return;

}


//list all users
void Shell::groups(vector<string> comm)
{
  if(comm.size() == 1) 
  {
    for(int i = 0; i < current_user->user_groups.size(); i++)
    {
      cout << current_user->user_groups[i] << "\t";
    }
    cout << endl;
  }
  else { cout << invalid_groups << endl; return; }
  
  return;

}


void Shell::chgrp(vector<string> comm)
{
  if(comm.size() == 3)
  {
    Properties *target_prop;
    string newgrp = comm[1];
    string changefile = comm[1];

    vector<string>::iterator g;
    g = find_if(s_groups.begin(), s_groups.end(), [&newgrp](string const& target) { return (target == newgrp); });

    if(g != s_groups.end())
    {
      vector<File>::iterator f;
      f = find_if(curDir->files.begin(), curDir->files.end(), [&changefile](File const& target) { return (target.getName() == changefile); });

      if(f != curDir->files.end())
      {
        if(checkIfUserHasPermissions(*f, 1))
        {
          target_prop = curDir->files[distance(curDir->files.begin(), f)].getProp();
          target_prop->group = newgrp;
        }
        else { cout << invalid_chgrp << endl; return; }
      }
      else { cout<< invalid_chgrp << endl; return; }
    }
    else {cout << "GROUP DOES NOT EXIST" << endl; return; }

  }
  else if(comm.size() != 3) { cout << invalid_chgrp << endl; return; }

  return; 
}






/////

/////


//change owner
void Shell::chown(vector<string> comm) 
{
  if(comm.size() == 3) 
  {
    Properties* target_prop;
    string new_owner = comm[1];
    string changefile = comm[2];

    vector<User>::iterator u;
    u = find_if(users.begin(), users.end(), [&new_owner](User const& target) { return (target.getName() == new_owner); });

    if(u != users.end()) 
    {
      vector<File>::iterator f;
      f = find_if(curDir->files.begin(), curDir->files.end(), [&changefile](File const& target) { return (target.getName() == changefile); });

      if( f != curDir->files.end())
      {
        if(checkIfUserHasPermissions(*f, 1))
        {
          target_prop = curDir->files[distance(curDir->files.begin(), f)].getProp();
          target_prop->owner = new_owner;
        }
        else { cout << invalid_chown << endl; return; }
      }
      else { cout << invalid_chown << endl; return; }
    }
    else { cout << "USER DOES NOT EXIST" << endl; return; }


  }
  else { cout << invalid_chown << endl; return; }

  return;
}


/*

// moved to main.cpp 

void Shell::run(vector<string> comm)
{
  if(comm.size() == 1)
  {
    string fname = comm[0];
    fname = fname.substr(2);
    int index = -1;
    bool found = false;

    for(int i = 0; i < curDir->files.size(); i++) 
    {
      if((curDir->files[i].getName() == fname) && (curDir->files[i].getFile()))
      {
        found = true;
        index = i;
        break;
      }
    }

    if(found == false) {cout << "FILE DOES NOT EXIST" << endl; return; }
    else
    {
      if(checkIfUserHasPermissions(curDir->files[index], 2)) { cout << fname << " executed" << endl; }
      else {cout << "invalid execute permissions" << endl; return; }
    }  
  }
  else { cout << invalid_run << endl; return; }


  return; 
}

*/