#ifndef __BNode_H__
#define __BNode_H__

#include <string>
#include <utility>
#include <cstdio>
using namespace std;

class BNode {

  // instance variables
 private:
 	BNode* parent;
 	BNode** children; // size is 5 (index 0, 1, 2, 3, 4)
 	pair<string, int> *data; // size is 4 (index 0, 1, 2,3 )
 	int degree; // In Internal node, has number of children (0-5), In context of leaf it has number of elements (either 2 or 3)
 public:
  // Need multiple constructors. Known: 1 for leaf, 1 for regular(3 child), 1 for root(2 child)
  BNode();
  bool isLeaf() const;
  void setLeaf(bool isLeaf);
  void getInfo(); // FOR DEBUGGING
  friend class Btree;
};

#endif
