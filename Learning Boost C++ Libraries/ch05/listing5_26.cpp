#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/assign.hpp>
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <string>

template<typename T>
struct TreeNode {
    T data;
    TreeNode<T> *left, *right;
    TreeNode<T> *prev, *next;

    TreeNode(const T& elem) : data(elem),
        left(nullptr), right(nullptr),
        prev(nullptr), next(nullptr)
    {}

    ~TreeNode()
    {
        delete left;
        delete right;
    }
};

template<typename T>
class BSTIterator :
    public boost::iterator_facade <BSTIterator<T>, T,
    boost::bidirectional_traversal_tag>
{
public:
    BSTIterator() : node_ptr(nullptr) {}
    explicit BSTIterator(TreeNode<T> *node) :
        node_ptr(node) {}
    BSTIterator(const BSTIterator<T>& that) :
        node_ptr(that.node_ptr) {}

private:
    TreeNode<T> *node_ptr;

    friend class boost::iterator_core_access;

    void increment()
    {
        node_ptr = node_ptr->next;
    }
    void decrement()
    {
        node_ptr = node_ptr->prev;
    }

    bool equal(const BSTIterator<T>& that) const
    {
        return node_ptr == that.node_ptr;
    }

    T& dereference() const
    {
        return node_ptr->data;
    }
};

template<typename T>
class BinarySearchTree
{
public:
    BinarySearchTree() : root(nullptr), first(nullptr),
        last(nullptr) {}
    ~BinarySearchTree()
    {
        delete root;
        delete last;
    }

    void insert(const T& elem)
    {
        if (!root) {
            root = new TreeNode<T>(elem);
            first = root;
            last = new TreeNode<T>(T());
            first->next = last;
            last->prev = first;
        } else {
            insert(elem, root);
        }
    }

    BSTIterator<T>begin()
    {
        return BSTIterator<T>(first);
    }
    BSTIterator<T>end()
    {
        return BSTIterator<T>(last);
    }

    BSTIterator<T>begin() const
    {
        return BSTIterator<const T>(first);
    }
    BSTIterator<T>end() const
    {
        return BSTIterator<const T>(last);
    }

private:
    TreeNode<T> *root;
    TreeNode<T> *first;
    TreeNode<T> *last;

    void insert(const T& elem, TreeNode<T> *node)
    {
        if (elem < node->data) {
            if (node->left) {
                insert(elem, node->left);
            } else {
                node->left = new TreeNode<T>(elem);
                node->left->prev = node->prev;
                node->prev = node->left;
                node->left->next = node;

                if (!node->left->prev) {
                    first = node->left;
                } else {
                    node->left->prev->next = node->left;
                }
            }
        } else if (node->data < elem) {
            if (node->right) {
                insert(elem, node->right);
            } else {
                node->right = new TreeNode<T>(elem);
                node->right->next = node->next;
                node->next = node->right;
                node->right->prev = node;

                if (node->right->next) {
                    node->right->next->prev = node->right;
                }
            }
        }
    }
};

int main()
{
    BinarySearchTree<std::string> bst;
    bst.insert("abc");
    bst.insert("def");
    bst.insert("xyz");

    for (auto& x : bst) {
        std::cout << x << '\n';
    }
}

