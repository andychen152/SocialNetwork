#include "BNode.h"

class SpecialBNode:public BNode {
 public:
  SpecialBNode() {
 	// initializing children pointer and setting to NULL
	this->parent = NULL;
	this->children = new BNode*[6];
	for(int i = 0; i < 6; i++) {
		this->children[i] = NULL;
	}
	this->data = new pair<string,int>[5];

	for(int i = 0; i < 5; i++) {
		this->data[i].first = "~~~~~~~~~~~~~~";
		this->data[i].second = -1;
	}
	this->degree = 0;
  }

  friend class Btree;

 private:
 	BNode* parent;
 	BNode** children; // size is 5 (index 0, 1, 2, 3, 4)
 	pair<string, int> *data; // size is 4 (index 0, 1, 2,3 )
 	int degree; // In Internal node, has number of children (0-5), In context of leaf it has number of elements (either 2 or 3)

};