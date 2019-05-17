#include <iostream>


template<typename KeyType>
class Set {
public:
    Set (): root(nullptr) {}

    void insert (const KeyType k) {
        make_insert(root, k);
    }

    void erase (const KeyType k) {
        remove(root, k);
    }
    
private:
    struct Node {

        Node (const KeyType k): height(1), key(k), left(nullptr), right(nullptr), parent(nullptr) {} 

        size_t height;
        const KeyType key;
        Node* left;
        Node* right;
        Node* parent;
    };

    size_t height(Node* p) const {
	    return p ? p->height : 0;
    }

    int bfactor(Node* p) const {
        return height(p->right) - height(p->left);
    }

    void fixheight(Node* p) {
        auto hl = height(p->left);
        auto hr = height(p->right);
        p->height = max(hl, hr) + 1;
    }

    Node* rotateright(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);
        return q;
    }

    Node* rotateleft(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fixheight(q);
        fixheight(p);
        return p;
    }

    Node* balance(Node* p) {
        fixheight(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0)
                p->right = rotateright(p->right);
            return rotateleft(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0)
                p->left = rotateleft(p->left);
            return rotateright(p);
        }
        return p;
    }

    Node* make_insert (Node* p, KeyType k) {
        if (!p) 
            return new Node(k);
        if (k < p->key)
            p->left = insert(p->left, k);
        else if (p->key < k)
            p->right = insert(p->right, k);
        return balance(p);
    }

    Node* find_min(Node* p) const {
	    return p->left ? find_min(p->left) : p;
    }

    Node* remove_min(Node* p) {
        if (p->left == nullptr)
            return p->right;
        p->left = remove_min(p->left);
        return balance(p);
    }

    Node* remove(Node* p, KeyType k) {
        if (!p) 
            return nullptr;
        if (k < p->key)
            p->left = remove(p->left, k);
        else if (p->key < k)
            p->right = remove(p->right, k);	
        else {
            Node* q = p->left;
            Node* r = p->right;
            delete p;
            if (!r) 
                return q;
            Node* min = find_min(r);
            min->right = remove_min(r);
            min->left = q;
            return balance(min);
        }
        return balance(p);
    }

    Node* root;
};


int main() {
    
}
