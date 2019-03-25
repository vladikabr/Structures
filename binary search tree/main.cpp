#include <iostream>

using namespace std;

struct Node {

    Node(int key) {
        k = key;
    }
    Node* p = nullptr;
    Node* l = nullptr;
    Node* r = nullptr;
    int k = 0;
};

Node* root;

Node* insert(Node* v) {
    Node* prev = nullptr;
    Node* cur = root;
    while (cur != nullptr) {
        prev = cur;
        if (v->k < cur->k)
            cur = cur->l;
        else if (v->k > cur->k)
            cur = cur->r;
        else
            return nullptr;
    }
    v->p = prev;
    if (prev == nullptr)
        root = v;
    else {
        if (v->k < prev->k)
            prev->l = v;
        else
            prev->r = v;
    }
    return v;
}

void delet(Node* v) {
    if (v->l == nullptr && v->r == nullptr) {
        if (v->k < v->p->k)
            v->p->l = nullptr;
        else
            v->p->r = nullptr;
        delete(v);
        return;
    }
    if (v->l == nullptr) {
        if (v->k < v->p->k)
            v->p->l = v->r;
        else
            v->p->r = v->r;
        v->r->p = v->p;
        delete(v);
        return;
    }
    if (v->r == nullptr) {
        if (v->k < v->p->k)
            v->p->l = v->l;
        else
            v->p->r = v->l;
        v->l->p = v->p;
        delete(v);
        return;
    }
    Node* u = v->r;
    while (u->l)
        u = u->l;
    if (u->k < u->p->k)
        u->p->l = u->r;
    else
        u->p->r = u->r;
    if (u->r)
        u->r->p = u->p;
    v->k = u->k;
}




int main() {


    return 0;
}


