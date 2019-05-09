#include <iostream>
#include <initializer_list>

template <class KeyType>
class BST {
public:
    struct Node {

        Node(const KeyType k): key(k), elements_count(0), number_of_keys(1) {}

        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
        const KeyType key;
        size_t elements_count = 0;
        size_t number_of_keys = 0;
    };

    BST(): root(nullptr) {};

    BST(const KeyType root_): root(new Node(root_)), number_of_all_elements(1) {};

    BST(std::initializer_list<KeyType> lst) {
        for (auto nod : lst) {
            insert(new Node(nod));
        }
    }

    Node* insert(KeyType key) {
        number_of_all_elements++;
        Node* v = new Node(key);
        Node* prev = nullptr;
        Node* cur = root;
        while (cur != nullptr) {
            prev = cur;
            if (v->key < cur->key) {
                cur->elements_count++;
                cur = cur->left;
            } else if (v->key > cur->key) {
                cur->elements_count++;
                cur = cur->right;
            } else {
                cur->number_of_keys++;
                return cur;
            }
        }
        number_of_unique_elements++;
        v->parent = prev;
        if (prev == nullptr) {
            root = v;
        } else {
            if (v->key < prev->key) {
                prev->left = v;
            } else {
                prev->right = v;
            }
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

    size_t CountElements(const Node* v) const {
        if (v == nullptr)
            return 0;
        return v->elements_count + v->number_of_keys;
    }

    size_t CountLess(const KeyType k, const Node* v) const {
        if (v == nullptr) 
            return 0;
        if (k < v->key) {
            return CountLess(k, v->left);
        } else if (k > v->key) {
            return CountElements(v->left) + CountLess(k, v->right) + v->number_of_keys;
        } else {
            return CountElements(v->left);
        }
    }

    Node* GetRoot() const {
        return root;
    }

    size_t size() const {
        return number_of_all_elements;
    }

    size_t NumberOfUnique() const {
        return number_of_unique_elements;
    }

private:
    Node* root;
    size_t number_of_all_elements = 0;
    size_t number_of_unique_elements = 0;
};

int main() {
    
}