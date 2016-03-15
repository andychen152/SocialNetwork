#include "BST.h"
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

BST::BST() {
  root = NULL;
}

void BST::insert(const string &name) {
  if(root) {
    this->insertHelper(root, name);
  }
  else {
    root = new Node(name);
  }
}

void BST::printRanged(const string &name_start, const string &name_end) {
  printRangedHelper(this->root, name_start, name_end);
}


void BST::insertHelper(Node* n, const string &name) {
  if(n->name > name) {
    if(!n->left) {
      n->left = new Node(name);
    }
    else {
      insertHelper(n->left, name);
    }
  }
  else {
    if(!n->right) {
      n->right = new Node(name);
    }
    else {
      insertHelper(n->right, name);
    }
  }
}

void BST::printRangedHelper(Node* n, const string &name_start, const string &name_end) {
  //cout << n->name << endl;
  if(!n) return;
  printRangedHelper(n->left, name_start, name_end);
  if(n->name >= name_start && n->name <= name_end)
    cout << n->name << endl;
  printRangedHelper(n->right, name_start, name_end);
}
