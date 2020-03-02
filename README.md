# Shell-Emulator
Shell Emulator created in C++. 

This was my final project for my CS3800 - Operating Systems class at Missouri S&T. (Fall 2019)


# Functionality
- Functional Permissions
- Users and Groups
- Process Execution and Scheduling
  +  A fake "Round Robin" process scheduling algorithm
  
  
# Commands
- ls
- pwd
- mkdir
- rmdir
- rm
- touch
- chmod
- useradd
- usermod
- userdel
- chuser
- users
- chown
- chgrp
- groups
- groupadd
- groupdel
- kill
- ps
- schedHist
- "./"
- quit
- exit


# Compile
g++ -std=c++11 main.cpp file.cpp filepermissions.cpp shellfile.cpp user.cpp
