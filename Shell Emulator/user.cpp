//tanner hermann
//final project
//user.cpp

#include "user.h"


void User::setName(string name) { user_name = name; }

string User::getName() const { return user_name; }

void User::setGroup(string group_name) {
  user_groups.push_back(group_name);
  sort(user_groups.begin(), user_groups.end());
  user_groups.erase(unique(user_groups.begin(), user_groups.end()), user_groups.end());
}

string User::getGroup() const { return user_groups[0]; }

bool User::matchGroup(string group_name) {
  vector<string>::iterator x;

  x = find_if(user_groups.begin(), user_groups.end(), [&group_name](string const& target) { return (target == group_name); } );

  if(x != user_groups.end()) { return true; }
  else { return false; }
}