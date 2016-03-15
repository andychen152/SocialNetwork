#ifndef __HASH_GRAPH_H__
#define __HASH_GRAPH_H__

#include <string>
#include "Profile.h"
#include <vector>
using namespace std;

struct Bucket {
    string name_key;
    bool removed; // for lazy delete purposes
    Profile* profile;
};

class HashGraph {
private:

    Bucket* graph;
    int num_entries;

    int hash(const string &name_key) const;

    // helper function, returns the Profile pointer of main Profile
    Profile* lookup(const string &name) const;
    void printProfile(const string &name, const bool &age = true, const bool &occ = true, const bool &friends = true) const;


public:
    HashGraph();
    void remove(const string &name);
    void insert(const vector<string> &inputs);

    // pair.first signifies to Btree WHO to change profileOffset. pair.second signifies to what. If it is -1. DO NOT DO ANYTHING. so check that first    
    pair<string,int> deleteFromDatabase(const string &name); 

    // print the whole social network. Name is set on default to print. You have option to choose to print age, occupation, and friends
    void printAll(const bool &age, const bool &occ, const bool &friends) const;

    // print a person's list of friends. Friends name is set to print on default. You have option to print age and occupation of friends alongside
    void printFriends(const string &name, const bool &age = true, const bool &occ = true) const;
    void printFriendList(const string &name);

    bool addFriend(const string &myName, const string &friendName);
    bool removeFriend(const string &myName, const string &friendName);

    int friendNum(const string &myName);
    string getAge(const string &myName);
    string getOcc(const string &myName);

    // Print the graph, useful for debuggin
    void printHash();
};

#endif
