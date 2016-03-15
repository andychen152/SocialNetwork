#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <string>
#include <cstdio>
using namespace std;

class Profile {

  // instance variables
 private:
  string name;
  int profileOffset;
  Profile* next;

 public:
  // constructor for being a "friend" on the graph                                                      
  Profile(const string &name);                                                                                 

  // constructor for being a main profile on the graph. This will save to disk memory.
  Profile(const string &name, const int &profileOffset, const string &age, const string &occupation);                                                                                               

  string getName() const;
  int getProfileOffset() const;
  Profile* getNextFriend() const;
  void setNextFriend(Profile* nextFriend);
  void addFriend(const string &friendName);
  void removeFriend(const string &friendName);
  void setProfileOffset(const int &offset) {this->profileOffset=offset;}

};

#endif
