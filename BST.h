#ifndef __BST_H__
#define __BST_H__

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class BST {
private:
  struct Node {
    string name;
    Node *left;
    Node *right;

    Node(string name) {
        this->name = name;
        this->left = NULL;
        this->right = NULL;
    }
  };

  Node* root;
  void insertHelper(Node*n, const string& name);
  void printRangedHelper(Node* n, const string &name_start, const string &name_end);

public:
  BST();
  void insert(const string &name);
  void printRanged(const string &name_start, const string &name_end);

};

#endif
