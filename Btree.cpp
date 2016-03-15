#include "Btree.h"
#include "BNode.h"
#include "SpecialBNode.h"
#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

Btree::Btree(){
	root = new BNode();
}

void Btree::debug() {
  BNode* tmp = findLeaf("Easton");
  tmp->getInfo();
  tmp->parent->getInfo();
  tmp->parent->children[0]->getInfo();
  tmp->parent->children[1]->getInfo();
  tmp->parent->parent->getInfo();
  tmp->parent->parent->children[1]->getInfo();

}

void Btree::printAll() {
	root->getInfo();
	printAllHelper(root);
}

void Btree::printAllHelper(BNode* n) {
	if(n == NULL)
		return;
	if(n->isLeaf()) {
		return;
	}
	else {
		for(int i=0; i < 5; i++) {
			if(n->children[i] != NULL && i <= n->degree) {
				n->children[i]->getInfo();
				printAllHelper(n->children[i]);
			}
		}
	}
}

BNode* Btree::findLeaf(const string &name) const {
	BNode *tmp = root;

	while(tmp->data[3].second!=-1) { // indicates if it is leaf or not
		if (name < tmp->data[0].first)
			tmp = tmp->children[0];
		else if (name < tmp->data[1].first)
			tmp = tmp->children[1];
		else if (tmp->degree==2 || name < tmp->data[2].first)
			tmp = tmp->children[2];
		else if (tmp->degree==3 || name < tmp->data[3].first)
			tmp = tmp->children[3];
		else
			tmp = tmp->children[4];
	}
	return tmp;
}

void Btree::sort(pair<string, int> *data, const int &degree) {
	pair<string, int> temp;
	for(int i=0; i < degree; i++) {
		for(int j = (i + 1); j < degree; j++) {
			if(data[i].first > data[j].first) {
				temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}
	}
}

int Btree::findIndex(BNode* parent, pair<string,int> data) {

	if (data.first<parent->data[0].first)
		return 0;
	else if (parent->degree == 1 || data.first<parent->data[1].first)
		return 1;
	else if (parent->degree==2 || data.first<parent->data[2].first)
		return 2;
	else if (parent->degree==3 || data.first < parent->data[3].first)
		return 3;
	else
		return 4;

}

void Btree::splitLeaf(BNode* leaf, BNode* parent, pair<string,int> new_data) {
	BNode* n1; BNode* n2; int i;
	if(parent != NULL) { // LEAF ALREADY HAS PARENT WHICH IS NOT FULL
		n1 = new BNode();
		for(i=0; i < 2; i++) {
			n1->data[i] = leaf->data[i+2];
			leaf->data[i+2].first = "~~~~~~~~~~~~";
			leaf->data[i+2].second = -10;
		}
		leaf->setLeaf(true);
		leaf->degree = 2; n1->degree = 2;
		i = findIndex(parent, new_data);
		parent->data[parent->degree] = n1->data[0];
		parent->degree++;
		sort(parent->data, parent->degree);
		for(int j = parent->degree; j > i + 1; j--) {
			parent->children[j] = parent->children[j - 1];
		}
		parent->children[i] = leaf; parent->children[i + 1] = n1;
		leaf->parent = parent; n1->parent = parent;
		n1->children[4] = leaf->children[4];
		leaf->children[4] = n1;
	}
	else { // LEAF DOES NOT HAVE PARENT, WHICH MEANS CREATE NEW ROOT
		n1 = new BNode();
		n2 = new BNode();
		parent = new BNode();
		parent->setLeaf(false);
		for(i=0; i < 2; i++) {
			n1->data[i] = leaf->data[i];
			n2->data[i] = leaf->data[i+2];
		}
		n1->degree = 2; n2->degree = 2;
		parent->data[0] = n2->data[0];
		parent->degree++;
		parent->children[0] = n1; parent->children[1] = n2;
		n1->parent = parent; n2->parent = parent;
		n1->children[4] = n2;
		root = parent;
	}
}

void Btree::splitInternal(BNode* leaf, BNode* parent, pair<string,int> new_data) {
	// EVERYTHING UNTIL THE WHILE LOOP IS SPLITTING THE LEAVES AND CREATING NEW PARENTS

	pair<string,int> mid; // CONTAINS THE KEY VALUE WE ARE PUSHING UPWARDS
	BNode* np1; BNode* np2; // np1 = LEFT PARENT, np2 = RIGHT PARENT
	BNode* grandparent = parent->parent; // PARENT OF PARENT
	SpecialBNode* nodeHolder;

	BNode* n1 = new BNode(); // n1 = NEW LEAF NODE (TO THE RIGHT OF LEAF)
	nodeHolder = new SpecialBNode();
	int i = findIndex(parent, new_data);
	pair<string,int>* sorted_data = new pair<string,int>[5];
	for(int j=0; j < 4; j++) {
		nodeHolder->data[j] = parent->data[j];
	}
	nodeHolder->data[4] = leaf->data[2];
	sort(nodeHolder->data, 5);
	for(int j=0; j < 2; j++) {
		n1->data[j] = leaf->data[j+2];
		leaf->data[j+2].first = "~~~~~~~~~~~~";
		leaf->data[j+2].second = -10;
	}
	leaf->setLeaf(true);
	leaf->degree = 2; n1->degree = 2;
	n1->children[4] = leaf->children[4];
	leaf->children[4] = n1;
	mid = nodeHolder->data[2];
	for(int j=0; j<6; j++) {
		if(parent->children[j] != leaf)
			nodeHolder->children[j]=parent->children[j];
		else {
			nodeHolder->children[j]=leaf;
			nodeHolder->children[j+1]=n1;
			while(j<4) {
				nodeHolder->children[j+2]=parent->children[j+1];
				j++;
			}
			break;
		}
	}
	np1 = new BNode(); np2 = new BNode();
	np1->setLeaf(false); np2->setLeaf(false);
	np1->degree = 2; np2->degree = 2;
	for(int j = 0; j < 3; j++) {
		np1->children[j] = nodeHolder->children[j];
	 	nodeHolder->children[j]->parent = np1;
	 	np2->children[j] = nodeHolder->children[j+3];
	 	nodeHolder->children[j+3]->parent = np2;
	}
	for(int j=0; j < 2; j++) {
		np1->data[j] = nodeHolder->data[j];
		np2->data[j] = nodeHolder->data[j + 3];
	}
	while(grandparent != NULL && grandparent->degree == 4) { // KEEP PERCOLATING UP SPLITTING INTERNAL NODES
		nodeHolder = new SpecialBNode();
		for(int j=0; j < 4; j++) {
		 	nodeHolder->data[j] = grandparent->data[j];
		}
		nodeHolder->data[4] = mid;
		sort(nodeHolder->data, 5);
		mid = nodeHolder->data[2];
		for(int j=0; j<6; j++) {
			if(grandparent->children[j]!=parent)
				nodeHolder->children[j]=grandparent->children[j];
			else {
				nodeHolder->children[j]=np1;
				nodeHolder->children[j+1]=np2;
				while(j<4) {
					nodeHolder->children[j+2]=grandparent->children[j+1];
					j++;
				}
				break;
			}
		}
		np1=new BNode(); np2=new BNode();
		np1->setLeaf(false); np2->setLeaf(false);
		np1->degree=2; np2->degree=2;

		for(int j = 0; j < 3; j++) {
			np1->children[j] = nodeHolder->children[j];
		 	nodeHolder->children[j]->parent = np1;
		 	np2->children[j] = nodeHolder->children[j+3];
		 	nodeHolder->children[j+3]->parent = np2;
		}
		for(int j=0; j<2; j++) {
			np1->data[j] = nodeHolder->data[j];
			np2->data[j] = nodeHolder->data[j+3];
		}
		delete nodeHolder;
		nodeHolder = NULL;
		parent = grandparent;
		grandparent = grandparent->parent;
	}
	if(grandparent == NULL) { // WE NEED TO CREATE A NEW ROOT
		grandparent = new BNode();
		grandparent->setLeaf(false);
		grandparent->data[0] = mid;
		grandparent->degree++;
		grandparent->children[0] = np1;
		grandparent->children[1] = np2;
		np1->parent = grandparent; np2->parent = grandparent;
		this->root = grandparent;
	}
	else if(grandparent->degree < 4) { // WE REACHED AN INTERNAL NODE ISN"T FULL
		int index = findIndex(grandparent, mid);
		grandparent->data[grandparent->degree] = mid;
		grandparent->degree++;
		sort(grandparent->data, grandparent->degree);
		for(int j = grandparent->degree; j > index + 1; j--) {
			grandparent->children[j] = grandparent->children[j - 1];
		}
		grandparent->children[index] = np1; grandparent->children[index + 1] = np2;
		np1->parent = grandparent; np2->parent = grandparent;
	}
}

void Btree::insert(const string &name, const int &profile_offset) {
	pair<string, int> new_data;
	new_data.first = name;
	new_data.second = profile_offset;
	if(root->isLeaf() && root->degree < 3) { // CASE WHERE ROOT IS STILL LEAF
		root->data[root->degree] = new_data;
		root->degree++;
		sort(root->data, root->degree);
	}
	else {
		BNode* current_leaf = findLeaf(name);
		BNode* current_parent = current_leaf->parent;
		if(current_leaf->degree < 3) { // CASE WHERE WE CAN JUST INSERT IN LEAF
			int i = findIndex(current_parent, new_data);
			current_leaf->data[current_leaf->degree] = new_data;
			current_leaf->degree++;
			sort(current_leaf->data, current_leaf->degree);
			if(i > 0) { // UPDATE KEY IN PARENT
				current_parent->data[i - 1] = current_leaf->data[0];
				return;
			}
			// THERE IS A CASE HERE WHERE YOU HAVE TO CHANGE THE KEYS ABOVE, IMPLEMENT HERE
			return;
		}
		else if(current_leaf->degree == 3 && current_parent == NULL) { // CASE WHERE WE NEED TO CREATE NEW ROOT FROM LEAVES (FIRST SPLIT)
			current_leaf->data[3] = new_data;
			sort(current_leaf->data, 4);
			splitLeaf(current_leaf, current_parent, new_data);
			return;
		}
		else if(current_leaf->degree == 3 && current_parent->degree < 4) { // CASE WHERE WE NEED TO SPLIT LEAVES BUT NOT PARENT NODE
			current_leaf->data[3] = new_data;
			sort(current_leaf->data, 4);
			splitLeaf(current_leaf, current_parent, new_data);
			return;
		}
		else if(current_leaf->degree == 3 && current_parent->degree == 4) { // CASE WHERE WE NEED TO SPLIT LEAVES AND PARENT NODE
			current_leaf->data[3] = new_data;
			sort(current_leaf->data, 4);
			splitInternal(current_leaf, current_parent, new_data);
			return;
		}
	}
}

void Btree::merge(BNode* leaf, BNode* left, BNode* right, BNode* parent, int index, const string &name) {
	BNode* grandparent = parent->parent;
	if(left) { // MERGE WITH LEFT LEAF
		left->data[2] = leaf->data[0];
		left->degree++;
		if(parent == root && parent->degree == 1) {
			root = left;
			return;
		}
		left->children[4] = leaf->children[4];
		delete leaf;
		leaf = NULL;
		parent->data[index - 1].first = "~~~~~~~~~~~~";
		parent->data[index - 1].second = -10;
		for(int i = index - 1; i + 1 < parent->degree; i++) {
			parent->data[i] = parent->data[i + 1];
			parent->children[i+1] = parent->children[i+2];
		}
		parent->degree--;
	}
	else if (right) { // MERGE WITH RIGHT LEAF
		leaf->data[1] = right->data[0];
		leaf->data[2] = right->data[1];
		leaf->degree += 2;
		if(parent == root && parent->degree == 1) {
			root = leaf;
			return;
		}
		leaf->children[4] = right->children[4];
		delete right;
		right = NULL;
		parent->data[index].first = "~~~~~~~~~~~~";
		parent->data[index].second = -10;
		for(int i = index; i + 1 < parent->degree; i++) {
			parent->data[i] = parent->data[i + 1];
			parent->children[i + 1] = parent->children[i + 2];
		}
		parent->degree--;
		if(grandparent != NULL && index == 0) {
			BNode* n_parent = parent; BNode* n_grandparent = grandparent;
			int grand_index = findIndex(n_grandparent, pair<string,int>(name,0));
			if(grand_index > 0) {
				n_grandparent->data[grand_index - 1] = leaf->data[0];
			}
			else {
				while(grand_index == 0 && n_parent != NULL && n_grandparent != NULL) {
					n_parent = n_grandparent;
					n_grandparent = n_parent->parent;
					if(n_parent != NULL && n_grandparent != NULL)
						grand_index = findIndex(n_grandparent, pair<string,int>(name,0));
				}
				if(grand_index > 0)
					n_grandparent->data[grand_index - 1] = leaf->data[0];
			}
		}
	}
	if(parent->degree >= 2 || parent == root) // PARENT STILL HAS THREE CHILDREN OR IS ROOT
		return;
	else { // INTERNAL NODES NEED TO BORROW OR MERGE
		left = NULL; right = NULL;
		index = findIndex(grandparent, pair<string,int>(name, 0));
		if(index > 0)
			left = grandparent->children[index - 1];
		if(index < 4 && index < grandparent->degree)
			right = grandparent->children[index + 1];
		if(index > 0)
			grandparent->data[index - 1] = grandparent->children[index]->children[0]->data[0];
		while(grandparent != root && parent->degree < 2 && (left == NULL || (left != NULL && left->degree == 2)) && (right == NULL || (right != NULL && right->degree == 2))) { // merge and percolate up
			if(left) { // MERGE WITH LEFT INTERNAL NODE
				left->data[2] = grandparent->data[index - 1];
				left->data[3] = parent->data[0];
				left->children[3] = parent->children[0]; parent->children[0]->parent = left;
				left->children[4] = parent->children[1]; parent->children[1]->parent = left;
				delete parent;
				parent = NULL;
				grandparent->data[index - 1].first = "~~~~~~~~~~~~";
				grandparent->data[index - 1].second = -10;
				left->degree += 2;
				for(int i = index - 1; i + 1 < grandparent->degree; i++) {
					grandparent->data[i] = grandparent->data[i + 1];
					grandparent->children[i+1] = grandparent->children[i+2];
				}
				grandparent->degree--;
				parent = grandparent;
				grandparent = parent->parent;
				left = NULL; right = NULL;
				if(grandparent != NULL) {
					index = findIndex(grandparent, pair<string,int>(name, 0));
					if(index > 0)
						left = grandparent->children[index - 1];
					if(index < 4 && index < grandparent->degree)
						right = grandparent->children[index + 1];
				}
			}
			else if (right) { // MERGE WITH RIGHT INTERNAL NODE
				parent->data[1] = grandparent->data[index];
				parent->data[2] = right->data[0];
				parent->data[3] = right->data[1];
				parent->children[2] = right->children[0]; right->children[0]->parent = parent;
				parent->children[3] = right->children[1]; right->children[1]->parent = parent;
				parent->children[4] = right->children[2]; right->children[2]->parent = parent;
				delete right;
				right = NULL;
				grandparent->data[index].first = "~~~~~~~~~~~~";
				grandparent->data[index].second = -10;
				parent->degree += 3;
				for(int i = index; i + 1 < grandparent->degree; i++) {
					grandparent->data[i] = grandparent->data[i + 1];
					grandparent->children[i + 1] = grandparent->children[i + 2];
				}
				grandparent->degree--;
				parent = grandparent;
				grandparent = parent->parent;
				left = NULL; right = NULL;
				if(grandparent != NULL) {
					index = findIndex(grandparent, pair<string,int>(name, 0));
					if(index > 0)
						left = grandparent->children[index - 1];
					if(index < 4 && index < grandparent->degree)
						right = grandparent->children[index + 1];
				}
			}
		}
		if(parent->degree < 2 && left != NULL && left->degree > 2) { // BORROW FROM LEFT INTERNAL NODE
			parent->data[1] = parent->data[0];
			parent->data[0] = grandparent->data[index - 1];
			grandparent->data[index - 1] = left->data[left->degree - 1];
			left->data[left->degree - 1].first = "~~~~~~~~~~~~";
			left->data[left->degree - 1].second = -10;
			parent->children[2] = parent->children[1];
			parent->children[1] = parent->children[0];
			parent->children[0] = left->children[left->degree];
			left->children[left->degree]->parent = parent;
			parent->degree++;
			left->degree--;
		}
		else if(parent->degree < 2 && right != NULL && right->degree > 2) { // BORROW FROM RIGHT INTERNAL NODE
			parent->data[1] = grandparent->data[index];
			grandparent->data[index] = right->data[0];
			parent->children[2] = right->children[0];
			right->children[0]->parent = parent;
			for(int i=0; i < right->degree; i++) {
				if(i + 1 < right->degree)
					right->data[i] = right->data[i + 1];
				else {
					right->data[i].first = "~~~~~~~~~~~~";
					right->data[i].second = -10;
				}
				right->children[i] = right->children[i + 1];
			}
			parent->degree++;
			right->degree--;
		}
		else if(parent->degree < 2 && grandparent == root) { // REACHED THE ROOT
			if(grandparent->degree == 1) { // PULL DOWN THE ROOT
				if(left) {
					left->data[2] = grandparent->data[0];
					left->data[3] = parent->data[0];
					left->children[3] = parent->children[0]; parent->children[0]->parent = left;
					left->children[4] = parent->children[1]; parent->children[1]->parent = left;
					delete parent;
					parent = NULL;
					delete grandparent;
					grandparent = NULL;
					left->degree += 2;
					root = left;
				}
				else if (right) {
					parent->data[1] = grandparent->data[0];
					parent->data[2] = right->data[0];
					parent->data[3] = right->data[1];
					parent->children[2] = right->children[0]; right->children[0]->parent = parent;
					parent->children[3] = right->children[1]; right->children[1]->parent = parent;
					parent->children[4] = right->children[2]; right->children[2]->parent = parent;
					delete right;
					right = NULL;
					delete grandparent;
					grandparent = NULL;
					parent->degree += 3;
					root = parent;
				}
			}
			else { // REACHED THE ROOT BUT WE STILL HAVE SIBLINGS TO MERGE WITH
				if(left) { // MERGE WITH LEFT INTERNAL NODE (ROOT CASE)
					left->data[2] = grandparent->data[index - 1];
					left->data[3] = parent->data[0];
					left->children[3] = parent->children[0]; parent->children[0]->parent = left;
					left->children[4] = parent->children[1]; parent->children[1]->parent = left;
					delete parent;
					parent = NULL;
					grandparent->data[index - 1].first = "~~~~~~~~~~~~";
					grandparent->data[index - 1].second = -10;
					left->degree += 2;
					for(int i = index - 1; i + 1 < grandparent->degree; i++) {
						grandparent->data[i] = grandparent->data[i + 1];
						grandparent->children[i+1] = grandparent->children[i+2];
					}
					grandparent->degree--;
				}
				else if (right) { // MERGE WITH RIGHT INTERNAL NODE (ROOT CASE)
					parent->data[1] = grandparent->data[index];
					parent->data[2] = right->data[0];
					parent->data[3] = right->data[1];
					parent->children[2] = right->children[0]; right->children[0]->parent = parent;
					parent->children[3] = right->children[1]; right->children[1]->parent = parent;
					parent->children[4] = right->children[2]; right->children[2]->parent = parent;
					delete right;
					right = NULL;
					grandparent->data[index].first = "~~~~~~~~~~~~";
					grandparent->data[index].second = -10;
					parent->degree += 3;
					for(int i = index; i + 1 < grandparent->degree; i++) {
						grandparent->data[i] = grandparent->data[i + 1];
						grandparent->children[i + 1] = grandparent->children[i + 2];
					}
					grandparent->degree--;
				}
			}
		}
	}
}

bool Btree::removeValue(pair<string, int> *data, const int &degree, const string &name) {
	int i; bool removed = false;
	for( i=0; i < degree; i++) {
		if(data[i].first == name) {
			removed = true;
			break;
		}
	}
	if(removed == false)
		return false;
	if(i + 1 == degree) {
		data[i].first = "~~~~~~~~~~~~";
		data[i].second = -10;
	}
	while(i + 1 < degree) {
		data[i] = data[i + 1];
		i++;
	}
	return removed;
}

void Btree::remove(const string &name) {
	BNode* current_leaf = findLeaf(name);
	BNode* parent = current_leaf->parent;
	if(current_leaf == root) { // ROOT IS A LEAF
		if(current_leaf->degree == 0) {
			cout << "Tree is empty" << endl;
			return;
		}
		if(removeValue(current_leaf->data, current_leaf->degree, name))
			current_leaf->degree--;
		else {
			cout << "Name doesn't exist in tree" << endl;
			return;
		}
	}
	else {
		BNode* left_sib = NULL; BNode* right_sib = NULL;
		int index = findIndex(parent, pair<string,int>(name, 0));
		if(index > 0)
			left_sib = parent->children[index - 1];
		if(index < 4 && index < parent->degree)
			right_sib = parent->children[index + 1];
		if(current_leaf->degree == 3) { // JUST DELETE (WILL NEED TO UPDATE KEYS POSSIBLY)
			if(removeValue(current_leaf->data, current_leaf->degree, name)) {
				current_leaf->degree--;
				if(parent != NULL && index > 0) { // UPDATE KEY DIRECTLY IN PARENT
					parent->data[index - 1] = current_leaf->data[0];
				}
				else if(index == 0 && parent != NULL && parent->parent != NULL) { // WILL NEED TO LOOK UPWARDS TO UPDATE KEY
					int grand_index = findIndex(parent->parent, pair<string,int>(name,0));
					while(grand_index == 0 && parent != NULL && parent->parent != NULL) {
						parent = parent->parent;
						if(parent != NULL && parent->parent != NULL)
							grand_index = findIndex(parent->parent, pair<string,int>(name,0));
					}
					if(grand_index > 0) // UPDATE KEY IN INTERNAL NODE HIGHER UP
						parent->parent->data[grand_index - 1] = current_leaf->data[0];
				}
			}
			else {
				cout << "Name doesn't exist in tree" << endl;
				return;
			}
		}
		else if(current_leaf->degree < 3 && ((left_sib != NULL && left_sib->degree == 3) || (right_sib != NULL && right_sib->degree == 3))) { // BORROW FROM LEAF SIBLING
			if(removeValue(current_leaf->data, current_leaf->degree, name))
				current_leaf->degree--;
			else {
				cout << "Name doesn't exist in tree" << endl;
				return;
			}
			if(left_sib != NULL && left_sib->degree == 3) { // BORROW ELEMENTS FROM LEFT LEAF
				parent->data[index - 1] = left_sib->data[2];
				left_sib->data[2].first = "~~~~~~~~~~~~";
				left_sib->data[2].second = -10;
				left_sib->degree--;
				current_leaf->data[current_leaf->degree] = parent->data[index - 1];
				current_leaf->degree++;
				sort(current_leaf->data, current_leaf->degree);
			}
			else if(right_sib != NULL && right_sib->degree == 3) { // BORROW ELEMENTS FROM RIGHT LEAF
				parent->data[index] = right_sib->data[1];
				current_leaf->data[current_leaf->degree] = right_sib->data[0];
				current_leaf->degree++;
				sort(current_leaf->data, current_leaf->degree);
				if(index > 0)
					parent->data[index - 1] = current_leaf->data[0];
				else if(index == 0 && parent != NULL && parent->parent != NULL) { // WILL NEED TO LOOK UPWARDS TO UPDATE KEY
					int grand_index = findIndex(parent->parent, pair<string,int>(name,0));
					while(grand_index == 0 && parent != NULL && parent->parent != NULL) {
						parent = parent->parent;
						if(parent != NULL && parent->parent != NULL)
							grand_index = findIndex(parent->parent, pair<string,int>(name,0));
					}
					if(grand_index > 0) // UPDATE KEY IN INTERNAL NODE HIGHER UP
						parent->parent->data[grand_index - 1] = current_leaf->data[0];
				}
				right_sib->data[0] = right_sib->data[1];
				right_sib->data[1] = right_sib->data[2];
				right_sib->data[2].first = "~~~~~~~~~~~~";
				right_sib->data[2].second = -10;
				right_sib->degree--;
			}
		}
		else if(current_leaf->degree < 3) { // MERGE LEAVES
			if(removeValue(current_leaf->data, current_leaf->degree, name)) {
				current_leaf->degree--;
				if(parent == root && parent->degree == 1) { // ROOT BECOMES A LEAF
					if(left_sib) {
						left_sib->data[2] = current_leaf->data[0];
						left_sib->degree++;
						root = left_sib;
						for(int i=0; i < 5; i++) {
							left_sib->children[i] = NULL;
						}
						delete current_leaf; delete parent;
						return;
					}
					else if(right_sib) {
						current_leaf->data[1] = right_sib->data[0];
						current_leaf->data[2] = right_sib->data[1];
						current_leaf->degree  += 2;
						root = current_leaf;
						for(int i=0; i < 5; i++) {
							current_leaf->children[i] = NULL;
						}
						delete right_sib; delete parent;
						return;
					}
				}
			}
			else {
				cout << "Name doesn't exist in tree" << endl;
				return;
			}
			merge(current_leaf, left_sib, right_sib, parent, index, name); // PROCEED TO MERGE
		}
	}


}

void Btree::printSingle(const int &profile_offset, const bool &age, const bool &occupation) {

    FILE* pFile;
    pFile = fopen("profile_database.txt", "r");
    char info_holder[30];

    // default will print name
    fseek(pFile, 53*profile_offset, SEEK_SET);
    fgets(info_holder, 20, pFile);
    cout << "Name: " << info_holder;

    if(age == true) {
        fseek(pFile, 53*profile_offset + 20, SEEK_SET);
        fgets(info_holder, 3, pFile);
        cout << ", Age: " << info_holder;
    }
    if(occupation == true) {
        fseek(pFile, 53*profile_offset + 23, SEEK_SET);
        fgets(info_holder, 30, pFile);
        cout << ", Occupation: " << info_holder;
    }
    cout << endl;
}

bool Btree::inNetwork(const string &myName) {
	BNode* start=this->findLeaf(myName);
	if(start->degree==0)
		return false;
	if(start->data[0].first==myName)
		return true;
	if((start->degree)>1) {
		if(start->data[1].first==myName)
			return true;
	}
	if((start->degree)>2) {
		if(start->data[2].first==myName)
			return true;
	}
	return false;
}

void Btree::printMain(const string &nameStart, const string &nameEnd, const bool &age, const bool &occupation)  {

	BNode* start = this->findLeaf(nameStart);
	int startValue = 0;
	string endingOne;

	// Special cases if root is leaf
	if (start->degree == 0)
		return;
	if(start->degree == 1) {
		printSingle(start->data[0].second, age, occupation);
		return;
	}

	// special case where the end is smaller than the start of BTree or if start is bigger than the end of Btree
	BNode* firstFinder = root;
	while(firstFinder->children[0]!=NULL)
		firstFinder=firstFinder->children[0];
	if(nameEnd<firstFinder->data[0].first)
	return;
	string lastName;
	BNode* tmp = start;
	while(tmp->children[4]!=NULL)
	  tmp = tmp->children[4];
	lastName = tmp->data[tmp->degree-1].first;
	if (lastName<nameStart)
		return;

	// default for regular case. First node has special checkings
	if(nameStart<=start->data[0].first && nameEnd>=start->data[0].first)
		printSingle(start->data[0].second, age, occupation);
	if(nameStart<=start->data[1].first && nameEnd>=start->data[1].first)
		printSingle(start->data[1].second, age, occupation);
	if(start->degree==3) {
		if(nameStart<=start->data[2].first && nameEnd>=start->data[2].first)
			printSingle(start->data[2].second, age, occupation);
	}
	start = start->children[4];

	// iterative case. Main part of function
	while (start != NULL) {
		if (start->data[0].first <= nameEnd) {
			printSingle(start->data[0].second, age, occupation);
		}
		else
			return;
		if (start->data[1].first <= nameEnd) {
			printSingle(start->data[1].second, age, occupation);
			endingOne=start->data[1].first;
		}
		else
			return;
		if (start->degree==3) {
			if (start->data[2].first <= nameEnd) {
				printSingle(start->data[2].second, age, occupation);
				endingOne=start->data[2].first;
			}
			else
				return;
		}
		start = start->children[4];
	}
	if(!(endingOne<=nameEnd))
		cout << "Reached end of place without end of name being found. Check your search range again." << endl;
}

void Btree::updateProfileOffset(const string &profileName, const int &newOffset) {
	if(newOffset==-1)
		return;
	BNode* tmp = findLeaf(profileName);
	if(tmp->data[0].first==profileName)
		tmp->data[0].second=newOffset;
	else if(tmp->data[1].first==profileName)
		tmp->data[1].second=newOffset;
	else if(tmp->data[2].first==profileName)
		tmp->data[2].second=newOffset;
	else
		cout << "ERROR: consult updateProfileOffset\n";
}

