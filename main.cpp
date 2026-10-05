#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "array.h"
#include "slist.h"
#include "dlist.h"
#include "stack.h"
#include "queue.h"
#include "avl.h"
using namespace std;

// все именованные структуры хранятся в односвязном списке
struct Container {
    string name;
    char type;
    Array* arr;
    FList* flist;
    DList* dlist;
    Stack* stack;
    Queue* queue;
    Node* tree;
    Container* next;
};

struct Database {
    Container* head;
};

Container* findContainer(Database* db, const string& name) {
    Container* cur = db->head;
    while (cur != nullptr) {
        if (cur->name == name) return cur;
        cur = cur->next;
    }
    return nullptr;
}

Container* getOrCreate(Database* db, const string& name, char type) {
    Container* c = findContainer(db, name);
    if (c != nullptr) {
        if (c->type != type) return nullptr;
        return c;
    }
    c = new Container{name, type, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    if (type == 'M') c->arr = createArray();
    else if (type == 'F') c->flist = createFList();
    else if (type == 'L') c->dlist = createDList();
    else if (type == 'S') c->stack = createStack();
    else if (type == 'Q') c->queue = createQueue();

    if (db->head == nullptr) db->head = c;
    else {
        Container* last = db->head;
        while (last->next != nullptr) last = last->next;
        last->next = c;
    }
    return c;
}

Container* getExisting(Database* db, const string& name, char type) {
    Container* c = findContainer(db, name);
    if (c == nullptr || c->type != type) return nullptr;
    return c;
}

void freeDatabase(Database* db) {
    Container* cur = db->head;
    while (cur != nullptr) {
        Container* next = cur->next;
        if (cur->type == 'M') freeArray(cur->arr);
        else if (cur->type == 'F') freeList(cur->flist);
        else if (cur->type == 'L') freeList(cur->dlist);
        else if (cur->type == 'S') freeStack(cur->stack);
        else if (cur->type == 'Q') freeQueue(cur->queue);
        else if (cur->type == 'T') freeTree(cur->tree);
        delete cur;
        cur = next;
    }
    db->head = nullptr;
}

// формат файла: <тип> <имя> <элементы...>, одна структура на строку
void loadFile(Database* db, const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) return;
    string line;
    while (getline(in, line)) {
        istringstream ss(line);
        string typeStr, name, item;
        if (!(ss >> typeStr >> name)) continue;
        char type = typeStr[0];
        if (type != 'M' && type != 'F' && type != 'L' && type != 'S' && type != 'Q' && type != 'T') continue;
        Container* c = getOrCreate(db, name, type);
        if (c == nullptr) continue;
        while (ss >> item) {
            if (type == 'M') pushBack(c->arr, item);
            else if (type == 'F') pushBack(c->flist, item);
            else if (type == 'L') pushBack(c->dlist, item);
            else if (type == 'S') push(c->stack, item);
            else if (type == 'Q') push(c->queue, item);
            else if (type == 'T') c->tree = insert(c->tree, stoi(item));
        }
    }
}

// от дна к вершине, чтобы при загрузке порядок восстановился
void saveStackRec(FNode* node, ofstream& out) {
    if (node == nullptr) return;
    saveStackRec(node->next, out);
    out << " " << node->data;
}

// прямой обход
void saveTreeRec(Node* value, ofstream& out) {
    if (value == nullptr) return;
    out << " " << value->key;
    saveTreeRec(value->left, out);
    saveTreeRec(value->right, out);
}

void saveFile(Database* db, const string& filename) {
    ofstream out(filename);
    Container* c = db->head;
    while (c != nullptr) {
        out << c->type << " " << c->name;
        if (c->type == 'M') {
            for (int i = 0; i < c->arr->size; i++) out << " " << c->arr->data[i];
        }
        else if (c->type == 'F') {
            for (FNode* cur = c->flist->head; cur != nullptr; cur = cur->next) out << " " << cur->data;
        }
        else if (c->type == 'L') {
            for (DNode* cur = c->dlist->head; cur != nullptr; cur = cur->next) out << " " << cur->data;
        }
        else if (c->type == 'S') {
            saveStackRec(c->stack->head, out);
        }
        else if (c->type == 'Q') {
            for (FNode* cur = c->queue->head; cur != nullptr; cur = cur->next) out << " " << cur->data;
        }
        else if (c->type == 'T') {
            saveTreeRec(c->tree, out);
        }
        out << endl;
        c = c->next;
    }
}

const int MAX_WORDS = 8;

void printError(const string& msg) {
    cout << "-> ERROR: " << msg << endl;
}

void printBool(bool ok) {
    cout << "-> " << (ok ? "TRUE" : "FALSE") << endl;
}

bool runArray(Database* db, string* w, int n) {
    string cmd = w[0];
    if (n < 2) { printError("no name"); return false; }

    if (cmd == "MPUSH" && n == 3) {
        Container* c = getOrCreate(db, w[1], 'M');
        if (!c) { printError("name used by another type"); return false; }
        pushBack(c->arr, w[2]);
        cout << "-> " << w[2] << endl;
        return true;
    }
    if (cmd == "MINSERT" && n == 4) {
        Container* c = getOrCreate(db, w[1], 'M');
        if (!c) { printError("name used by another type"); return false; }
        if (!insertAt(c->arr, stoi(w[2]), w[3])) { printError("bad index"); return false; }
        cout << "-> " << w[3] << endl;
        return true;
    }

    Container* c = getExisting(db, w[1], 'M');
    if (!c) { printError("array not found"); return false; }

    if (cmd == "MGET" && n == 3) {
        string res;
        if (getAt(c->arr, stoi(w[2]), res)) cout << "-> " << res << endl;
        else printError("bad index");
        return false;
    }
    if (cmd == "MSET" && n == 4) {
        if (!setAt(c->arr, stoi(w[2]), w[3])) { printError("bad index"); return false; }
        cout << "-> " << w[3] << endl;
        return true;
    }
    if (cmd == "MDEL" && n == 3) {
        if (!removeAt(c->arr, stoi(w[2]))) { printError("bad index"); return false; }
        cout << "-> deleted" << endl;
        return true;
    }
    if (cmd == "MLEN" && n == 2) {
        cout << "-> " << length(c->arr) << endl;
        return false;
    }
    printError("unknown command or wrong arguments");
    return false;
}

bool runFList(Database* db, string* w, int n) {
    string cmd = w[0];
    if (n < 2) { printError("no name"); return false; }

    if ((cmd == "FPUSH" || cmd == "FPUSHTAIL" || cmd == "FPUSHHEAD") && n == 3) {
        Container* c = getOrCreate(db, w[1], 'F');
        if (!c) { printError("name used by another type"); return false; }
        if (cmd == "FPUSHHEAD") pushFront(c->flist, w[2]);
        else pushBack(c->flist, w[2]);
        cout << "-> " << w[2] << endl;
        return true;
    }

    Container* c = getExisting(db, w[1], 'F');
    if (!c) { printError("list not found"); return false; }
    string res;

    if (cmd == "FPUSHAFTER" && n == 4) {
        bool ok = pushAfter(c->flist, w[2], w[3]);
        if (ok) cout << "-> " << w[3] << endl; else printError("target not found");
        return ok;
    }
    if (cmd == "FPUSHBEFORE" && n == 4) {
        bool ok = pushBefore(c->flist, w[2], w[3]);
        if (ok) cout << "-> " << w[3] << endl; else printError("target not found");
        return ok;
    }
    if (cmd == "FDELHEAD" && n == 2) {
        bool ok = popFront(c->flist, res);
        if (ok) cout << "-> " << res << endl; else printError("list is empty");
        return ok;
    }
    if (cmd == "FDELTAIL" && n == 2) {
        bool ok = popBack(c->flist, res);
        if (ok) cout << "-> " << res << endl; else printError("list is empty");
        return ok;
    }
    if (cmd == "FDELAFTER" && n == 3) {
        bool ok = removeAfter(c->flist, w[2], res);
        if (ok) cout << "-> " << res << endl; else printError("nothing to delete");
        return ok;
    }
    if (cmd == "FDELBEFORE" && n == 3) {
        bool ok = removeBefore(c->flist, w[2], res);
        if (ok) cout << "-> " << res << endl; else printError("nothing to delete");
        return ok;
    }
    if (cmd == "FDEL" && n == 3) {
        bool ok = removeValue(c->flist, w[2]);
        printBool(ok);
        return ok;
    }
    if (cmd == "FGET" && n == 3) {
        if (getAt(c->flist, stoi(w[2]), res)) cout << "-> " << res << endl;
        else printError("bad index");
        return false;
    }
    if (cmd == "FFIND" && n == 3) {
        printBool(findNode(c->flist, w[2]) != nullptr);
        return false;
    }
    if (cmd == "FPRINTREV" && n == 2) {
        cout << "-> ";
        printReverse(c->flist);
        return false;
    }
    printError("unknown command or wrong arguments");
    return false;
}

bool runDList(Database* db, string* w, int n) {
    string cmd = w[0];
    if (n < 2) { printError("no name"); return false; }

    if ((cmd == "LPUSH" || cmd == "LPUSHTAIL" || cmd == "LPUSHHEAD") && n == 3) {
        Container* c = getOrCreate(db, w[1], 'L');
        if (!c) { printError("name used by another type"); return false; }
        if (cmd == "LPUSHHEAD") pushFront(c->dlist, w[2]);
        else pushBack(c->dlist, w[2]);
        cout << "-> " << w[2] << endl;
        return true;
    }

    Container* c = getExisting(db, w[1], 'L');
    if (!c) { printError("list not found"); return false; }
    string res;

    if (cmd == "LPUSHAFTER" && n == 4) {
        bool ok = pushAfter(c->dlist, w[2], w[3]);
        if (ok) cout << "-> " << w[3] << endl; else printError("target not found");
        return ok;
    }
    if (cmd == "LPUSHBEFORE" && n == 4) {
        bool ok = pushBefore(c->dlist, w[2], w[3]);
        if (ok) cout << "-> " << w[3] << endl; else printError("target not found");
        return ok;
    }
    if (cmd == "LDELHEAD" && n == 2) {
        bool ok = popFront(c->dlist, res);
        if (ok) cout << "-> " << res << endl; else printError("list is empty");
        return ok;
    }
    if (cmd == "LDELTAIL" && n == 2) {
        bool ok = popBack(c->dlist, res);
        if (ok) cout << "-> " << res << endl; else printError("list is empty");
        return ok;
    }
    if (cmd == "LDELAFTER" && n == 3) {
        bool ok = removeAfter(c->dlist, w[2], res);
        if (ok) cout << "-> " << res << endl; else printError("nothing to delete");
        return ok;
    }
    if (cmd == "LDELBEFORE" && n == 3) {
        bool ok = removeBefore(c->dlist, w[2], res);
        if (ok) cout << "-> " << res << endl; else printError("nothing to delete");
        return ok;
    }
    if (cmd == "LDEL" && n == 3) {
        bool ok = removeValue(c->dlist, w[2]);
        printBool(ok);
        return ok;
    }
    if (cmd == "LGET" && n == 3) {
        if (getAt(c->dlist, stoi(w[2]), res)) cout << "-> " << res << endl;
        else printError("bad index");
        return false;
    }
    if (cmd == "LFIND" && n == 3) {
        printBool(findNode(c->dlist, w[2]) != nullptr);
        return false;
    }
    if (cmd == "LPRINTREV" && n == 2) {
        cout << "-> ";
        printReverse(c->dlist);
        return false;
    }
    printError("unknown command or wrong arguments");
    return false;
}

bool runStack(Database* db, string* w, int n) {
    string cmd = w[0];
    if (cmd == "SPUSH" && n == 3) {
        Container* c = getOrCreate(db, w[1], 'S');
        if (!c) { printError("name used by another type"); return false; }
        push(c->stack, w[2]);
        cout << "-> " << w[2] << endl;
        return true;
    }
    if (cmd == "SPOP" && n == 2) {
        Container* c = getExisting(db, w[1], 'S');
        if (!c) { printError("stack not found"); return false; }
        if (isEmpty(c->stack)) { printError("stack is empty"); return false; }
        cout << "-> " << pop(c->stack) << endl;
        return true;
    }
    printError("unknown command or wrong arguments");
    return false;
}

bool runQueue(Database* db, string* w, int n) {
    string cmd = w[0];
    if (cmd == "QPUSH" && n == 3) {
        Container* c = getOrCreate(db, w[1], 'Q');
        if (!c) { printError("name used by another type"); return false; }
        push(c->queue, w[2]);
        cout << "-> " << w[2] << endl;
        return true;
    }
    if (cmd == "QPOP" && n == 2) {
        Container* c = getExisting(db, w[1], 'Q');
        if (!c) { printError("queue not found"); return false; }
        if (isEmpty(c->queue)) { printError("queue is empty"); return false; }
        cout << "-> " << pop(c->queue) << endl;
        return true;
    }
    printError("unknown command or wrong arguments");
    return false;
}

bool runTree(Database* db, string* w, int n) {
    string cmd = w[0];
    if (cmd == "TINSERT" && n == 3) {
        Container* c = getOrCreate(db, w[1], 'T');
        if (!c) { printError("name used by another type"); return false; }
        int key = stoi(w[2]);
        if (search(c->tree, key) != nullptr) { printError("key already exists"); return false; }
        c->tree = insert(c->tree, key);
        cout << "-> " << key << endl;
        return true;
    }

    if (n < 2) { printError("no name"); return false; }
    Container* c = getExisting(db, w[1], 'T');
    if (!c) { printError("tree not found"); return false; }

    if (cmd == "TDEL" && n == 3) {
        int key = stoi(w[2]);
        if (search(c->tree, key) == nullptr) { printBool(false); return false; }
        c->tree = remove(c->tree, key);
        printBool(true);
        return true;
    }
    if ((cmd == "TGET" || cmd == "ISMEMBER") && n == 3) {
        printBool(search(c->tree, stoi(w[2])) != nullptr);
        return false;
    }
    if (n == 2 && (cmd == "TINORDER" || cmd == "TPREORDER" || cmd == "TPOSTORDER" || cmd == "TBFS")) {
        cout << "-> ";
        if (cmd == "TINORDER") inorder(c->tree);
        else if (cmd == "TPREORDER") preorder(c->tree);
        else if (cmd == "TPOSTORDER") postorder(c->tree);
        else bfs(c->tree);
        cout << endl;
        return false;
    }
    printError("unknown command or wrong arguments");
    return false;
}

void runPrint(Database* db, string* w, int n) {
    if (n != 2) { printError("usage: PRINT name"); return; }
    Container* c = findContainer(db, w[1]);
    if (!c) { printError("not found"); return; }
    if (c->type == 'M') { cout << "-> "; print(c->arr); }
    else if (c->type == 'F') { cout << "-> "; print(c->flist); }
    else if (c->type == 'L') { cout << "-> "; print(c->dlist); }
    else if (c->type == 'S') { cout << "-> "; print(c->stack); }
    else if (c->type == 'Q') { cout << "-> "; print(c->queue); }
    else if (c->type == 'T') { cout << "->" << endl; print(c->tree); }
}

bool execute(Database* db, const string& query) {
    string w[MAX_WORDS];
    int n = 0;
    istringstream ss(query);
    while (n < MAX_WORDS && ss >> w[n]) n++;
    if (n == 0) { printError("empty query"); return false; }

    string cmd = w[0];
    if (cmd == "PRINT") { runPrint(db, w, n); return false; }
    if (cmd == "ISMEMBER") return runTree(db, w, n);

    // первая буква команды - тип структуры
    switch (cmd[0]) {
        case 'M': return runArray(db, w, n);
        case 'F': return runFList(db, w, n);
        case 'L': return runDList(db, w, n);
        case 'S': return runStack(db, w, n);
        case 'Q': return runQueue(db, w, n);
        case 'T': return runTree(db, w, n);
    }
    printError("unknown command");
    return false;
}

int main(int argc, char* argv[]) {
    string filename, query;
    for (int i = 1; i < argc - 1; i++) {
        string arg = argv[i];
        if (arg == "--file") filename = argv[i + 1];
        else if (arg == "--query") query = argv[i + 1];
    }
    if (filename.empty() || query.empty()) {
        cout << "Usage: ./dbms --file <file> --query '<COMMAND args>'" << endl;
        return 1;
    }

    Database db{nullptr};
    loadFile(&db, filename);

    bool changed = false;
    try {
        changed = execute(&db, query);
    } catch (const exception& e) {   // stoi на нечисловом аргументе
        printError("number expected");
    }

    if (changed) saveFile(&db, filename);
    freeDatabase(&db);
    return 0;
}