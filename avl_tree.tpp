#include <algorithm>

#include "avl_tree.h"

template<typename T>
avl_tree<T>::avl_tree_node::avl_tree_node() noexcept { }

template<typename T>
avl_tree<T>::avl_tree_node::avl_tree_node(T const& value, avl_tree_node* parent) noexcept : value(value), parent(parent) { }

template<typename T>
ptrdiff_t avl_tree<T>::height(node_ptr node) noexcept
{
    return node ? node->height : 0;
}

template<typename T>
void avl_tree<T>::fix_height(node_ptr node) noexcept
{
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

template<typename T>
ptrdiff_t avl_tree<T>::difference(node_ptr node) noexcept
{
    return height(node->left) - height(node->right);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::rr_rotation(node_ptr parent) noexcept
{
    node_ptr node;
    node = parent->right;
    node->parent = parent->parent;
    parent->right = node->left;
    if (node->left) {
        node->left->parent = parent.get();
    }
    node->left = parent;
    parent->parent = node.get();
    fix_height(parent);
    fix_height(node);
    return node;
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::ll_rotation(node_ptr parent) noexcept
{
    node_ptr node;
    node = parent->left;
    node->parent = parent->parent;
    parent->left = node->right;
    if (node->right) {
        node->right->parent = parent.get();
    }
    node->right = parent;
    parent->parent = node.get();
    fix_height(parent);
    fix_height(node);
    return node;
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::lr_rotation(node_ptr parent) noexcept
{
    parent->left = rr_rotation(parent->left);
    return ll_rotation(parent);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::rl_rotation(node_ptr parent) noexcept
{
    parent->right = ll_rotation(parent->right);
    return rr_rotation(parent);
}

template<typename T>
void avl_tree<T>::balance(node_ptr& node) noexcept
{
    fix_height(node);
    auto diff = difference(node);
    if (diff > 1) {
        if (difference(node->left) > 0) {
            node = ll_rotation(node);
        }
        else {
            node = lr_rotation(node);
        }
    }
    else {
        if (diff < -1) {
            if (difference(node->right) > 0) {
                node = rl_rotation(node);
            }
            else {
                node = rr_rotation(node);
            }
        }
    }
}

template<typename T>
avl_tree<T>::avl_tree() noexcept { }

template<typename T>
avl_tree<T>::~avl_tree() = default;

template<typename T>
template<bool is_const_iterator>
avl_tree<T>::const_noconst_iterator<is_const_iterator>::const_noconst_iterator() = default;

template<typename T>
template<bool is_const_iterator>
avl_tree<T>::const_noconst_iterator<is_const_iterator>::const_noconst_iterator(avl_tree<T>::avl_tree_node const* node) noexcept
: ptr(node) { }

template<typename T>
template<bool is_const_iterator>
avl_tree<T>::const_noconst_iterator<is_const_iterator>::const_noconst_iterator(avl_tree<T>::const_noconst_iterator<false> const& other) noexcept : ptr(other.ptr) { }

template<typename T>
template<bool is_const_iterator>
template<bool any_const_noconst>
bool avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator==(avl_tree<T>::const_noconst_iterator<any_const_noconst> const& other) const noexcept {
    return ptr == other.ptr;
}

template<typename T>
template<bool is_const_iterator>
template<bool any_const_noconst>
bool avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator!=(avl_tree<T>::const_noconst_iterator<any_const_noconst> const& other) const noexcept {
    return !operator==(other);
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>& avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator++() {
    if (ptr->right) {
        ptr = ptr->right.get();
        while (ptr->left) {
            ptr = ptr->left.get();
        }
    } else {
        avl_tree_node const* node = ptr->parent;
        while (node && cmp(node->value, ptr->value) < 0) {
            ptr = node;
            node = ptr->parent;
        }
        ptr = node;
    }
    return *this;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>& avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator--() {
    if (ptr->left) {
        ptr = ptr->left.get();
        while (ptr->right) {
            ptr = ptr->right.get();
        }
    } else {
        avl_tree_node const* node = ptr->parent;
        while (node && cmp(node->value, ptr->value) > 0) {
            ptr = node;
            node = ptr->parent;
        }
        ptr = node;
    }
    return *this;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator> avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator++(int) {
    const const_noconst_iterator copy(*this);
    ++(*this);
    return copy;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator> avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator--(int) {
    const const_noconst_iterator copy(*this);
    --(*this);
    return copy;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>::reference avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator*() const noexcept
{
    return ptr->value.value();
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>::pointer avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator->() const noexcept
{
    return &ptr->value.value();
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>& avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator=(
        const avl_tree<T>::const_noconst_iterator<is_const_iterator>& other) noexcept
{
    ptr = other.ptr;
    return *this;
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::find(T const& value, avl_tree_node const* node) const
{
    if (node == nullptr) {
        return iterator(&fake_end_node);
    }
    if (cmp(value, node->value) == 0) {
        return iterator(node);
    }
    return cmp(value, node->value) < 0 ? find(value, node->left.get()) : find(value, node->right.get());
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::find(T const& value) const
{
    return find(value, root.get());
}

template<typename T>
bool avl_tree<T>::empty() const noexcept {
    return min == &fake_end_node;
}

template<typename T>
void avl_tree<T>::clear() noexcept {
    root.reset(&fake_end_node, cleaner);
    min = &fake_end_node;
    fake_end_node.left = nullptr;
    fake_end_node.right = nullptr;
    fake_end_node.parent = nullptr;
}

template<typename T>
std::pair<typename avl_tree<T>::iterator, bool> avl_tree<T>::insert(T const& value, avl_tree_node* parent, node_ptr& current)
{
    if (current == nullptr) {
        current.reset(new avl_tree_node(value, parent), cleaner);
        return {avl_tree::iterator(current.get()), true};
    }
    if (cmp(value, current->value) == 0) {
        return {avl_tree::iterator(current.get()), false};
    }
    auto tmp = insert(value, current.get(), cmp(value, current->value) < 0 ? current->left : current->right);
    balance(current);
    return tmp;
}

template<typename T>
std::pair<typename avl_tree<T>::iterator, bool> avl_tree<T>::insert(T const& value)
{
    auto tmp = insert(value, nullptr, root);
    if (cmp(value, min->value) < 0) {
        min = tmp.first.ptr;
    }
    return tmp;
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::minimum(avl_tree::node_ptr const& node) noexcept
{
    return node->left ? minimum(node->left) : node;
}

template<typename T>
void avl_tree<T>::remove_minimum(avl_tree<T>::node_ptr& node) noexcept {
    if (node->left == nullptr) {
        if (node->right) {
            node->right->parent = node->parent;
        }
        node = node->right;
        return;
    }
    remove_minimum(node->left);
    balance(node);
}

template<typename T>
void avl_tree<T>::remove(T const& value, avl_tree::node_ptr& node)
{
    if (node == nullptr) {
        return;
    }
    if (cmp(value, node->value) < 0) {
        remove(value, node->left);
    }
    else {
        if (cmp(value, node->value) > 0) {
            remove(value, node->right);
        }
        else {
            if (node->right == nullptr) {
                if (node->left) {
                    node->left->parent = node->parent;
                }
                node = node->left;
                return;
            }
            node_ptr left = node->left;
            node_ptr right = node->right;
            avl_tree_node* parent = node->parent;
            node = minimum(node->right);
            remove_minimum(right);
            node->left = left;
            if (left) {
                left->parent = node.get();
            }
            node->right = right;
            if (right) {
                right->parent = node.get();
            }
            node->parent = parent;
        }
    }
    balance(node);
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::erase(avl_tree<T>::const_iterator it) {
    avl_tree_node const* ptr = it.ptr;
    iterator new_it((++it).ptr);
    if (ptr == min) {
        min = new_it.ptr;
    }
    remove(ptr->value.value(), root);
    return new_it;
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::lower_bound(T const& value) const {
    avl_tree_node const* node = root.get();
    avl_tree_node const* successor = nullptr;
    while (node != nullptr) {
        if (cmp(node->value, value) >= 0) {
            successor = node;
            node = node->left.get();
        }
        else {
            node = node->right.get();
        }
    }
    return iterator(successor);
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::upper_bound(T const& value) const {
    avl_tree_node const* node = root.get();
    avl_tree_node const* successor = nullptr;
    while (node != nullptr) {
        if (cmp(node->value, value) > 0) {
            successor = node;
            node = node->left.get();
        }
        else {
            node = node->right.get();
        }
    }
    return iterator(successor);
}

template<typename T>
void avl_tree<T>::swap(avl_tree& other) noexcept {
    if (root.get() != &fake_end_node && other.root.get() != &other.fake_end_node) {
        root.swap(other.root);
    }
    else {
        if (root.get() == &fake_end_node) {
            if (other.root.get() != &other.fake_end_node) {
                root = other.root;
                other.root.reset(&other.fake_end_node, other.cleaner);
            }
        } else {
            other.root = root;
            root.reset(&fake_end_node, cleaner);
        }
    }
    if (min != &fake_end_node && other.min != &other.fake_end_node) {
        std::swap(min, other.min);
    }
    else {
        if (min == &fake_end_node) {
            if (other.min != &other.fake_end_node) {
                min = other.min;
                other.min = &other.fake_end_node;
            }
        } else {
            other.min = min;
            min = &fake_end_node;
        }
    }
    std::swap(fake_end_node, other.fake_end_node);
    auto fix_end_pointers = [](avl_tree& tree) {
        if (tree.fake_end_node.left) {
            tree.fake_end_node.left->parent = &tree.fake_end_node;
        }
        if (tree.fake_end_node.parent) {
            tree.fake_end_node.parent->right.reset(&tree.fake_end_node, tree.cleaner);
        }
    };
    fix_end_pointers(*this);
    fix_end_pointers(other);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::copy_subtree(avl_tree<T>::node_ptr const& node, avl_tree_node* parent, avl_tree const& owner) {
    if (node == nullptr) {
        return nullptr;
    }
    node_ptr ptr;
    if (node.get() != &owner.fake_end_node) {
        ptr.reset(new avl_tree<T>::avl_tree_node(node->value.value(), parent), cleaner);
    }
    else {
        ptr.reset(&fake_end_node, cleaner);
        ptr->parent = parent;
    }
    ptr->height = node->height;
    ptr->left = copy_subtree(node->left, ptr.get(), owner);
    ptr->right = copy_subtree(node->right, ptr.get(), owner);
    return ptr;
}

template<typename T>
avl_tree<T>::avl_tree(avl_tree const& other) : root(copy_subtree(other.root, nullptr, other)) {
    min = minimum(root).get();
}

template<typename T>
avl_tree<T>& avl_tree<T>::operator=(avl_tree const& other)
{
    avl_tree copy(other);
    swap(copy);
    return *this;
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::begin() noexcept {
    return iterator(min);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::cbegin() const noexcept {
    return const_iterator(min);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::begin() const noexcept {
    return cbegin();
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::end() noexcept {
    return iterator(&fake_end_node);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::cend() const noexcept {
    return const_iterator(&fake_end_node);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::end() const noexcept {
    return cend();
}

template<typename T>
typename avl_tree<T>::reverse_iterator avl_tree<T>::rbegin() noexcept {
    return avl_tree<T>::reverse_iterator(end());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::crbegin() const noexcept {
    return avl_tree<T>::const_reverse_iterator(end());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::rbegin() const noexcept {
    return crbegin();
}

template<typename T>
typename avl_tree<T>::reverse_iterator avl_tree<T>::rend() noexcept {
    return avl_tree<T>::reverse_iterator(begin());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::crend() const noexcept {
    return avl_tree<T>::const_reverse_iterator(begin());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::rend() const noexcept
{
    return crend();
}

template<typename T>
int avl_tree<T>::cmp(std::optional<T> const& lhs, std::optional<T> const& rhs)
{
    if (!lhs.has_value()) {
        return 1;
    }
    if (!rhs.has_value()) {
        return -1;
    }
    return lhs.value() > rhs.value() ? 1 : (lhs.value() < rhs.value() ? -1 : 0);
}

template<typename T>
void swap(avl_tree<T>& lhs, avl_tree<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

template<typename T>
avl_tree<T>::cleaner_t::cleaner_t(avl_tree_node* ptr) noexcept : fake_end(ptr) { }

template<typename T>
void avl_tree<T>::cleaner_t::operator()(avl_tree::avl_tree_node* ptr) noexcept
{
    if (ptr != fake_end) {
        delete ptr;
    }
}