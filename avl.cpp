#include <iostream>
#include <string>
#include "avl.h"
using namespace std;

// высота пустого поддерева 0, листа 1
unsigned char getHeight(Node* value) {
    if (value == nullptr) return 0;
    return value->height;
}

int getBalanceFactor(Node* value) {
    return getHeight(value->left) - getHeight(value->right);
}

void fixHeight(Node* value) {
    unsigned char hL = getHeight(value->left);
    unsigned char hR = getHeight(value->right);
    if (hL > hR) value->height = hL + 1;
    else value->height = hR + 1;
}

Node* findMin(Node* value) {
    if (value == nullptr) return nullptr;
    if (value->left == nullptr) return value;
    return findMin(value->left);
}

Node* rotateRight(Node* value) {
    Node* tmp = value->left;
    value->left = tmp->right;
    tmp->right = value;

    fixHeight(value);   // value теперь ниже tmp, считаем его первым
    fixHeight(tmp);
    return tmp;
}

Node* rotateLeft(Node* value) {
    Node* tmp = value->right;
    value->right = tmp->left;
    tmp->left = value;

    fixHeight(value);
    fixHeight(tmp);
    return tmp;
}

Node* balance(Node* value) {
    fixHeight(value);

    if (getBalanceFactor(value) == 2) {
        if (getBalanceFactor(value->left) >= 0) {
            return rotateRight(value);
        }
        else {   // большой поворот
            value->left = rotateLeft(value->left);
            return rotateRight(value);
        }
    }

    if (getBalanceFactor(value) == -2) {
        if (getBalanceFactor(value->right) <= 0) {
            return rotateLeft(value);
        }
        else {   // большой поворот
            value->right = rotateRight(value->right);
            return rotateLeft(value);
        }
    }

    return value;
}

Node* insert(Node* value, int key) {
    if (value == nullptr) return new Node(key);
    if (key < value->key) value->left = insert(value->left, key);
    else if (key > value->key) value->right = insert(value->right, key);
    else return value;

    return balance(value);
}

Node* remove(Node* value, int key) {
    if (value == nullptr) return nullptr;

    if (key < value->key) value->left = remove(value->left, key);
    else if (key > value->key) value->right = remove(value->right, key);
    else {
        if (value->left == nullptr) {
            Node* tmp = value->right;
            delete value;
            return tmp;
        }
        else if (value->right == nullptr) {
            Node* tmp = value->left;
            delete value;
            return tmp;
        }
        // два потомка: заменяем ключом минимума из правого поддерева
        Node* min = findMin(value->right);
        value->key = min->key;
        value->right = remove(value->right, min->key);
    }

    return balance(value);
}

Node* search(Node* value, int key) {
    if (value == nullptr) return nullptr;
    if (key == value->key) return value;
    if (key < value->key) return search(value->left, key);
    return search(value->right, key);
}

void inorder(Node* value) {
    if (value == nullptr) return;
    inorder(value->left);
    cout << value->key << " ";
    inorder(value->right);
}

void preorder(Node* value) {
    if (value == nullptr) return;
    cout << value->key << " ";
    preorder(value->left);
    preorder(value->right);
}

void postorder(Node* value) {
    if (value == nullptr) return;
    postorder(value->left);
    postorder(value->right);
    cout << value->key << " ";
}

int countNodes(Node* value) {
    if (value == nullptr) return 0;
    return 1 + countNodes(value->left) + countNodes(value->right);
}

// обход в ширину через очередь на массиве
void bfs(Node* root) {
    if (root == nullptr) return;
    int n = countNodes(root);
    Node** q = new Node*[n];
    int head = 0, tail = 0;
    q[tail++] = root;
    while (head < tail) {
        Node* cur = q[head++];
        cout << cur->key << " ";
        if (cur->left != nullptr) q[tail++] = cur->left;
        if (cur->right != nullptr) q[tail++] = cur->right;
    }
    delete[] q;
}

// вывод боком: правое поддерево сверху
static void printRec(Node* value, const string& prefix, bool isLeft) {
    if (value == nullptr) return;
    printRec(value->right, prefix + (isLeft ? "|   " : "    "), false);
    cout << prefix << (isLeft ? "\\-- " : "/-- ")
         << value->key << "(h:" << (int)value->height << ")" << endl;
    printRec(value->left, prefix + (isLeft ? "    " : "|   "), true);
}

void print(Node* root) {
    if (root == nullptr) { cout << "(empty)" << endl; return; }
    printRec(root->right, "", false);
    cout << root->key << "(h:" << (int)root->height << ")" << endl;
    printRec(root->left, "", true);
}

void freeTree(Node* value) {
    if (value == nullptr) return;
    freeTree(value->left);
    freeTree(value->right);
    delete value;
}