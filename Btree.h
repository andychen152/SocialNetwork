#ifndef __Btree_H__
#define __Btree_H__

#include <string>
#include <utility>
#include <cstdio>
#include "BNode.h"
using namespace std;

class Btree {

  // instance variables
 private:
  BNode* root;

  // Helper functions for insert that split the nodes
  void splitLeaf(BNode* leaf, BNode* parent, pair<string,int> new_data);
  void splitInternal(BNode* leaf, BNode* parent, pair<string,int> new_data);

  // Helper function for remove that merge the nodes if there are no siblings to borrow from
  void merge(BNode* leaf, BNode* left, BNode* right, BNode* parent, int index, const string &name);

  // Helper function for remove that removes correct value from the leaf and returns a boolean value on whether it exists in leaf
  bool removeValue(pair<string, int> *data, const int &degree, const string &name);

  // Sorts keys in node into correct order
  void sort(pair<string, int> *data, const int &degree);

  // Goes into the database and prints out the associated information
  void printSingle(const int &profile_offset, const bool &age, const bool &occupation);

  // Recursive helper for the depth first printAll
  void printAllHelper(BNode* n);

  // Returns the index of the child pointer the name should go to
  int findIndex(BNode* parent, pair<string,int> data);

 public:
  // only needs
  Btree();

  // NOT IMPLEMENT - returns the pointer to leaf of insert
  BNode* getRoot() { return this->root; }

  // Returns pointer to correct leaf in Btree that the name should go
  BNode* findLeaf(const string &name) const;

  // Inserts new name and profile offset into Btree
  void insert(const string &name, const int &profile_offset);

  // Removes name from the Btree if it exists
  void remove(const string &name);

  // Updates the profile offset to match the database
  void updateProfileOffset(const string &profileName, const int &newOffset);

  // Prints a ranged query of the names in Btree
  void printMain(const string &nameStart, const string &nameEnd, const bool &age, const bool &occupation); // DO NOT MAKE CONST, dont ask why

  // Checks if the name is in the Btree
  bool inNetwork(const string &myName);

  // Debugging functions: debug() can be shaped to test specific instances and printAll() will print the entire tree including all internal nodes (depth first)
  void debug();
  void printAll();

  };

#endif
