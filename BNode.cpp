#include "BNode.h"
#include <string>
#include <cstdio>
#include <iostream>

BNode::BNode() {
	// initializing children pointer and setting to NULL
	this->parent = NULL;
	this->children = new BNode*[5];
	for(int i = 0; i < 5; i++) {
		this->children[i] = NULL;
	}
	this->data = new pair<string,int>[4];
	for(int i = 0; i < 4; i++) {
		this->data[i].first = "~~~~~~~~~~~";
		this->data[i].second = -1;
	}
	this->degree = 0;
}

bool BNode::isLeaf() const {
	if(data[3].second == -1)
		return true;
	return false;
}

void BNode::setLeaf(bool is_leaf) {
	if(is_leaf == true) {
		data[3].second = -1;
		return;
	}
	data[3].second = -10;
}

void BNode::getInfo() {
	if(isLeaf() == true)
		cout << "LEAF ";
	else
		cout << "INTERNAL NODE ";
	cout << "DEGREE: " << this->degree;
	cout << " ELEMENTS: ";
	for(int i=0; i < this->degree; i++) {
		cout << this->data[i].first << " ";
	}
	cout << endl;
}
