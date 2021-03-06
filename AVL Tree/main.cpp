#include <initializer_list>

template<typename KeyType>
class Set {
public:

    struct Node { 
        Node (const KeyType k): height(1), key(k), left(nullptr), right(nullptr), parent(nullptr) {} 

        int height;
        const KeyType key;
        Node* left;
        Node* right;
        Node* parent;
    };

    class iterator {
    public: 
        iterator (): tree(nullptr), it(nullptr) {}

        iterator (const Node* p, const Set *s): tree(s), it(p) {}

        iterator (const iterator &other): tree(other.tree), it(other.it) {}

        iterator& operator=(const iterator other) {
            tree = other.tree;
            it = other.it;
            return *this;
        }
        
        iterator& operator++() {
            if (it == nullptr || tree == nullptr || tree->empty()) {
                it = nullptr;
                tree = nullptr;
                return *this;
            }
            if (!(it->key < tree->max()->key || tree->max()->key < it->key))
                it = nullptr;
            else {
                if (it->right != nullptr)
                    it = tree->find_min(it->right);
                else {
                    while (it->parent != nullptr && it->parent->right == it)
                        it = it->parent;
                    it = it->parent;
                }
            }
            return *this;
        }

        iterator operator++(int) {
            auto old = *this;
            ++(*this);
            return old;
        }

        iterator& operator--() {
            if (tree == nullptr || tree->empty()) {
                it = nullptr;
                return *this;
            }
            if (it == nullptr) 
                it = tree->find_max(tree->GetRoot());
            else if (!(it->key < tree->min()->key || tree->min()->key < it->key))
                it = nullptr;
            else {
                if (it->left) {
                    it = tree->find_max(it->left);
                } else {
                    while (it->parent != nullptr && it->parent->left == it)
                        it = it->parent;
                    it = it->parent;
                }
            }
            return *this;
        }

        iterator operator--(int) {
            auto old = *this;
            --(*this);
            return old;
        }

        const KeyType operator*() const {
            return it->key;
        }

        const KeyType* operator->() const {
            return &(it->key);
        }

        bool operator==(const iterator &other) const {
            return it == other.it && tree == other.tree;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

    private:
        const Set* tree;
        const Node* it;
    };

    Set (): root(nullptr), sz(0), mn(nullptr), mx(nullptr) {}

    template<typename Iter>
    Set (Iter begin, Iter end): root(nullptr), sz(0), mn(nullptr), mx(nullptr) {
        while (begin != end) {
            insert(*begin);
            begin++;
        }
    }

    Set (const std::initializer_list<KeyType> &list): root(nullptr), sz(0), mn(nullptr), mx(nullptr) {
        for (auto key : list) 
            insert(key);
    }

    Set (const Set &other): root(nullptr), sz(other.size()), mn(nullptr), mx(nullptr) {
        root = make_copy(other.GetRoot());
        fix_min_max();
        fix_height(root);
    };

    ~Set (void) { 
        make_destruction(root);
    }

    Set<KeyType>& operator=(const Set &other) {
        sz = other.size();
        auto duplicate = make_copy(other.GetRoot());
        make_destruction(root);
        root = duplicate;
        fix_min_max();
        fix_height(root);
        return *this;
    }

    std::size_t size() const {
        return sz;
    }

    bool empty() const {
        return size() == 0;
    }

    void insert(const KeyType k) {
        root = make_insert(root, k);
        fix_parent(root);
        fix_min_max();
    }

    void erase(const KeyType k) {
        root = make_erase(root, k);
        fix_parent(root);
        fix_min_max();
    }

    Node* min() const {
        return mn;
    }

    Node* max() const {
        return mx;
    }

    iterator begin() const {
        return iterator(find_min(root), this);
    }

    iterator end() const {
        return iterator(nullptr, this);
    }

    iterator lower_bound(const KeyType k) const {
        return iterator(make_lower_bound(root, k), this);
    }

    iterator find(const KeyType k) const {
        auto it = lower_bound(k);
        if (it != end() && !(*it < k || k < *it))
            return it;
        return end();
    }

    
private:

    void fix_min_max() {
        mn = find_min(root);
        mx = find_max(root);
    }

    int height(Node* p) const {
	    return p ? p->height : 0;
    }

    int bfactor(Node* p) const {
        return height(p->right) - height(p->left);
    }

    void fix_height(Node* p) {
        if (p != nullptr) {
            auto hl = height(p->left);
            auto hr = height(p->right);
            p->height = (hl > hr ? hl : hr) + 1;
        }
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
                fix_height(p);
            }
            return rotate_left(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0) {
                p->left = rotate_left(p->left);
                fix_parent(p);
                fix_height(p);
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
        fix_height(p);
        return balance(p);
    }

    Node* find_min(Node* p) const {
        if (p == nullptr)
            return nullptr;
	    return p->left ? find_min(p->left) : p;
    }

    Node* find_max(Node* p) const {
        if (p == nullptr)
            return nullptr;
	    return p->right ? find_max(p->right) : p;
    }

    Node* remove_min(Node* p) {
        if (p->left == nullptr) {
            return p->right;
        }
        p->left = remove_min(p->left);
        fix_parent(p);
        fix_height(p);
        return balance(p);
    }

    Node* make_erase(Node* p, KeyType k) {
        if (!p) 
            return nullptr;
        if (k < p->key) {
            p->left = make_erase(p->left, k);
        }
        else if (p->key < k) {
            p->right = make_erase(p->right, k);	
        }
        else {
            sz--;
            Node* q = p->left;
            Node* r = p->right;
            if (!r) {
                delete(p);
                return q;
            }
            Node* min = find_min(r);
            min->right = remove_min(r);
            min->left = q;
            fix_parent(min);
            fix_height(min);
            delete(p);
            return balance(min);
        }
        fix_parent(p);
        fix_height(p);
        return balance(p);
    }

    Node* make_copy(Node* p) {
        if (!p)
            return nullptr;
        Node* res = new Node(p->key);
        res->left = make_copy(p->left);
        res->right = make_copy(p->right);
        fix_parent(res);
        fix_height(res);
        return res;
    }

    void make_destruction(Node* p) {
        if (p != nullptr) {
            make_destruction(p->left);
            make_destruction(p->right);
            delete(p);
        }
    }

    Node* make_lower_bound(Node* p, const KeyType &k) const {
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
    std::size_t sz;
    Node* mn;
    Node* mx;
};

int main() {
    return 0;    
}   