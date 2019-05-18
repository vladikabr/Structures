#include <iostream>


template<typename KeyType>
class Set {
public:

    class iterator {
    public: 
        iterator (): it(nullptr), isEnd(true) {}

        

    private:
        Node* it;
        bool isEnd;
    };

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

    void fix_height(Node* p) {
        auto hl = height(p->left);
        auto hr = height(p->right);
        // p->height = std::max(hl, hr) + 1;
        p->height = (hl > hr ? hl : hr) + 1;
    }

    void fix_parent(Node* p) {
        if (p == nullptr)
            return;
        if (p->left)
            p->left->parent = p;
        if (p->right)
            p->right->parent = p;
        if (p == root) 
            p->parent = nullptr;
    }

    Node* rotate_right(Node* p) {
        Node* q = p->left;
        p->left = q->right;
        q->right = p;
        fix_parent(p);
        fix_parent(q);
        fix_height(p);
        fix_height(q);
        return q;
    }

    Node* rotate_left(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fix_parent(p);
        fix_parent(q);
        fix_height(q);
        fix_height(p);
        return p;
    }

    Node* balance(Node* p) {
        fix_height(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0) {
                p->right = rotate_right(p->right);
                fix_parent(p);
            }
            return rotate_left(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0) {
                p->left = rotate_left(p->left);
                fix_parent(p);
            }
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
        fix_parent(p);
        return balance(p);
    }

    Node* find_min(Node* p) const {
	    return p->left ? find_min(p->left) : p;
    }

    Node* remove_min(Node* p) {
        if (p->left == nullptr)
            return p->right;
        p->left = remove_min(p->left);
        fix_parent(p);
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
            if (!r) 
                return q;
            Node* min = find_min(r);
            min->right = remove_min(r);
            min->left = q;
            fix_parent(min);
            fix_parent(p->parent);
            delete p;
            return balance(min);
        }
        fix_parent(p);
        return balance(p);
    }

    Node* make_copy(Node* p) {
        if (!p)
            return nullptr;
        auto res = new Node(p->key);
        res->left = make_copy(p->left);
        res->right = make_copy(p->right);
        fix_parent(res);
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
    Set<int> s{1,2,3,4,5,6,7,8,9,10};
    std::cout << s.size() << " ";
    s.erase(1);
    s.erase(4);
    s.erase(10);
    s.erase(7);
    s.insert(1);
    s.insert(5);
    s.insert(2);

    s.insert(2);
    s.erase(2);
    s.erase(4);
    s.erase(3);
    s.erase(7);
    std::cout << s.size();
}
