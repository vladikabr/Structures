#include <iostream>

using namespace std;

struct Node {

    Node(int key, int y_ = rand() % 100) {
        k = key;
        y = y_;
    }
    Node* p = nullptr;
    Node* l = nullptr;
    Node* r = nullptr;
    int k = 0;
    int y = 0;
};


Node* merge(Node* t1, Node* t2) { // all keys t1 < all keys t2
    if (t1 == nullptr)
        return t2;
    if (t2 == nullptr)
        return t1;
    if (t1->y < t2->y) {
        t1->r = merge(t1->r, t2);
        return t1;
    } else {
        t2->l = merge(t1, t2->l);
        return t2;
    }
}

pair<Node*, Node*> split(Node* t, int key) {
    if (t == nullptr)
        return {nullptr, nullptr};
    if (t->k < key) {
        auto sp = split(t->r, key);
        t->r = sp.first;
        return {t, sp.second};
    } else {
        auto sp = split(t->l, key);
        t->l = sp.second;
        return {sp.first, t};
    }
}

Node* insert(Node* t, Node* v) {
    if (t == nullptr)
        return v;
    auto sp = split(t, v->k);
    t = merge(sp.first, v);
    t = merge(t, sp.second);
    return t;
}

Node* delet(Node* t, int k) {
    auto sp1 = split(t, k);
    auto sp2 = split(sp1.second, k + 1);
    t = merge(sp1.first, sp2. second);
    return t;
}


int main() {
    
}

