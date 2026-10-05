#include <iostream>
#include <string>
#include "stack.h"
using namespace std;

Stack* createStack() {
    return new Stack{nullptr};
}

void push(Stack* s, const string& value) {
    FNode* n = new FNode{value, s->head};
    s->head = n;
}

bool isEmpty(Stack* s) {
    return s->head == nullptr;
}

// возвращает снятый элемент; перед вызовом проверять isEmpty
string pop(Stack* s) {
    if (s->head == nullptr) return "";
    FNode* old = s->head;
    string value = old->data;   // сохраняем до delete
    s->head = old->next;
    delete old;
    return value;
}

void print(Stack* s) {
    FNode* cur = s->head;
    if (cur == nullptr) { cout << "(empty)" << endl; return; }
    cout << "top: ";
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void freeStack(Stack* s) {
    while (!isEmpty(s)) pop(s);
    delete s;
}