#pragma once
#include <string>

struct Node {
    int key;
    Node* left;
    Node* right;
    unsigned char height;
    Node(int k) { key = k; left = right = nullptr; height = 1; }
};

unsigned char getHeight(Node* value);
int getBalanceFactor(Node* value);
void fixHeight(Node* value);
Node* findMin(Node* value);
Node* rotateRight(Node* value);
Node* rotateLeft(Node* value);
Node* balance(Node* value);
Node* insert(Node* value, int key);
Node* remove(Node* value, int key);
Node* search(Node* value, int key);
void inorder(Node* value);
void preorder(Node* value);
void postorder(Node* value);
int countNodes(Node* value);
void bfs(Node* root);
void print(Node* root);
void freeTree(Node* value);