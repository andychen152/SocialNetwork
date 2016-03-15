#include <iostream>
#include "Profile.h"

Profile::Profile(const string &name) {
	this->name = name;
	this->profileOffset = -1;
	next = NULL;
}

Profile::Profile(const string &name, const int &profileOffset, const string &age, const string &occupation){
	this->name = name;
	this->profileOffset = profileOffset;
	next = NULL;

	// adding information to disk
	const char * charName = name.c_str();
	const char * charAge = age.c_str();
	const char * charOccupation = occupation.c_str();

	FILE *pFile;
	if(profileOffset==0)
		pFile = fopen("profile_database.txt", "w+");
	else
		pFile = fopen("profile_database.txt", "r+");
	int offset = (53 * profileOffset); // beginning offset will be index times size of each profile (53 bytes)
	fseek(pFile, offset, SEEK_SET);
	fputs(charName, pFile);
	offset += 20;
	fseek(pFile, offset, SEEK_SET);
	fputs(charAge, pFile);
	offset += 3;
	fseek(pFile, offset, SEEK_SET);
	fputs(charOccupation, pFile);
	fclose(pFile);
}

string Profile::getName() const {
	return this->name;
}

int Profile::getProfileOffset() const {
	return this->profileOffset;
}

Profile* Profile::getNextFriend() const {
	return this->next;
}

void Profile::setNextFriend(Profile* nextFriend) {
	this->next = nextFriend;
}

void Profile::addFriend(const string &friendName) {
	Profile* newFriend = new Profile(friendName);
	Profile *tmp = this;
	while (tmp->next!=NULL)
		tmp = tmp->getNextFriend();
	tmp->next = newFriend;
}

void Profile::removeFriend(const string &friendName) {
    Profile *tmp = this;
    while (tmp->next != NULL) {
        if(tmp->next->name == friendName) {
            Profile* tmp2 = tmp->next->next;
            delete tmp->next;
            tmp->next = tmp2;
            return;
        }
        tmp = tmp->next;
    }
    cout << this->name << " is not friends with " << friendName << endl;
    return;
}
