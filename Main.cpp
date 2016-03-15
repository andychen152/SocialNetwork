// Hashtable Project
// WEIRD BUG PLEASE CHECK IF TIME PERSISTS. OCCUPATION CAN NOT CONTAIN SPACES. LIKELY GOING TO BE ERROR IN NAMES AND FRIENDS

#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Profile.h"
#include "HashGraph.h"
#include "BNode.h"
#include "Btree.h"
#include "BST.h"
#include "SocialNetwork.h"
using namespace std;

string parseName(string line) { //USE parseName FUNCTION TO INSERT INTO BST
  stringstream ss(line);
  vector<string> inputs;
  while(ss.good()) {
      string substr;
      getline( ss, substr, ',' );
      inputs.push_back(substr);
  }
  return inputs[0];
}

int main() {

  // Code for testing BST vs B+ Tree time. DO NOT DELETE CODE
  /*
  clock_t t1,t2;
  t1=clock();
  Btree b;
  BST h;
  std::cout.setstate(std::ios_base::failbit);
  for(int i=0; i<40000; i++) {
    h.insert(to_string(rand()%100000000));
    h.printRanged("3","8");
    //b.insert(to_string(rand()%100000000), 10);
    //b.printMain("3","8",false , false);
  }
  std::cout.clear();
  t2=clock();
  float diff((float)t2-(float)t1);
  cout << diff << endl;
  */

  // Btree b;
  // string inp;

  // ifstream file("input.txt");
  // while(getline(file,inp))
  //   b.insert(parseName(inp), 0);

  // b.printAll();
  // b.remove("Adam");
  // b.printAll();

  ifstream file("input.txt");

  try {
  	string inp;
  	ifstream file("input.txt");

  	SocialNetwork s;

  	try
  	{
  		while(getline(file, inp))
  			s.insert(inp);
  	}
  	catch(exception& ex) {
  		cerr << ex.what() << endl;
  	}

    while(true) {
      char opt;
      cout << "\n[ WELCOME TO OUR SOCIAL NETWORK ]\n";
      cout << "THIS NETWORK CURRENTLY HAS " << s.getNumEntries() << " ACTIVE USERS.\n\n";
      cout << "1: PRINT ALL USER INFORMATION\n";
      cout << "2: ACCESS PROFILE\n";
      cout << "3: RANGED QUERY\n";
      cout << "4: CREATE NEW USER\n";
      cout << "5: DELETE PROFILE\n";
      cout << "6: EXIT\n";
      cin >> opt;
      if(cin.eof()) {
        break;
      }
      if (opt == '1') {
        bool age, occ, friends;
        char ans = ' ';
        while(!(ans == 'y' || ans == 'n')) {
          cout << "Would you like to print age information? (y/n)" << endl;
          cin >> ans; tolower(ans);
          if(ans == 'y') {
            age = true;
          }
          else if (ans == 'n') {
            age = false;
          }
          else {
            cout << "Please answer with y or n." << endl;
          }
        }
        ans = ' ';
        while(!(ans == 'y' || ans == 'n')) {
          cout << "Would you like to print occupation information? (y/n)" << endl;
          cin >> ans; tolower(ans);
          if(ans == 'y') {
            occ = true;
          }
          else if (ans == 'n') {
            occ = false;
          }
          else {
            cout << "Please answer with y or n." << endl;
          }
        }
        ans = ' ';
        while(!(ans == 'y' || ans == 'n')) {
          cout << "Would you like to print friend information? (y/n)" << endl;
          cin >> ans; tolower(ans);
          if(ans == 'y') {
            friends = true;
          }
          else if (ans == 'n') {
            friends = false;
          }
          else {
            cout << "Please answer with y or n." << endl;
          }
          cout << "\n";
        }
        s.printAll(age, occ, friends);
      }
      else if (opt == '2') {
        bool age, occ;
        char ans = ' ';
        string name;
        string userInp;
        string friendInp;
        cout << "Who's profile would you like to enter?" << endl;
        cin.ignore();
        getline(cin,name);

        if(!(s.inNetwork(name)))
          cout << "\nProfile does not exist. Please try again with another name.\n\n";
        else {

          cout << "\nWelcome " << name << "! You currently have " << s.friendNum(name) << " friends.\n"
          << "You are " << s.getAge(name) << " years old. Your occupation is: " << s.getOcc(name) << ".\n\n";

          cout << "Enter \"1\" if you would like to print friend's information.\n"
               << "Enter \"2\" if you would like to add a friend.\n"
               << "Enter \"3\" if you would like to unfriend someone.\n";

          cin >> userInp;
          if(userInp=="1") {
            while(!(ans == 'y' || ans == 'n')) {
              cout << "Would you like to the print age information of " << name << "\'s friends? (y/n)" << endl;
              cin >> ans; tolower(ans);
              if(ans == 'y') {
                age = true;
              }
              else if (ans == 'n') {
                age = false;
              }
              else {
                cout << "Please answer with y or n." << endl;
              }
            }

            ans = ' ';
            while(!(ans == 'y' || ans == 'n')) {
              cout << "Would you like to print the occupation information of " << name << "\'s friends? (y/n)" << endl;
              cin >> ans; tolower(ans);
              if(ans == 'y') {
                occ = true;
              }
              else if (ans == 'n') {
                occ = false;
              }
              else {
                cout << "Please answer with y or n." << endl;
              }
            }
            s.printFriends(name, age, occ);
          }
          else if(userInp=="2") {
            cout << "Who would you like to add as a friend?" << endl;
            cin.ignore();
            getline(cin,friendInp);
            bool caseOP = s.addFriend(name, friendInp);
            if (caseOP)
              cout << "\nCongratulations " << name <<"! You have succesfully added " << friendInp << " as a friend.\n\n";
          }
          else if(userInp=="3") {
            if(s.friendNum(name)==0) {
              cout << "\nSorry. Because you have no friends, you can not unfriend anyone.\n";
            }
            else  {
              cout << "Who would you like to unfriend? Here is a list of your friends:\n" ;
              s.printFriendList(name);
              cout << endl;
              cin.ignore();
              getline(cin,friendInp);
              bool caseOP = s.removeFriend(name, friendInp);
              if (caseOP)
                cout << "\nCongratulations " << name <<"! You have succesfully removed " << friendInp << " as a friend.\n\n";
            }
          }
        }
      }
      else if (opt == '3') {
        string start_name, end_name;
        bool age, occ;
        char ans = ' ';
        cout << "Start my query from:" << endl;
        cin >> start_name;
        cout << "Search from " << start_name << " to:" << endl;
        cin >> end_name;
        while(!(ans == 'y' || ans == 'n')) {
          cout << "Would you like to print age information? (y/n)" << endl;
          cin >> ans; tolower(ans);
          if(ans == 'y') {
            age = true;
          }
          else if (ans == 'n') {
            age = false;
          }
          else {
            cout << "Please answer with y or n." << endl;
          }
        }
        ans = ' ';
        while(!(ans == 'y' || ans == 'n')) {
          cout << "Would you like to print occupation information? (y/n)" << endl;
          cin >> ans; tolower(ans);
          if(ans == 'y') {
            occ = true;
          }
          else if (ans == 'n') {
            occ = false;
          }
          else {
            cout << "Please answer with y or n." << endl;
          }
        }
        s.printRanged(start_name, end_name, age, occ);
      }
      else if (opt == '4') {
        string final_input, str, name;
        int friends;
        cout << "What is the name of the new user?\n";
        cin.ignore();
        getline(cin,str); name=str;
        if(s.inNetwork(name))
          cout << "\nSorry! But \"" << name << "\" is already a taken name.\n";
        else {
          final_input += str + ",";
          cout << "What is " << name << "\'s age?" << endl;
          cin >> str;
          final_input += str + ",";
          cout << "What is " << name << "\'s occupation" << endl;
          cin.ignore();
          getline(cin,str);
          final_input += str;
          cout << "How many friends does " << name << "\'s have? (Please enter a number)" << endl;
          cin >> friends;
          int counter = 1;
          s.insert(final_input);
          while(counter <= friends) {
            cout << "What the the name of friend #" << counter << "?" << endl;
            cin >> str;
            s.addFriend(name, str);
            counter++;
          }
          cout << name << " was successfully inserted into the Social Network!" << endl;
        }
      }
      else if (opt == '5') {
        string profileName;
        cout << "Enter the name of the profile you would like to delete.\n";
        cin >> profileName;
        if(s.inNetwork(profileName)) {
          s.remove(profileName);
          cout << "You have successfully removed " << profileName << "\n";
        }
        else // person is not in network, do not do anthing
          cout << "We could not find this person in our database. Please try again.";
      }
      else if (opt == '6') {
        cout << "Thank you for using our Social Network!" << endl;
        break;
      }
      else {
        cin.clear();
        cout << "Please choose a listed option." << endl;
      }
    }
  }
  catch(exception& ex) {
    cerr << ex.what() << endl;
  }
  return 0;
}
