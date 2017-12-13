//
//  main.cpp
//  merkle_tree
//
//  Created by Sindhu Balakrishnan on 12/3/17.
//  Copyright © 2017 Sindhu Balakrishnan. All rights reserved.
//

#include <iostream>
#include <limits>
#include <functional>
#include <string>
#include <list>
#include <queue>
#include <algorithm>
using namespace std;

struct merkle_node {
    string data;
    merkle_node* left;
    merkle_node* right;
    bool isLeaf;
};

void getUserCommand();
void addNodeToMerkleTree();
merkle_node* createMerkleNode();
void insertIntoMerkleTree(merkle_node* node);
void printMerkleTree();
void validateMerkleTree();
merkle_node* findFreeNode();
void insertAndRehash(merkle_node* parent);
void createNewVerticalLevel(merkle_node* node);
int height(merkle_node* node);

merkle_node* root;

int main(int argc, const char * argv[]) {
    
    getUserCommand();
    return 0;
}

merkle_node* createMerkleNode(string data, bool isLeaf = false){
    merkle_node* node = new merkle_node();
    hash<string> str_hash;
    node->data = str_hash(data);
    node->left = NULL;
    node->right = NULL;
    node->isLeaf = isLeaf;
    return node;
}

merkle_node* findFreeNode(){
    
    queue<merkle_node*> q;
    q.push(root);
    while(! q.empty()){
        
        merkle_node* node = q.front();
        if(node == NULL) break;
        if((node->left == NULL) || (node->right == NULL)) return node;
        q.push(node->left);
        q.push(node->right);
        q.pop();
    }
    return NULL;
}

int height(merkle_node* node){
    
    if(node == NULL){
        return 0;
    }
    return max(height(node->left), height(node->right)) + 1;
}

void createNewVerticalLevel(merkle_node* node){
    
    int ht = height(root);
    int i=1;
    while(i<ht){
        // hash the data in node->data
        merkle_node* copy = createMerkleNode(node->data);
        copy->left = node;
        node = copy;
        i++;
    }
    // now combine root and node
    merkle_node* oldRoot = root;
    string oldRootData = oldRoot->data;
    root = createMerkleNode(oldRootData.append(node->data));
    root->left = oldRoot;
    root->right = node;
}

void insertAndRehash(merkle_node* parent, merkle_node* child){
    
    (parent->left == NULL) ? parent->left = child : parent->right = child;;
    parent->data = parent->data.append(child->data);
}

void insertIntoMerkleTree(merkle_node* node){

    if(root == NULL){
        root = createMerkleNode(node->data);
        root->left = node;
    } else {
        merkle_node* parent = findFreeNode();
        // if the only available free node is a leaf
        // then the tree is full
        if(parent->isLeaf){
            createNewVerticalLevel(node);
        } else {
            insertAndRehash(parent, node);
        }
    }
}

void addNodeToMerkleTree(){
    
    cout << "Enter the string: ";
    string data;
    cin >> data;
    merkle_node* node = createMerkleNode(data, true);
    insertIntoMerkleTree(node);
    getUserCommand();
}

void validateMerkleTree(){
    cout << "Enter the string to validate (presence in the tree): ";
    string data;
    cin >> data;
    getUserCommand();
}

void printMerkleTree(){
    
    queue<merkle_node*> q;
    q.push(root);
    while(! q.empty()){
        
        merkle_node* node = q.front();
        cout << node->data << endl;
        if(node->left != NULL) q.push(node->left);
        if(node->right != NULL) q.push(node->right);
        q.pop();
    }
    getUserCommand();
}

void getUserCommand() {
    cout << "Enter the command (add, validate, print, end): " << endl;
    string cmd;
    cin>> cmd;
    
    if (cmd.compare("add") == 0) {
        addNodeToMerkleTree();
    }
    else if (cmd.compare("validate") == 0) {
        validateMerkleTree();
    }
    else if (cmd.compare("print") == 0) {
        printMerkleTree();
    }
    else if (cmd.compare("end") == 0) {
        exit(0);
    }
    else {
        cout << "Enter a valid command: add, validate, print or exit" << endl;
        getUserCommand();
    }
}
