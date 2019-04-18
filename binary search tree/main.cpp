#include <iostream>
#include <initializer_list>

template <class KeyType>
class BNS {
public:
    struct Node {

        Node(KeyType k) {
            key = k;
        }

        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
        KeyType key;
    };

    BNS(): root(nullptr) {};

    BNS(KeyType root_): root(new Node(root_)), number_of_elements(1) {};

    BNS(std::initializer_list<KeyType> lst) {
        for (auto nod : lst) {
            insert(new Node(nod));
            number_of_elements++;
        }
    }

    Node* insert(KeyType key) {
        Node* v = new Node(key);
        Node* prev = nullptr;
        Node* cur = root;
        while (cur != nullptr) {
            prev = cur;
            if (v->key < cur->key)
                cur = cur->left;
            else if (v->key > cur->key)
                cur = cur->right;
            else
                return nullptr;
        }
        v->parent = prev;
        if (prev == nullptr)
            root = v;
        else {
            if (v->key < prev->key)
                prev->left = v;
            else
                prev->right = v;
        }
        return v;
    }

    Node* GetKey(KeyType key) const {
        Node* res = root;
        while (res != nullptr) {
            if (key == res->key) {
                return res;
            }
            if (key < res->key) {
                res = res->left;
            } else {
                res = res->right;
            }
        }
        return res;
    }

    void erase(KeyType x) {
        Node* v = GetKey(x);
        if (v == nullptr)
            return;
        if (v->left == nullptr && v->right == nullptr) {
            if (v->key < v->parent->key)
                v->parent->left = nullptr;
            else
                v->parent->right = nullptr;
            delete(v);
            return;
        }
        if (v->left == nullptr) {
            if (v->key < v->parent->key)
                v->parent->left = v->right;
            else
                v->parent->right = v->right;
            v->right->parent = v->parent;
            delete(v);
            return;
        }
        if (v->right == nullptr) {
            if (v->key < v->parent->key)
                v->parent->left = v->left;
            else
                v->parent->right = v->left;
            v->left->parent = v->parent;
            delete(v);
            return;
        }
        Node* u = v->right;
        while (u->left)
            u = u->left;
        if (u->key < u->parent->key)
            u->parent->left = u->right;
        else
            u->parent->right = u->right;
        if (u->right)
            u->right->parent = u->parent;
        v->key = u->key;
    }

    Node* GetRoot() const {
        return root;
    }

    size_t size() const {
        return number_of_elements;
    }

private:
    Node* root;
    size_t number_of_elements = 0;
};

int main() {

    BNS<int> t(10);
    t.insert(15);
    t.insert(5);
    t.insert(20);
    t.erase(5);
    if (t.GetKey(5))
        std::cout << "True";


    
}