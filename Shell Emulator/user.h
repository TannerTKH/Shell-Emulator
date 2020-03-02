//tanner hermann
//final project
//user.h


#ifndef USER_H
#define USER_H

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class User {
  private:
      string user_name;

  public:
      vector<string> user_groups;

      User() { 
        user_name = "default_user";
        user_groups.push_back("users");
      }

      User(string name) {
        user_name = name;
        user_groups.push_back("users");

      }

      User(string name, vector<string> groups) {
        user_name = name;
        user_groups.push_back("users");
        user_groups.insert(user_groups.end(), groups.begin(), groups.end());

      }

      void setName(string name);
      string getName() const;

      void setGroup(string group_name);
      string getGroup() const;

      bool matchGroup(string group_name);


};

#endif