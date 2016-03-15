#include "SocialNetwork.h"
#include <string>
#include <utility>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <vector>
#include "HashGraph.h"
#include "Profile.h"
#include "Btree.h"
#include "BNode.h"

SocialNetwork::SocialNetwork() {
	h = new HashGraph();
	b = new Btree();
    num_entries = 0;
}

void SocialNetwork::insert(const string &line) {
    stringstream ss(line);
    vector<string> inputs;

    while(ss.good()) {
        string substr;
        getline( ss, substr, ',' );
        inputs.push_back(substr);
    }

    h->insert(inputs);
    b->insert(inputs[0], num_entries);
    num_entries++;
}

void SocialNetwork::remove(const string &profileName) {
    this->num_entries--;
    pair<string,int> profileDatabaseInformation = h->deleteFromDatabase(profileName);
    h->remove(profileName);
    b->remove(profileName);
    b->updateProfileOffset(profileDatabaseInformation.first,profileDatabaseInformation.second);
}

// special function instructor wants. will be a clickable button on main page.
void SocialNetwork::printAll(const bool &age, const bool &occ, const bool &friends) const {
    h->printAll(age, occ, friends);
}

// print function located in profile page
void SocialNetwork::printFriends(const string &name, const bool &age, const bool &occ) const {
    h->printFriends(name, age, occ);
}

void SocialNetwork::printRanged(const string &nameStart, const string &nameEnd, const bool &age, const bool &occupation) const {
    b->printMain(nameStart, nameEnd, age, occupation);
}
