#include "HashGraph.h"
#include "Profile.h"

#include <string>
#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>

using namespace std;

HashGraph::HashGraph() {
    this->graph = new Bucket[211];
    for(int i=0; i<211; i++) {
        this->graph[i].profile=NULL;
    }
    this->num_entries = 0;
}

int HashGraph::hash(const string &name_key) const {
    char c;
    int sum = 0;
    for(int i=0; i < name_key.length(); i++) {
        c = name_key[i];
        sum += c;
    }
    return (sum % 211);
}

Profile* HashGraph::lookup(const string &name) const {
    int it = 0;
    int counter = 0; // to keep track of how many searches has been made (Max is 211)
    
    while((graph[(hash(name) + it)%211].profile != NULL || graph[(hash(name) + it)%211].removed == true) && counter < 212) {
        if(graph[(hash(name) + it)%211].removed == true) {
            ++it;
            continue;
        }
        if(graph[(hash(name) + it)%211].name_key == name) {
            return graph[(hash(name) + it)%211].profile;
        }
        ++counter;
        ++it;
    }
    cout << "Profile doesn't exist" << endl;
    return NULL;
}

pair<string,int> HashGraph::deleteFromDatabase(const string &name) {
    pair<string,int> result; // return what the last persons new index is for btree
    Profile* tmp = lookup(name);
    int deletorOffset = tmp->getProfileOffset();
    
    vector<string> fullNewDatabase; // will store the database in string mode

    FILE* pFile;
    int offset=0;
    char info_holder[30];
    pFile=fopen("profile_database.txt", "r+");
    for(int i=0; i<this->num_entries-1; i++) {
        if(i==deletorOffset) { // case where you want to insert last element here
            fseek(pFile,(this->num_entries-1)*53,SEEK_SET);
            fgets(info_holder,20,pFile);
            string n(info_holder);
            fullNewDatabase.push_back(n);
            fseek(pFile,((this->num_entries-1)*53)+20,SEEK_SET);
            fgets(info_holder,3,pFile);
            string a(info_holder);
            fullNewDatabase.push_back(a);
            fseek(pFile,((this->num_entries-1)*53)+23,SEEK_SET);
            fgets(info_holder,30,pFile);
            string o(info_holder);
            fullNewDatabase.push_back(o);
        }
        else {
            fseek(pFile,offset,SEEK_SET);
            fgets(info_holder, 20, pFile);
            string n(info_holder);
            fullNewDatabase.push_back(n);
            fseek(pFile,offset+20,SEEK_SET);
            fgets(info_holder,3, pFile);
            string a(info_holder);
            fullNewDatabase.push_back(a);
            fseek(pFile,offset+23,SEEK_SET);
            fgets(info_holder,30,pFile);
            string o(info_holder);
            fullNewDatabase.push_back(o);
        }
        offset+=53;
    }
    fclose(pFile);

    // putting last name into result.first and new index into result.second
    if(deletorOffset==this->num_entries-1) { // special case which we are deleting last one. btree will do NOTHING in this case
        result.first = "ERROR: should never print be used. Consult HashGraph if printed out.";
        result.second = -1;
    }
    else {
        pFile = fopen("profile_database.txt","r+");
        fseek(pFile,(this->num_entries-1)*53,SEEK_SET);
        fgets(info_holder,20,pFile);
        string n(info_holder);
        result.first = n;
        result.second = deletorOffset;
        fclose(pFile);
        // updating last person's offset
        Profile *tmpProfile = lookup(result.first);
        tmpProfile->setProfileOffset(result.second);
    }

    this->num_entries--;
    offset=0;

    // create new profile_database and write in it
    pFile=fopen("profile_database.txt", "w+");
    for(int i=0; i<this->num_entries; i++) {
        const char * name_holder = fullNewDatabase[i*3].c_str();
        const char * age_holder = fullNewDatabase[(i*3)+1].c_str();
        const char * occupation_holder = fullNewDatabase[(i*3)+2].c_str();
        fseek(pFile,i*53,SEEK_SET);
        fputs(name_holder,pFile);
        fseek(pFile,(i*53)+20,SEEK_SET);
        fputs(age_holder,pFile);
        fseek(pFile,(i*53)+23,SEEK_SET);
        fputs(occupation_holder,pFile);
    }
    fclose(pFile);
    return result;
}

void HashGraph::remove(const string &name) {
    int it = 0;
    int counter = 0; // to keep track of how many searches has been made (Max is 211)
    
    while((graph[(hash(name) + it)%211].profile != NULL || graph[(hash(name) + it)%211].removed == true) && counter < 212) {
        if(graph[(hash(name) + it)%211].name_key == name) {
            Profile* profile = graph[(hash(name) + it)%211].profile;
            if(profile->getNextFriend() == NULL) {
                graph[(hash(name) + it)%211].name_key = "";
                graph[(hash(name) + it)%211].removed = true;
                delete graph[(hash(name) + it)%211].profile;
                graph[(hash(name) + it)%211].profile = NULL;
                return;
            }
            while(profile->getNextFriend() != NULL) {
                string friend_name = profile->getNextFriend()->getName();
                profile->removeFriend(friend_name);
                Profile* friend_profile = lookup(friend_name);
                friend_profile->removeFriend(name);
            }
            graph[(hash(name) + it)%211].name_key = "";
            graph[(hash(name) + it)%211].removed = true;
            delete graph[(hash(name) + it)%211].profile;
            graph[(hash(name) + it)%211].profile = NULL;
            return;
        }
        ++counter;
        ++it;
    }
    return;
}

void HashGraph::insert(const vector<string> &inputs) {
    int it = 0;
    while(graph[(hash(inputs[0]) + it)%211].profile != NULL) {
        ++it;
    }
    graph[(hash(inputs[0]) + it)%211].name_key = inputs[0];
    graph[(hash(inputs[0]) + it)%211].profile = new Profile(inputs[0],num_entries,inputs[1],inputs[2]);
    ++num_entries;
    for(int i = 3; i < inputs.size(); i++) {
        if (inputs[i]!="")
            graph[(hash(inputs[0]) + it)%211].profile->addFriend(inputs[i]);
    }
    return;
}

void HashGraph::printAll(const bool &age,const bool &occ,const bool &friends) const {
    if(num_entries == 0) {
        cout << "Social Network is empty" << endl;
        return;
    }
    FILE *pFile;
    pFile = fopen("profile_database.txt", "r+");
    char info_holder[30];
    int offset = 53;
    for(int i=0; i < num_entries; i++) {
        fseek(pFile, offset*i, SEEK_SET);
        fgets(info_holder, 20, pFile);
        string str(info_holder);
        printProfile(str, age, occ, friends);
    }
    fclose(pFile);
    return;
}

bool HashGraph::addFriend(const string &myName, const string &friendName) {
    Profile* my_profile = lookup(myName);
    if(my_profile == NULL)
        return false;
    Profile* friend_profile = lookup(friendName);
    if(friend_profile == NULL)
        return false;
    my_profile->addFriend(friendName);
    friend_profile->addFriend(myName);
    return true;
}

bool HashGraph::removeFriend(const string &myName, const string &friendName) {
    Profile* my_profile = lookup(myName);
    if(my_profile == NULL)
        return false;
    Profile* friend_profile = lookup(friendName);
    if(friend_profile == NULL)
        return false;
    my_profile->removeFriend(friendName);
    friend_profile->removeFriend(myName);
    return true;
}

void HashGraph::printFriends(const string &name, const bool &age, const bool &occ) const {
    Profile* my_profile = lookup(name);
    if (my_profile == NULL)
        return;
    Profile* friends = my_profile->getNextFriend();
    if(friends == NULL) {
        cout << "You have no friends" << endl;
        return;
    }
    while(friends != NULL) {
        printProfile(friends->getName(), age, occ, false);
        friends = friends->getNextFriend();
    }
    return;
}

void HashGraph::printFriendList(const string &name) {
    Profile* my_profile = lookup(name);
    if (my_profile==NULL)
        return;
    Profile* friends = my_profile->getNextFriend();
    while(friends!=NULL) {
        if(friends->getNextFriend()!=NULL)
            cout << friends->getName() << ", ";
        else
            cout << friends->getName();
        friends=friends->getNextFriend();
    }
}

void HashGraph::printProfile(const string &name,const bool &age,const bool &occ,const  bool &friends) const {
    int it = 0;
    Profile* profile = lookup(name);
    if(profile == NULL)
        return;
    cout << "Name: " << name;
    int profile_offset = profile->getProfileOffset();
    FILE* pFile;
    pFile = fopen("profile_database.txt", "r+");
    char info_holder[30];
    if(age == true) {
        fseek(pFile, 53*profile_offset + 20, SEEK_SET);
        fgets(info_holder, 3, pFile);
        cout << ", Age: " << info_holder;
    }
    if(occ == true) {
        fseek(pFile, 53*profile_offset + 23, SEEK_SET);
        fgets(info_holder, 30, pFile);
        cout << ", Occupation: " << info_holder;
    }
    if(friends == true) {
        cout << ", Friends:";
        profile = profile->getNextFriend();
        while(profile != NULL) {
            cout << " " << profile->getName();
            profile = profile->getNextFriend();
        }
    }
    cout << endl;
    
    return;
}

string HashGraph::getOcc(const string &myName) {
    int it = 0;
    Profile* profile = lookup(myName);
    if(profile == NULL)
        return NULL; 
    int profile_offset = profile->getProfileOffset();
    FILE* pFile;
    pFile = fopen("profile_database.txt", "r+");
    char info_holder[30];
    fseek(pFile, 53*profile_offset + 23, SEEK_SET);
    fgets(info_holder, 30, pFile);
    return string(info_holder);    
}

string HashGraph::getAge(const string &myName) {
    int it = 0;
    Profile* profile = lookup(myName);
    if(profile == NULL)
        return NULL; 
    int profile_offset = profile->getProfileOffset();
    FILE* pFile;
    pFile = fopen("profile_database.txt", "r+");
    char info_holder[3];
    fseek(pFile, 53*profile_offset + 20, SEEK_SET);
    fgets(info_holder, 3, pFile);
    return string(info_holder);
}

void HashGraph::printHash() {
    for(int i=0; i < 211; i++) {
        if(graph[i].profile != NULL) {
            cout << "Name Key: " << graph[i].name_key;
            if(graph[i].removed == true)
                cout << ", Removed: True";
            cout << ", Removed: False";
            cout << endl;
        }
    }
    return;
}

int HashGraph::friendNum(const string &myName) {
    int counter = 0;
    Profile* tmp = lookup(myName);
    while(tmp->getNextFriend()!=NULL){
        tmp=tmp->getNextFriend();
        counter++;
    }
    return counter;
}

