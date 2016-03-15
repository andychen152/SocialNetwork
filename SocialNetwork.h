#ifndef __SOCIAL_NETWORK_H__
#define __SOCIAL_NETWORK_H__

#include <string>
#include <utility>
#include <cstdio>
#include <vector>
#include "HashGraph.h"
#include "Profile.h"
#include "Btree.h"
#include "BNode.h"
using namespace std;

class SocialNetwork {
 private:
  HashGraph* h;
  Btree* b;
  int num_entries;
 public:
  // only needs
  SocialNetwork();
  void insert(const string &line);
  void remove(const string &profileName);
  void printAllB() {b->printAll();}

  // special function instructor wants. will be a clickable button on main page.
  void printAll(const bool &age = true, const bool &occ = true, const bool &friends = true) const;

  // print function located in profile page
  void printFriends(const string &name, const bool &age = true, const bool &occ = true) const;

  void printRanged(const string &nameStart, const string &nameEnd, const bool &age = true, const bool &occupation = true) const;

  int friendNum(const string &myName) {return h->friendNum(myName);}
  void printFriendList(const string &name) {h->printFriendList(name);}

  bool addFriend(const string &myName, const string &friendName){return h->addFriend(myName, friendName);}
  bool removeFriend(const string &myName, const string &friendName){return h->removeFriend(myName, friendName);}
  string getAge(const string &myName) {return h->getAge(myName);}
  string getOcc(const string &myName) {return h->getOcc(myName);}
  bool inNetwork(const string &myName) {return b->inNetwork(myName);}
  int getNumEntries() {return this->num_entries;}
};

#endif
