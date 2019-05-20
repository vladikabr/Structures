#include <initializer_list>
#include <iostream>

template<typename KeyType>
class Set {
public:

    struct Node {

        Node (const KeyType k): height(1), rank(1), key(k), left(nullptr), right(nullptr), parent(nullptr) {} 

        size_t height;
        size_t rank;
        const KeyType key;
        Node* left;
        Node* right;
        Node* parent;
    };

    class iterator {
    public: 
        iterator (): it(nullptr) {}

        iterator (Node* p, Set &s): tree(s), it(p) {}
        
        iterator operator++(int) {
            if (it->key == tree.max())
                it = nullptr;
            else {
                if (it->right != nullptr)
                    it = tree.find_min(it->right);
                else {
                    while (it->parent->right == it)
                        it = it->parent;
                    it = it->parent;
                }
            }
            return *this;
        }

        iterator operator--(int) {
            if (it == nullptr) 
                it = tree.find_max(tree.GetRoot());
            else if (it->key == tree.min())
                it = nullptr;
            else {
                if (it->left) {
                    it = it->left;
                    while (it->right)
                        it = it->right;
                } else {
                    while (it->parent->left == it)
                        it = it->parent;
                    it = it->parent;
                }
            }
            return *this;
        }

        const KeyType operator*() const {
            if (it == nullptr) 
                return 0;
            return it->key;
        }

        // const KeyType operator->() const {
        //     if (it == nullptr) 
        //         return 0;
        //     return it->key;
        // }

        bool operator==(const iterator &other) const {
            return it == other.it;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }
        
    private:
        Set& tree;
        Node* it;
    };

    Set (): root(nullptr), sz(0), mn(0), mx(0) {}

    template<typename Iter>
    Set (const Iter &begin, const Iter &end): root(nullptr), sz(0), mn(0), mx(0) {
        while (begin++ != end)
            insert(*begin);
    }

    Set (const std::initializer_list<KeyType> &list): root(nullptr), sz(0), mn(0), mx(0) {
        for (auto key : list) 
            insert(key);
    }

    Set (const Set &other): root(nullptr), sz(other.size()), mn(other.min()), mx(other.max()) {
        root = make_copy(other.GetRoot());
    };

    ~Set () { // do not wor yet :)
        make_destruction(root);
    }

    void operator=(const Set &other) {
        root = make_copy(other.GetRoot());
        sz = other.size();
        mn = other.min();
        mx = other.max();
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return size() == 0;
    }

    void insert(const KeyType k) {
        root = make_insert(root, k);
        fix_min();
        fix_max();
    }

    void erase(const KeyType k) {
        root = make_erase(root, k);
        fix_min();
        fix_max();
    }

    KeyType min() const {
        return mn;
    }

    KeyType max() const {
        return mx;
    }

    iterator begin() {
        return iterator(find_min(GetRoot()), *this);
    }

    iterator end() {
        return iterator(nullptr, *this);
    }

    iterator lower_bound(const KeyType k) { // need const !!!
        return iterator(make_lower_bound(GetRoot(), k), *this);
    }

    iterator find(const KeyType k) { // need const !!!
        auto it = lower_bound(k);
        if (*it == k)
            return it;
        return end();
    }

    
private:

    void fix_min() {
        mn = find_min(GetRoot())->key;
    }

    void fix_max() {
        mx = find_max(GetRoot())->key;
    }

    size_t height(Node* p) const {
	    return p ? p->height : 0;
    }

    size_t rank(Node* p) const {
        return p ? p->rank : 0;
    }

    int bfactor(Node* p) const {
        return height(p->right) - height(p->left);
    }

    void fix_height_rank(Node* p) {
        auto hl = height(p->left);
        auto hr = height(p->right);
        auto rl = rank(p->left);
        auto rr = rank(p->right);
        p->height = (hl > hr ? hl : hr) + 1;
        p->rank = rl + rr + 1;
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
        fix_height_rank(p);
        fix_height_rank(q);
        return q;
    }

    Node* rotate_left(Node* q) {
        Node* p = q->right;
        q->right = p->left;
        p->left = q;
        fix_parent(p);
        fix_parent(q);
        fix_height_rank(q);
        fix_height_rank(p);
        return p;
    }

    Node* balance(Node* p) {
        fix_height_rank(p);
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

    Node* find_max(Node* p) const {
	    return p->right ? find_max(p->right) : p;
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

    Node* make_lower_bound(Node* p, const KeyType k) const {
        if (p == nullptr)
            return nullptr;
        while (p != nullptr && p->key < k) {
            p = p->right;
        }
        if (p == nullptr)
            return nullptr;
        Node* s = make_lower_bound(p->left, k);
        if (s != nullptr)
            return s;
        return p;
    }

    Node* GetRoot() const {
        return root;
    }

    Node* root;
    size_t sz;
    KeyType mn;
    KeyType mx;
};

int main() {
    Set<int> s{1,2,3,4,5,6,7,8,9,10};
    std::cout << s.min() << " " << s.max() << "\n";  
    s.erase(4);
    s.erase(5);   
    std::cout << s.min() << " " << s.max() << "\n";
    s.insert(11);
    s.insert(0);  
    std::cout << s.min() << " " << s.max() << "\n";
    s.erase(11);
    s.erase(10);   
    s.erase(0);
    s.erase(1);
    s.erase(2);
    std::cout << s.min() << " " << s.max() << "\n";
    auto it = s.begin();
    while (it != s.end()) {
        std::cout << *it << " ";
        it++;
    }
    std::cout << "\n";
    for (int i = 1; i <= 12; i++) {
        std::cout << i << " - ";
        auto iter = s.find(i);
        if (iter == s.end())
            std::cout << "end\n";
        else 
            std::cout << *iter << "\n";
    }
}