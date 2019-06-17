#pragma once

#include <initializer_list>

template<typename KeyType>
class Set {
public:

    struct Node { 
        Node (const KeyType k): height(1), key(k), left(nullptr), right(nullptr), parent(nullptr) {} 

        std::size_t height;
        const KeyType key;
        Node* left;
        Node* right;
        Node* parent;
    };

    class iterator {
    public: 
        iterator (): tree(nullptr), it(nullptr) {}

        iterator (const Node* node, const Set *s): tree(s), it(p) {}

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
            if (!(it->key < tree->max()->key || tree->max()->key < it->key)) {
                it = nullptr;
            } else {
                if (it->right != nullptr) {
                    it = tree->find_min(it->right);
                } else {
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
            if (it == nullptr) {
                it = tree->find_max(tree->GetRoot());
            } else if (!(it->key < tree->min()->key || tree->min()->key < it->key)) {
                it = nullptr;
            } else {
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

    Set (): root(nullptr), size(0), min(nullptr), max(nullptr) {}

    template<typename Iter>
    Set (Iter begin, Iter end): root(nullptr), size(0), min(nullptr), max(nullptr) {
        while (begin != end) {
            insert(*begin);
            begin++;
        }
    }

    Set (const std::initializer_list<KeyType> &list): root(nullptr), size(0), min(nullptr), max(nullptr) {
        for (auto key : list) 
            insert(key);
    }

    Set (const Set &other): root(nullptr), size(other.size()), min(nullptr), max(nullptr) {
        root = make_copy(other.GetRoot());
        fix_min_max();
        fix_height(root);
    };

    ~Set (void) { 
        make_destruction(root);
    }

    Set<KeyType>& operator=(const Set &other) {
        size = other.size();
        auto duplicate = make_copy(other.GetRoot());
        make_destruction(root);
        root = duplicate;
        fix_min_max();
        fix_height(root);
        return *this;
    }

    std::size_t size() const {
        return size;
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
        return min;
    }

    Node* max() const {
        return max;
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
        if (it != end() && !(*it < k || k < *it)) {
            return it;
        }
        return end();
    }

    
private:

    void fix_min_max() {
        min = find_min(root);
        max = find_max(root);
    }

    int height(Node* node) const {
	    return node ? node->height : 0;
    }

    int bfactor(Node* node) const {
        return height(node->right) - height(node->left);
    }

    void fix_height(Node* node) {
        if (node != nullptr) {
            auto hl = height(node->left);
            auto hr = height(node->right);
            node->height = (hl > hr ? hl : hr) + 1;
        }
    }

    void fix_parent(Node* node) {
        if (node == nullptr) {
            return;
        }
        if (node->left) {
            node->left->parent = node;
        }
        if (node->right) {
            node->right->parent = node;
        }
        if (node == root) {
            node->parent = nullptr; 
        }
    }

    Node* rotate_right(Node* node) {
        Node* left_child = node->left;
        node->left = left_child->right;
        left_child->right = node;
        fix_parent(node);
        fix_parent(left_child);
        fix_height(node);
        fix_height(left_child);
        return left_child;
    }

    Node* rotate_left(Node* node) {
        Node* right_child = node->right;
        node->right = right_child->left;
        right_child->left = node;
        fix_parent(right_child);
        fix_parent(node);
        fix_height(node);
        fix_height(right_child);
        return right_child;
    }

    Node* balance(Node* node) {
        fix_height(node);
        if (bfactor(node) == 2) {
            if (bfactor(node->right) < 0) {
                node->right = rotate_right(node->right);
                fix_parent(node);
                fix_height(node);
            }
            return rotate_left(node);
        }
        if (bfactor(node) == -2) {
            if (bfactor(node->left) > 0) {
                node->left = rotate_left(node->left);
                fix_parent(node);
                fix_height(node);
            }
            return rotate_right(node);
        }
        return node;
    }

    Node* make_insert(Node* node, KeyType k) {
        if (!node) {
            size++;
            return new Node(k);
        }
        if (k < node->key) {
            node->left = make_insert(node->left, k);
        } else if (node->key < k) {
            node->right = make_insert(node->right, k);
        }
        fix_parent(node);
        fix_height(node);
        return balance(node);
    }

    Node* find_min(Node* node) const {
        if (node == nullptr) {
            return nullptr;
        }
	    return node->left ? find_min(node->left) : node;
    }

    Node* find_max(Node* node) const {
        if (node == nullptr) {
            return nullptr;
        }
	    return node->right ? find_max(node->right) : node;
    }

    Node* remove_min(Node* node) {
        if (node->left == nullptr) {
            return node->right;
        }
        node->left = remove_min(node->left);
        fix_parent(node);
        fix_height(node);
        return balance(node);
    }

    Node* make_erase(Node* node, KeyType k) {
        if (!node) {
            return nullptr;
        }
        if (k < node->key) {
            node->left = make_erase(node->left, k);
        } else if (node->key < k) {
            node->right = make_erase(node->right, k);	
        } else {
            size--;
            Node* left_child = node->left;
            Node* right_child = node->right;
            if (!right_child) {
                delete(node);
                return left_child;
            }
            Node* min = find_min(right_child);
            min->right = remove_min(right_child);
            min->left = left_child;
            fix_parent(min);
            fix_height(min);
            delete(node);
            return balance(min);
        }
        fix_parent(node);
        fix_height(node);
        return balance(node);
    }

    Node* make_copy(Node* node) {
        if (!node) {
            return nullptr;
        }
        Node* res = new Node(node->key);
        res->left = make_copy(node->left);
        res->right = make_copy(node->right);
        fix_parent(res);
        fix_height(res);
        return res;
    }

    void make_destruction(Node* node) {
        if (node != nullptr) {
            make_destruction(node->left);
            make_destruction(node->right);
            delete(node);
        }
    }

    Node* make_lower_bound(Node* node, const KeyType &k) const {
        while (node != nullptr && node->key < k) {
            node = node->right;
        }
        if (node == nullptr) {
            return nullptr;
        }
        Node* left_search = make_lower_bound(node->left, k);
        if (left_search != nullptr) {
            return left_search;
        }
        return node;
    }

    Node* GetRoot() const {
        return root;
    }

    Node* root;
    std::size_t size;
    Node* min;
    Node* max;
};
