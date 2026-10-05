#pragma once
#include <string>
#include "slist.h"

// вершина стека - голова списка
struct Stack {
    FNode* head;
};

Stack* createStack();
void push(Stack* s, const std::string& value);
bool isEmpty(Stack* s);
std::string pop(Stack* s);
void print(Stack* s);
void freeStack(Stack* s);