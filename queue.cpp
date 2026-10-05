#include <iostream>
#include <string>
#include "queue.h"
using namespace std;

Queue* createQueue() {
    return new Queue{nullptr, nullptr};
}

void push(Queue* q, const string& value) {
    FNode* n = new FNode{value, nullptr};
    if (q->tail == nullptr) {
        q->head = n;
        q->tail = n;
    } else {
        q->tail->next = n;
        q->tail = n;
    }
}

bool isEmpty(Queue* q) {
    return q->head == nullptr;
}

// возвращает извлечённый элемент; перед вызовом проверять isEmpty
string pop(Queue* q) {
    if (q->head == nullptr) return "";
    FNode* old = q->head;
    string value = old->data;   // сохраняем до delete
    q->head = old->next;
    if (q->head == nullptr) q->tail = nullptr;
    delete old;
    return value;
}

void print(Queue* q) {
    FNode* cur = q->head;
    if (cur == nullptr) { cout << "(empty)" << endl; return; }
    cout << "front: ";
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void freeQueue(Queue* q) {
    while (!isEmpty(q)) pop(q);
    delete q;
}