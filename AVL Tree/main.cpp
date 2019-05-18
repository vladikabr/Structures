#include <iostream>


template<typename KeyType>
class Set {
public:
    Set (): root(nullptr) {}

    template<typename Iter>
    Set (const Iter &begin, const Iter &end): root(nullptr), sz(0) {
        while (begin++ != end)
            insert(*begin);
    }

    Set (const std::initializer_list<KeyType> &list): root(nullptr), sz(0) {
        for (auto key : list) 
            insert(key);
    }

    Set (const Set &other): root(nullptr), sz(other.size()) {
        root = make_copy(other.GetRoot());
    };

    ~Set () { // do not wor yet :)
        make_destruction(root);
    }

    void operator=(const Set &other) {
        root = make_copy(other.GetRoot());
        sz = other.size();
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return size() == 0;
    }

    void insert(const KeyType k) {
        root = make_insert(root, k);
    }

    void erase(const KeyType k) {
        root = make_erase(root, k);
    }

    KeyType rt() {
        return root->key;
    }

    
private:
    struct Node {

        Node (const KeyType k): height(1), key(k), left(nullptr), right(nullptr) {} 

        size_t height;
        const KeyType key;
        Node* left;
        Node* right;
    };

    size_t height(Node* p) const {
	    return p ? p->height : 0;
    }

    int bfactor(Node* p) const {
        return height(p->right) - height(p->left);
    }

    void fix_height(Node* p) {
        auto hl = height(p->left);
        auto hr = height(p->right);
        // p->height = std::max(hl, hr) + 1;
        p->height = (hl > hr ? hl : hr) + 1;
    }

    Node* rotate_right(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fix_height(p);
        fix_height(q);
        return q;
    }

    Node* rotate_left(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fix_height(q);
        fix_height(p);
        return p;
    }

    Node* balance(Node* p) {
        fix_height(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0)
                p->right = rotate_right(p->right);
            return rotate_left(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0)
                p->left = rotate_left(p->left);
            return rotate_right(p);
        }
        return p;
    }

    Node* make_insert(Node* p, KeyType k) {
        if (!p) {
            sz++;
            return new Node(k);
        }
        if (k < p->key)
            p->left = make_insert(p->left, k);
        else if (p->key < k)
            p->right = make_insert(p->right, k);
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

    Node* make_erase(Node* p, KeyType k) {
        if (!p) 
            return nullptr;
        if (k < p->key)
            p->left = make_erase(p->left, k);
        else if (p->key < k)
            p->right = make_erase(p->right, k);	
        else {
            sz--;
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

    Node* make_copy(Node* p) {
        if (!p)
            return nullptr;
        auto res = new Node(p->key);
        res->left = make_copy(p->left);
        res->right = make_copy(p->right);
        return res;
    }

    void make_destruction(Node* p) {
        if (p != nullptr) {
            make_destruction(p->left);
            make_destruction(p->right);
            delete(p);
            p = nullptr;
        }
    }

    Node* GetRoot() const {
        return root;
    }

    Node* root;
    size_t sz;
};


int main() {
    Set<int> a({1,2,3,4,5,6,7,8,9});
    std::cout << a.empty() << "\n";
    Set<int> b = a;
    std::cout << b.size() << "\n";
    for (int i = 1; i < 10; i++)
        b.erase(i);
    std::cout << a.size() << "\n";
    std::cout << b.empty() << "\n";
    
}
