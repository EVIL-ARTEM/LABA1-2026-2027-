#pragma once
#include <string>
#include "slist.h"

// добавление в хвост, извлечение из головы
struct Queue {
    FNode* head;
    FNode* tail;
};

Queue* createQueue();
void push(Queue* q, const std::string& value);
bool isEmpty(Queue* q);
std::string pop(Queue* q);
void print(Queue* q);
void freeQueue(Queue* q);