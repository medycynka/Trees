//
// Created by PC on 16.11.2022.
//

#ifndef TREES_RBT_BUCKET_HPP
#define TREES_RBT_BUCKET_HPP

#pragma once

#include "../bst/iterators/iterator.hpp"
#include "../bst/iterators/reverse_iterator.hpp"
#include "../bst/iterators/const_iterator.hpp"
#include "../bst/iterators/const_reverse_iterator.hpp"
#include "rbt_bucket_node.hpp"
#include <initializer_list>
#include <vector>
#include <type_traits>

namespace tree::rbt {

    template <typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    class RBBucketTree {
    public:
        typedef Allocator                                               allocator;
        typedef T                                                       value_type;
        typedef T&                                                      reference;
        typedef const T&                                                const_reference;
        typedef T*                                                      pointer;
        typedef const T*                                                const_pointer;
        typedef tree::rbt::node_impl::RBBucketNode<T, Allocator>        node_type;
        typedef tree::rbt::node_impl::RBBucketNode<T, Allocator>*       node_pointer;
        typedef const tree::rbt::node_impl::RBBucketNode<T, Allocator>* const_node_pointer;
        typedef tree::rbt::node_impl::RBBucketNode<T, Allocator>&       node_reference;
        typedef const tree::rbt::node_impl::RBBucketNode<T, Allocator>& const_node_reference;
        typedef typename __gnu_cxx::__alloc_traits<Allocator>::template rebind<node_type>::other node_allocator;
        typedef RBBucketTree<T, Allocator>                              self_type;
        typedef tree::iterators::Iterator<tree::rbt::node_impl::RBBucketNode<T, Allocator>>             iterator;
        typedef tree::iterators::ConstIterator<tree::rbt::node_impl::RBBucketNode<T, Allocator>>        const_iterator;
        typedef tree::iterators::ReverseIterator<tree::rbt::node_impl::RBBucketNode<T, Allocator>>      reverse_iterator;
        typedef tree::iterators::ConstReverseIterator<tree::rbt::node_impl::RBBucketNode<T, Allocator>> const_reverse_iterator;

    private:
        /**
            * Helper functions
        */
        size_t calculate_size_(node_pointer);
        void   display_(node_pointer, std::size_t);
        void   left_rotation_(node_pointer);
        void   right_rotation_(node_pointer);
        void   insert_fix_up_(node_pointer);
        void   merge_(node_pointer);
        void   split_(node_pointer);
        void   delete_fix_up_(node_pointer);
        void   copy_(node_pointer);
        void   chop_(node_pointer);

    public:
        /**
        * Constructors
        */
        constexpr RBBucketTree()                                                               : size_{ 0 }, root_{ nullptr } {};
        constexpr explicit RBBucketTree(node_allocator &alloc)                                 : size_{ 0 }, root_{ nullptr }, al{alloc} {};
        constexpr RBBucketTree(const RBBucketTree<T, Allocator>& t)                            : size_{ 0 }, root_{ nullptr }, al{t.al.select_on_container_copy_construction()} { copy_(t.root_); };
        constexpr RBBucketTree(const RBBucketTree<T, Allocator>& t, node_allocator &alloc)     : size_{ 0 }, root_{ nullptr }, al{alloc} { copy_(t.root_); };
        constexpr RBBucketTree(RBBucketTree<T, Allocator>&& t) noexcept                        : size_{ 0 }, root_{ nullptr }, al{t.al} { copy_(t.root_); t.clear(); };
        constexpr RBBucketTree(RBBucketTree<T, Allocator>&& t, node_allocator &alloc) noexcept : size_{ 0 }, root_{ nullptr }, al{alloc} { copy_(t.root_); t.clear(); };
        constexpr RBBucketTree(std::initializer_list<T> init)                                  : size_{ 0 }, root_{ nullptr } { for (auto& e : init) { insert(e); } };
        constexpr explicit RBBucketTree(std::size_t initSize)                                  : size_{ 0 }, root_{ nullptr } { if constexpr (std::is_arithmetic_v<T>){ for (auto i = 0; i < initSize; ++i) { insert(T{i}); } } };
        template<typename InputIt>
        constexpr RBBucketTree(InputIt first, InputIt last)                                    : size_{ 0 }, root_{ nullptr } {  while (first != last) {  insert(*first); first++; } };

        /**
            * Utility functions
        */
        constexpr node_pointer                    get_root()       const { return root_; };
        constexpr iterator                        root()                 { return iterator(root_); };
        constexpr const_iterator                  root()           const { return const_iterator(root_); };
        constexpr const_iterator                  croot()          const { return const_iterator(root_); };
        constexpr allocator                       get_allocator()  const { return root_->get_allocator(); };
        constexpr node_allocator                  get_node_alloc() const { return root_->get_allocator(); };
        constexpr node_allocator                  get_node_alloc()       { return al; };
        constexpr size_t                          size()                 { return size_; };
        [[nodiscard]] constexpr bool              empty()          const { return (root_ == nullptr && size_ == 0); };
        constexpr void                            display()              { display_(root_, 0); };
        constexpr void                            clear()                { size_ = 0; chop_(root_); root_ = nullptr; };
        constexpr node_pointer                    max_it()         const { return (empty() ? nullptr : root_->max_node()); };
        constexpr iterator                        max_iter()             { return iterator(max_it()); };
        constexpr node_pointer                    min_it()         const { return (empty() ? nullptr : root_->min_node()); };
        constexpr iterator                        min_iter()             { return iterator(min_it()); };
        iterator                                  insert(const_reference);
        template<typename InputIt>
        void                                      insert(InputIt first, InputIt last);
        std::pair<iterator, bool>                 insert_unique(const_reference);
        iterator                                  find(const_reference);
        const_iterator                            find(const_reference) const;
        std::size_t                               count(const_reference);
        iterator                                  iterator_to(const_reference);
        const_iterator                            iterator_to(const_reference) const;
        node_pointer                              node_find(const_reference);
        size_t                                    black_height();
        void                                      replace(const_reference, const_reference);
        bool                                      remove(const_reference);
        iterator                                  erase(const_iterator pos);
        iterator                                  erase(iterator pos);
        iterator                                  erase(iterator first, iterator last);
        std::size_t                               erase(const_reference);
        void                                      swap(reference) noexcept;
        void                                      copy_from(const RBBucketTree<T, Allocator>& src);
        void                                      copy_from(RBBucketTree<T, Allocator>&& src);
        std::pair<iterator, iterator>             bounded_range(const_reference from, const_reference to);
        std::pair<const_iterator, const_iterator> bounded_range(const_reference from, const_reference to) const;
        std::pair<iterator, iterator>             equal_range(const_reference val);
        std::pair<const_iterator, const_iterator> equal_range(const_reference val) const;
        iterator                                  lower_bound(const_reference val);
        const_iterator                            lower_bound(const_reference val) const;
        iterator                                  upper_bound(const_reference val);
        const_iterator                            upper_bound(const_reference val) const;

        /*
        * Ostream overloading
        */
        friend std::ostream& operator<<(std::ostream& ofs, const RBBucketTree<T, Allocator>& tree) {
            for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
                ofs << *it << ", ";
            }

            ofs << "\n";

            return ofs;
        }

        friend std::ostream& operator<<(std::ostream& ofs, const RBBucketTree<T, Allocator>* tree) {
            for (auto it = tree->cbegin(); it != tree->cend(); ++it) {
                ofs << *it << ", ";
            }

            ofs << "\n";

            return ofs;
        }

        /**
            * Operators
        */
        void                        operator+ (const_reference in)                           { merge_(in.root_); };
        void                        operator- (const_reference in)                           { split_(in.root_); };
        RBBucketTree<T, Allocator>& operator+=(const_reference x)                            { insert(x); return *this; };
        RBBucketTree<T, Allocator>* operator-=(const_reference x)                            { remove(x); return *this; };
        value_type                  operator[](const size_t& idx);
        value_type                  operator[](const size_t& idx) const;
        RBBucketTree<T, Allocator>& operator= (const RBBucketTree<T, Allocator>&);
        RBBucketTree<T, Allocator>& operator= (RBBucketTree<T, Allocator>&&) noexcept;
        RBBucketTree<T, Allocator>& operator= (std::initializer_list<T> ilist);
        bool                        operator==(const RBBucketTree<T, Allocator>&) const;
        bool                        operator!=(const RBBucketTree<T, Allocator>& tree) const { return !(*this == tree); };
        bool                        operator< (const RBBucketTree<T, Allocator>& tree) const;
        bool                        operator> (const RBBucketTree<T, Allocator>& tree) const { return tree < *this; };
        bool                        operator<=(const RBBucketTree<T, Allocator>& tree) const { return !(*this > tree); };
        bool                        operator>=(const RBBucketTree<T, Allocator>& tree) const { return !(*this < tree); };
        constexpr explicit          operator bool()                                    const { return !empty(); };

        /**
            * Iterators
            * Begin/End functions
        */
        iterator               begin()         noexcept { return iterator(min_it()); };
        iterator               end()           noexcept { return iterator(); };
        const_iterator         begin()   const noexcept { return const_iterator(min_it()); };
        const_iterator         end()     const noexcept { return const_iterator(); };
        const_iterator         cbegin()  const noexcept { return const_iterator(min_it()); };
        const_iterator         cend()    const noexcept { return const_iterator(); };
        reverse_iterator       rbegin()        noexcept { return reverse_iterator(max_it()); };
        reverse_iterator       rend()          noexcept { return reverse_iterator(); };
        const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(max_it()); };
        const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(); };
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(max_it()); };
        const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(); };

    private:
        size_t         size_;
        node_pointer   root_;
        node_allocator al{};
    };

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::copy_(node_pointer in_node) {
        if (in_node) {
            insert(in_node->key);
            copy_(in_node->left);
            copy_(in_node->right);
        }
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::chop_(node_pointer in_node) {
        if (in_node) {
            chop_(in_node->left);
            chop_(in_node->right);

            al.destroy(in_node);
            al.deallocate(in_node, 1);
        }
    }

    template <typename T, typename Allocator>
    inline RBBucketTree<T, Allocator> &RBBucketTree<T, Allocator>::operator=(const RBBucketTree<T, Allocator>& tree) {
        if (this != &tree) {
            chop_(root_);
            root_ = nullptr;
            size_ = 0;
            copy_(tree.root_);
            al = tree.al;
        }

        return *this;
    }

    template <typename T, typename Allocator>
    inline RBBucketTree<T, Allocator> &RBBucketTree<T, Allocator>::operator=(RBBucketTree<T, Allocator>&& tree) noexcept {
        chop_(root_);
        root_ = nullptr;
        size_ = 0;
        copy_(tree.root_);
        al = tree.al;
        tree.clear();

        return *this;
    }

    template <typename T, typename Allocator>
    RBBucketTree<T, Allocator> &RBBucketTree<T, Allocator>::operator=(std::initializer_list<T> ilist) {
        chop_(root_);
        size_ = 0;

        for(auto &e : ilist){
            insert(e);
        }

        return *this;
    }

    template <typename T, typename Allocator>
    inline bool RBBucketTree<T, Allocator>::operator==(const RBBucketTree<T, Allocator>& tree) const {
        if (size_ != tree.size_ || root_ != tree.root_) return false;
        else {
            auto it = begin();
            auto tree_it = tree.begin();

            while (it != end() && tree_it != tree.end()) {
                if (*it != *tree_it) return false;
                else {
                    ++it;
                    ++tree_it;
                }
            }

            return true;
        }
    }

    template <typename T, typename Allocator>
    inline bool RBBucketTree<T, Allocator>::operator<(const RBBucketTree<T, Allocator>& tree) const {
        if (size_ == tree.size_ && root_ < tree.root_) {
            auto it = begin();
            auto tree_it = tree.begin();

            while (it != end() && tree_it != tree.end()) {
                if (*it >= *tree_it) return false;
                else {
                    ++it;
                    ++tree_it;
                }
            }

            return true;
        }
        else {
            return (size_ < tree.size_&& root_ < tree.root_);
        }
    }

    template <typename T, typename Allocator>
    inline T RBBucketTree<T, Allocator>::operator[](const size_t& idx) {
        if (idx < 0 || idx >= size_) throw tree::exception::TreeIndexOutOfBoundException();
        else {
            if (idx == 0) return min_it()->key;

            auto count = 1;
            auto* n = min_it()->node_Successor();

            while (count++ != idx) n = n->node_Successor();

            return n->key;
        }
    }

    template <typename T, typename Allocator>
    inline T RBBucketTree<T, Allocator>::operator[](const size_t& idx) const {
        if (idx < 0 || idx >= size_) throw tree::exception::TreeIndexOutOfBoundException();
        else {
            if (idx == 0) return min_it()->key;

            auto count = 1;
            auto* n = min_it()->node_Successor();

            while (count++ != idx) n = n->node_Successor();

            return n->key;
        }
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::insert(const_reference input) {
        node_pointer tmp_node;
        auto* new_node = al.allocate(1);
        al.construct(new_node, input);
        auto* root_copy = root_;
        tmp_node = nullptr;

        if (root_ == nullptr) {
            root_ = new_node;
            new_node->father = nullptr;
        } else {
            while (root_copy != nullptr) {
                tmp_node = root_copy;

                if (root_copy->key > new_node->key) root_copy = root_copy->left;
                else if (root_copy->key < new_node->key) root_copy = root_copy->right;
                else {
                    al.destroy(new_node);
                    al.deallocate(new_node, 1);
                    root_copy->add_to_bucket();
                    size_++;

                    return iterator(root_copy);
                }
            }

            new_node->father = tmp_node;

            if (tmp_node->key < new_node->key) tmp_node->right = new_node;
            else if (tmp_node->key > new_node->key) tmp_node->left = new_node;
            else {
                tmp_node->add_to_bucket();
                size_++;

                return iterator(tmp_node);
            }
        }

        size_++;
        insert_fix_up_(new_node);

        return iterator_to(input);
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::insert_fix_up_(node_pointer new_node) {
        auto* tmp_node = new_node;

        while (tmp_node != root_ && tmp_node->father->color == tree::rbt::node_impl::red) {
            if (tmp_node->father == tmp_node->father->father->left) {
                auto* right_uncle = tmp_node->father->father->right;

                if ((right_uncle != nullptr) && (right_uncle->color == tree::rbt::node_impl::red)) {
                    tmp_node->father->color = tree::rbt::node_impl::black;
                    right_uncle->color = tree::rbt::node_impl::black;
                    tmp_node->father->father->color = tree::rbt::node_impl::red;
                    tmp_node = tmp_node->father->father;
                }
                else {
                    if (tmp_node->father->right == tmp_node) {
                        tmp_node = tmp_node->father;
                        left_rotation_(tmp_node);
                    }

                    tmp_node->father->color = tree::rbt::node_impl::black;
                    tmp_node->father->father->color = tree::rbt::node_impl::red;
                    right_rotation_(tmp_node->father->father);
                }
            }
            else {
                auto* left_uncle = tmp_node->father->father->left;

                if ((left_uncle != nullptr) && (left_uncle->color == tree::rbt::node_impl::red)) {
                    tmp_node->father->color = tree::rbt::node_impl::black;
                    left_uncle->color = tree::rbt::node_impl::black;
                    tmp_node->father->father->color = tree::rbt::node_impl::red;
                    tmp_node = tmp_node->father->father;
                }
                else {
                    if (tmp_node->father->left == tmp_node) {
                        tmp_node = tmp_node->father;
                        right_rotation_(tmp_node);
                    }

                    tmp_node->father->color = tree::rbt::node_impl::black;
                    tmp_node->father->father->color = tree::rbt::node_impl::red;
                    left_rotation_(tmp_node->father->father);
                }
            }
        }

        root_->color = tree::rbt::node_impl::black;
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::right_rotation_(node_pointer in_node) {
        if (in_node->left == nullptr) return;
        else {
            auto* left_child = in_node->left;
            auto* left_child_right = left_child->right;
            auto* father = in_node->father;

            if (father == nullptr) {
                left_child->father = nullptr;
                root_ = left_child;
            }
            else {
                left_child->father = father;

                if (father->left == in_node) father->left = left_child;
                if (father->right == in_node) father->right = left_child;
            }

            left_child->right = in_node;
            in_node->father = left_child;
            in_node->left = left_child_right;

            if (left_child_right != nullptr) left_child_right->father = in_node;
        }
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::left_rotation_(node_pointer in_node) {
        if (in_node->right == nullptr) return;
        else {
            auto* right_child = in_node->right;
            auto* right_child_left = right_child->left;
            auto* father = in_node->father;

            if (father == nullptr) {
                right_child->father = nullptr;
                root_ = right_child;
            }
            else {
                right_child->father = father;

                if (father->left == in_node) father->left = right_child;
                if (father->right == in_node) father->right = right_child;
            }

            right_child->left = in_node;
            in_node->father = right_child;
            in_node->right = right_child_left;

            if (right_child_left != nullptr) right_child_left->father = in_node;
        }
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::find(const_reference val) {
        auto* tmp_node = root_;

        while (tmp_node != nullptr) {
            if (tmp_node->key == val) return iterator(tmp_node);
            if (val > tmp_node->key) tmp_node = tmp_node->right;
            else if (val < tmp_node->key) tmp_node = tmp_node->left;
        }

        return end();
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::const_iterator RBBucketTree<T, Allocator>::find(const_reference val) const
    {
        auto* tmp_node = root_;

        while (tmp_node != nullptr) {
            if (tmp_node->key == val) return const_iterator(tmp_node);
            if (val > tmp_node->key) tmp_node = tmp_node->right;
            else if (val < tmp_node->key) tmp_node = tmp_node->left;
        }

        return cend();
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::iterator_to(const_reference val) {
        return iterator(node_find(val));
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::const_iterator RBBucketTree<T, Allocator>::iterator_to(const_reference val) const {
        return const_iterator(node_find(val));
    }

    template <typename T, typename Allocator>
    typename RBBucketTree<T, Allocator>::node_pointer RBBucketTree<T, Allocator>::node_find(const_reference in_node) {
        auto* tmp_node = root_;

        while (tmp_node != nullptr) {
            if (tmp_node->key == in_node) return tmp_node;
            if (in_node > tmp_node->key) tmp_node = tmp_node->right;
            else if (in_node < tmp_node->key) tmp_node = tmp_node->left;
        }

        return nullptr;
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::merge_(node_pointer in_node) {
        if (in_node != nullptr) {
            if (in_node->left) merge_(in_node->left);
            if (in_node->right) merge_(in_node->right);

            insert(in_node->key);
        }
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::split_(node_pointer in_node) {
        if (in_node != nullptr) {
            if (in_node->left) split_(in_node->left);
            if (in_node->right) split_(in_node->right);

            remove(in_node->key);
        }
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline size_t RBBucketTree<T, Allocator>::black_height() {
        auto* tmp_node = root_;
        auto num = 0;

        while (tmp_node != nullptr) {
            if (tmp_node->color == tree::rbt::node_impl::black) num++;

            tmp_node = tmp_node->left;
        }

        return num;
    }

    template <typename T, typename Allocator>
    inline size_t RBBucketTree<T, Allocator>::calculate_size_(node_pointer in_node) {
        if (in_node == nullptr) return 0;
        else {
            auto ls = calculate_size_(in_node->left);
            auto rs = calculate_size_(in_node->right);

            return (ls + rs + 1);
        }
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::display_(node_pointer in_node, size_t level) {
        if (in_node == nullptr) return;

        std::cout << "level: " << level << std::endl;
        std::cout << in_node;

        display_(in_node->left, level + 1);
        display_(in_node->right, level + 1);
    }

    template <typename T, typename Allocator>
    inline bool RBBucketTree<T, Allocator>::remove(const_reference val) {
        if (root_ == nullptr) {
            std::cout << "\nEmpty RBBucketTree.";

            return false;
        }

        auto* tmp_node = root_;
        auto found = 0;

        while (tmp_node != nullptr && found == 0) {
            if (tmp_node->key == val) found = 1;
            if (found == 0) {
                if (tmp_node->key < val) tmp_node = tmp_node->right;
                else tmp_node = tmp_node->left;
            }
        }

        if (found == 0) {
            return false;
        }
        else {
            if (tmp_node->bucket_size > 1){
                tmp_node->remove_from_bucket();
            } else {
                node_pointer successor;
                node_pointer grand_child;

                if (tmp_node->left == nullptr || tmp_node->right == nullptr) successor = tmp_node;
                else successor = tmp_node->node_Successor();

                if (successor->left != nullptr) grand_child = successor->left;
                else {
                    if (successor->right != nullptr) grand_child = successor->right;
                    else grand_child = nullptr;
                }

                if (grand_child != nullptr) grand_child->father = successor->father;

                if (successor->father == nullptr) root_ = grand_child;
                else {
                    if (successor == successor->father->left) successor->father->left = grand_child;
                    else successor->father->right = grand_child;
                }

                if (successor != tmp_node) {
                    tmp_node->color = successor->color;
                    tmp_node->key = successor->key;
                }

                if (successor->color == tree::rbt::node_impl::black) delete_fix_up_(grand_child);
            }

            size_--;

            return true;
        }
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::delete_fix_up_(node_pointer in_node) {
        if (in_node != nullptr) {
            node_pointer successor;

            while (in_node != root_ && in_node->color == tree::rbt::node_impl::black) {
                if (in_node->father->left == in_node) {
                    successor = in_node->father->right;

                    if (successor->color == tree::rbt::node_impl::red) {
                        successor->color = tree::rbt::node_impl::black;
                        in_node->father->color = tree::rbt::node_impl::red;
                        left_rotation_(in_node->father);
                        successor = in_node->father->right;
                    }
                    if (successor->right->color == tree::rbt::node_impl::black && successor->left->color == tree::rbt::node_impl::black) {
                        successor->color = tree::rbt::node_impl::red;
                        in_node = in_node->father;
                    }
                    else {
                        if (successor->right->color == tree::rbt::node_impl::black) {
                            successor->left->color = tree::rbt::node_impl::black;
                            successor->color = tree::rbt::node_impl::red;
                            right_rotation_(successor);
                            successor = in_node->father->right;
                        }

                        successor->color = in_node->father->color;
                        in_node->father->color = tree::rbt::node_impl::black;
                        successor->right->color = tree::rbt::node_impl::black;
                        left_rotation_(in_node->father);
                        in_node = root_;
                    }
                }
                else {
                    successor = in_node->father->left;

                    if (successor->color == tree::rbt::node_impl::red) {
                        successor->color = tree::rbt::node_impl::red;
                        in_node->father->color = tree::rbt::node_impl::red;
                        right_rotation_(in_node->father);
                        successor = in_node->father->left;
                    }
                    if (successor->left->color == tree::rbt::node_impl::black && successor->right->color == tree::rbt::node_impl::black) {
                        successor->color = tree::rbt::node_impl::red;
                        in_node = in_node->father;
                    }
                    else {
                        if (successor->left->color == tree::rbt::node_impl::black) {
                            successor->right->color = tree::rbt::node_impl::black;
                            successor->color = tree::rbt::node_impl::red;
                            left_rotation_(successor);
                            successor = in_node->father->left;
                        }

                        successor->color = in_node->father->color;
                        in_node->father->color = tree::rbt::node_impl::black;
                        successor->left->color = tree::rbt::node_impl::black;
                        right_rotation_(in_node->father);
                        in_node = root_;
                    }
                }

                in_node->color = tree::rbt::node_impl::black;
                root_->color = tree::rbt::node_impl::black;
            }
        }
    }

    template <typename T, typename Allocator>
    inline std::pair<typename RBBucketTree<T, Allocator>::iterator, typename RBBucketTree<T, Allocator>::iterator> RBBucketTree<T, Allocator>::bounded_range(const_reference from, const_reference to) {
        if (from <= to) {
            iterator from_it;
            iterator to_it;

            for (auto it = begin(); it != end(); ++it) {
                if (!from_it && *it == from) {
                    from_it = it;
                }
                if (!to_it && *it == to) {
                    to_it = it;
                }
                if (from_it && to_it) {
                    break;
                }
            }

            return { from_it ? from_it : end(), to_it ? to_it : end() };
        }
        else {
            return { end(), end() };
        }
    }

    template <typename T, typename Allocator>
    inline std::pair<typename RBBucketTree<T, Allocator>::const_iterator, typename RBBucketTree<T, Allocator>::const_iterator> RBBucketTree<T, Allocator>::bounded_range(const_reference from, const_reference to) const {
        if (from <= to) {
            const_iterator from_it;
            const_iterator to_it;

            for (const auto it = cbegin(); it != cend(); ++it) {
                if (!from_it && *it == from) {
                    from_it = it;
                }
                if (!to_it && *it == to) {
                    to_it = it;
                }
                if (from_it && to_it) {
                    break;
                }
            }

            return { from_it ? from_it : cend(), to_it ? to_it : cend() };
        }
        else {
            return { cend(), cend() };
        }
    }

    template <typename T, typename Allocator>
    inline std::pair<typename RBBucketTree<T, Allocator>::iterator, typename RBBucketTree<T, Allocator>::iterator> RBBucketTree<T, Allocator>::equal_range(const_reference val) {
        return { lower_bound(val), upper_bound(val) };
    }

    template <typename T, typename Allocator>
    inline std::pair<typename RBBucketTree<T, Allocator>::const_iterator, typename RBBucketTree<T, Allocator>::const_iterator> RBBucketTree<T, Allocator>::equal_range(const_reference val) const {
        return { lower_bound(val), upper_bound(val) };
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::lower_bound(const_reference val) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it >= val) {
                return it;
            }
        }

        return end();
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::const_iterator RBBucketTree<T, Allocator>::lower_bound(const_reference val) const {
        for (const auto it = cbegin(); it != cend(); ++it) {
            if (*it >= val) {
                return it;
            }
        }

        return cend();
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::upper_bound(const_reference val) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it > val) {
                return it;
            }
        }

        return end();
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::const_iterator RBBucketTree<T, Allocator>::upper_bound(const_reference val) const {
        for (const auto it = cbegin(); it != cend(); ++it) {
            if (*it > val) {
                return it;
            }
        }

        return cend();
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::erase(const_iterator position) {
        auto ret = iterator(position.getIter());
        ++ret;
        remove(*position);

        return ret;
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::erase(iterator position) {
        auto ret = position;
        ++ret;
        remove(*position);

        return ret;
    }

    template <typename T, typename Allocator>
    inline typename RBBucketTree<T, Allocator>::iterator RBBucketTree<T, Allocator>::erase(iterator first, iterator last) {
        auto ret = last;

        if (ret != end()) {
            ++ret;
        }

        std::vector<T> remover;
        for (auto it = first; it != last; ++it) {
            remover.push_back(*it);
        }

        for (auto& e : remover) {
            remove(e);
        }

        return ret;
    }

    template <typename T, typename Allocator>
    inline std::size_t RBBucketTree<T, Allocator>::erase(const_reference key) {
        std::size_t count = 0;

        while (find(key)) {
            remove(key);
            count++;
        }

        return count;
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::swap(reference other) noexcept {
        std::vector<T> swaper(other.begin(), other.end());

        other.clear();
        other.insert(begin(), end());
        clear();
        insert(swaper.begin(), swaper.end());
    }

    template <typename T, typename Allocator>
    template <typename InputIt>
    inline void RBBucketTree<T, Allocator>::insert(InputIt first, InputIt last) {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    template <typename T, typename Allocator>
    inline std::size_t RBBucketTree<T, Allocator>::count(const_reference key) {
        std::size_t count = 0;

        for (auto it = begin(); it != end(); ++it) {
            if (*it == key) {
                count++;
            }
        }

        return count;
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::copy_from(const RBBucketTree<T, Allocator>& src) {
        *this = src;
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::copy_from(RBBucketTree<T, Allocator>&& src) {
        *this = std::move(src);
    }

    template <typename T, typename Allocator>
    inline std::pair<typename RBBucketTree<T, Allocator>::iterator, bool> RBBucketTree<T, Allocator>::insert_unique(const_reference val) {
        auto check = size();
        insert(val);

        if (check == size()) {
            return { iterator_to(val), false };
        }
        else {
            auto* ret = node_find(val);

            if(ret != nullptr){
                return { iterator(ret), true };
            } else {
                throw tree::exception::TreeInsertionFailedException();
            }
        }
    }

    template <typename T, typename Allocator>
    inline void RBBucketTree<T, Allocator>::replace(const_reference replace_this, const_reference with_this) {
        auto* check = node_find(replace_this);

        if (check) {
            if (check->father == nullptr) {
                if (check->left != nullptr) {
                    if (with_this <= check->left->key) {
                        throw tree::exception::TreeReplaceException();
                    }
                }
                if (check->right != nullptr) {
                    if (with_this >= check->right) {
                        throw tree::exception::TreeReplaceException();
                    }
                }
            }
            else if (check->is_left_son()) {
                if (with_this >= check->father->key) {
                    throw tree::exception::TreeReplaceException();
                }
                if (check->left) {
                    if (with_this <= check->left->key) {
                        throw tree::exception::TreeReplaceException();
                    }
                }
                if (check->right) {
                    if (with_this >= check->left->key) {
                        throw tree::exception::TreeReplaceException();
                    }
                }
            }
            else {
                if (with_this <= check->father->key) {
                    throw tree::exception::TreeReplaceException();
                }
                if (check->left) {
                    if (with_this <= check->left->key) {
                        throw tree::exception::TreeReplaceException();
                    }
                }
                if (check->right) {
                    if (with_this >= check->left->key) {
                        throw tree::exception::TreeReplaceException();
                    }
                }
            }

            check->key = with_this;
        }
    }

}

#endif //TREES_RBT_BUCKET_HPP
