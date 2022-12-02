//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BST_HPP
#define TREES_BST_HPP

#pragma once

#include "bst_node.hpp"
#include "iterators/iterator.hpp"
#include "iterators/const_iterator.hpp"
#include "iterators/reverse_iterator.hpp"
#include "iterators/const_reverse_iterator.hpp"

namespace tree::bst{

    template <typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    class BST{
    public:
        typedef Allocator                                                                        allocator;
        typedef T                                                                                value_type;
        typedef T&                                                                               reference;
        typedef const T&                                                                         const_reference;
        typedef T*                                                                               pointer;
        typedef const T*                                                                         const_pointer;
        typedef tree::bst::node_impl::BSTNode<T, Allocator>                             node_type;
        typedef tree::bst::node_impl::BSTNode<T, Allocator>*                            node_pointer;
        typedef const tree::bst::node_impl::BSTNode<T, Allocator>*                      const_node_pointer;
        typedef tree::bst::node_impl::BSTNode<T, Allocator>&                            node_reference;
        typedef const tree::bst::node_impl::BSTNode<T, Allocator>&                      const_node_reference;
        typedef typename __gnu_cxx::__alloc_traits<Allocator>::template rebind<node_type>::other node_allocator;
        typedef BST<T, Allocator>                                                                self_type;
        typedef tree::iterators::Iterator<tree::bst::node_impl::BSTNode<T, Allocator>>             iterator;
        typedef tree::iterators::ConstIterator<tree::bst::node_impl::BSTNode<T, Allocator>>        const_iterator;
        typedef tree::iterators::ReverseIterator<tree::bst::node_impl::BSTNode<T, Allocator>>      reverse_iterator;
        typedef tree::iterators::ConstReverseIterator<tree::bst::node_impl::BSTNode<T, Allocator>> const_reverse_iterator;

    private:
        void Copy(node_pointer);
        void Chop(node_pointer);
        void Display(node_pointer, std::size_t);

    public:
        BST()                                                      : size_{ 0 }, root_{ nullptr } {};
        explicit BST(node_allocator &alloc)                        : size_{ 0 }, root_{ nullptr }, al{alloc} {};
        BST(const BST<T, Allocator>& t)                            : size_{ 0 }, root_{ nullptr }, al{ t.al.select_on_container_copy_construction() } { Copy(t.root_); };
        BST(const BST<T, Allocator>& t, node_allocator &alloc)     : size_{ 0 }, root_{ nullptr }, al{alloc} { Copy(t.root_); };
        BST(BST<T, Allocator>&& t)                        noexcept : size_{ 0 }, root_{ nullptr }, al{t.al} { Copy(t.root_); t.clear(); };
        BST(BST<T, Allocator>&& t, node_allocator &alloc) noexcept : size_{ 0 }, root_{ nullptr }, al{alloc} { Copy(t.root_); t.clear(); };
        BST(std::initializer_list<T> init)                         : size_{ 0 }, root_{ nullptr } { for (auto& e : init) { insert(e); } };
        template<typename InputIt>
        BST(InputIt first, InputIt last)                           : size_{ 0 }, root_{ nullptr } { while (first != last) { insert(*first); first++; } };

        node_pointer                              getRoot()        const { return root_; };
        iterator                                  root()                 { return iterator(root_); };
        const_iterator                            root()           const { return const_iterator(root_); };
        const_iterator                            croot()          const { return const_iterator(root_); };
        allocator                                 get_allocator()  const { return root_->get_allocator(); };
        node_allocator                            get_node_alloc() const { return al; };
        size_t                                    size()                 { return size_; };
        [[nodiscard]] bool                        empty()          const { return (root_ == nullptr && size_ == 0); };
        void                                      display()              { Display(root_, 0); };
        void                                      clear()                { size_ = 0; Chop(root_); root_ = nullptr; };
        node_pointer                              maxIt()          const { return (empty() ? nullptr : root_->max_node()); };
        iterator                                  maxIter()              { return iterator(maxIt()); };
        node_pointer                              minIt()          const { return (empty() ? nullptr : root_->min_node()); };
        iterator                                  minIter()              { return iterator(minIt()); };
        iterator                                  insert(const_reference);
        void                                      insert(iterator first, iterator last);
        std::pair<iterator, bool>                 insert_unique(const_reference);
        iterator                                  find(const_reference);
        const_iterator                            find(const_reference) const;
        std::size_t                               count(const_reference);
        iterator                                  iterator_to(const_reference);
        const_iterator                            iterator_to(const_reference) const;
        node_pointer                              node_find(const_reference);
        void                                      replace(const_reference, const_reference);
        bool                                      remove(const_reference);
        iterator                                  erase(const_iterator pos);
        iterator                                  erase(iterator pos);
        iterator                                  erase(iterator first, iterator last);
        std::size_t                               erase(const_reference);
        void                                      swap(BST<T, Allocator>&) noexcept;

        [[maybe_unused]] void                                      copy_from(const BST<T, Allocator>& src);
        [[maybe_unused]] void                                      copy_from(BST<T, Allocator>&& src);
        [[maybe_unused]] std::pair<iterator, iterator>             bounded_range(const_reference from, const_reference to);
        [[maybe_unused]] std::pair<const_iterator, const_iterator> bounded_range(const_reference from, const_reference to) const;
        [[maybe_unused]] std::pair<iterator, iterator>             equal_range(const_reference x);
        [[maybe_unused]] std::pair<const_iterator, const_iterator> equal_range(const_reference x) const;
        iterator                                                   lower_bound(const_reference x);
        [[maybe_unused]] const_iterator                            lower_bound(const_reference x) const;
        iterator                                                   upper_bound(const_reference x);
        [[maybe_unused]] const_iterator                            upper_bound(const_reference x) const;

        /*
        * Ostream overloading
        */
        friend std::ostream& operator<<(std::ostream& ofs, const BST<T, Allocator>& tree) {
            for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
                ofs << *it << ", ";
            }

            ofs << "\n";

            return ofs;
        }

        friend std::ostream& operator<<(std::ostream& ofs, const BST<T, Allocator>* tree) {
            for (auto it = tree->cbegin(); it != tree->cend(); ++it) {
                ofs << *it << ", ";
            }

            ofs << "\n";

            return ofs;
        }

        BST<T, Allocator>& operator+=(const_reference x)                   { insert(x); return *this; };
        BST<T, Allocator>& operator-=(const_reference x)                   { remove(x); return *this; };
        value_type         operator[](const size_t& id);
        value_type         operator[](const size_t& id)              const;
        BST<T, Allocator>& operator= (const BST<T, Allocator>&);
        BST<T, Allocator>& operator= (BST<T, Allocator>&&)           noexcept;
        BST<T, Allocator>& operator= (std::initializer_list<T> ilist);
        bool               operator==(const BST<T, Allocator>&)      const;
        bool               operator!=(const BST<T, Allocator>& tree) const { return !(*this == tree); };
        bool               operator< (const BST<T, Allocator>& tree) const;
        bool               operator> (const BST<T, Allocator>& tree) const { return tree < *this; };
        bool               operator<=(const BST<T, Allocator>& tree) const { return !(*this > tree); };
        bool               operator>=(const BST<T, Allocator>& tree) const { return !(*this < tree); };
        explicit           operator bool()                           const { return !empty(); };

        iterator               begin()         noexcept { return iterator(minIt()); };
        iterator               end()           noexcept { return iterator(); };
        const_iterator         begin()   const noexcept { return const_iterator(minIt()); };
        const_iterator         end()     const noexcept { return const_iterator(); };
        const_iterator         cbegin()  const noexcept { return const_iterator(minIt()); };
        const_iterator         cend()    const noexcept { return const_iterator(); };
        reverse_iterator       rbegin()        noexcept { return reverse_iterator(maxIt()); };
        reverse_iterator       rend()          noexcept { return reverse_iterator(); };
        const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(maxIt()); };
        const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(); };
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(maxIt()); };
        const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(); };

    private:
        std::size_t    size_;
        node_pointer   root_;
        node_allocator al{};
    };

    template <typename T, typename Allocator>
    inline void BST<T, Allocator>::Copy(node_pointer in) {
        if (in) {
            insert(in->key);
            Copy(in->left);
            Copy(in->right);
        }
    }

    template <typename T, typename Allocator>
    inline void BST<T, Allocator>::Chop(node_pointer in) {
        if (in) {
            Chop(in->left);
            Chop(in->right);

            al.destroy(in);
            al.deallocate(in, 1);
        }
    }

    template <typename T, typename Allocator>
    inline void BST<T, Allocator>::Display(node_pointer in, std::size_t level) {
        if (in == nullptr) return;

        std::cout << "level: " << level << std::endl;
        std::cout << in;

        Display(in->left, level + 1);
        Display(in->right, level + 1);
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::insert(const_reference input) {
        node_pointer q;
        auto* create = al.allocate(1);
        al.construct(create, input);
        auto* p = root_;
        q = nullptr;

        if (root_ == nullptr) {
            root_ = create;
            create->father = nullptr;
        }
        else {
            while (p != nullptr) {
                q = p;

                if (p->key > create->key) p = p->left;
                else if (p->key < create->key) p = p->right;
                else return end();
            }

            create->father = q;

            if (q->key < create->key) q->right = create;
            else if (q->key > create->key) q->left = create;
            else return end();
        }

        size_++;

        return iterator_to(input);
    }

    template <typename T, typename Allocator>
    inline void BST<T, Allocator>::insert(BST<T, Allocator>::iterator first, BST<T, Allocator>::iterator last) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }

    template <typename T, typename Allocator>
    inline std::pair<typename BST<T, Allocator>::iterator, bool> BST<T, Allocator>::insert_unique(const_reference val) {
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
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::find(const_reference val) {
        auto* t = root_;

        while (t != nullptr) {
            if (t->key == val) return iterator(t);
            if (val > t->key) t = t->right;
            else if (val < t->key) t = t->left;
        }

        return end();
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::const_iterator BST<T, Allocator>::find(const_reference val) const {
        auto* t = root_;

        while (t != nullptr) {
            if (t->key == val) return const_iterator(t);
            if (val > t->key) t = t->right;
            else if (val < t->key) t = t->left;
        }

        return cend();
    }

    template <typename T, typename Allocator>
    inline std::size_t BST<T, Allocator>::count(const_reference key) {
        std::size_t count = 0;

        for (auto it = begin(); it != end(); ++it) {
            if (*it == key) {
                count++;
            }
        }

        return count;
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::iterator_to(const_reference key) {
        return find(key);
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::const_iterator BST<T, Allocator>::iterator_to(const_reference key) const {
        return find(key);
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::node_pointer BST<T, Allocator>::node_find(const_reference val) {
        auto* t = root_;

        while (t != nullptr) {
            if (t->key == val) return t;
            if (val > t->key) t = t->right;
            else if (val < t->key) t = t->left;
        }

        return nullptr;
    }

    template <typename T, typename Allocator>
    inline void BST<T, Allocator>::replace(const_reference replace_this, const_reference with_this) {
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

    template <typename T, typename Allocator>
    inline bool BST<T, Allocator>::remove(const_reference x) {
        if (root_ == nullptr) {
            std::cout << "\nEmpty RBTree.";

            return false;
        }

        auto* p = root_;
        auto found = 0;

        while (p != nullptr && found == 0) {
            if (p->key == x) found = 1;
            if (found == 0) {
                if (p->key < x) p = p->right;
                else p = p->left;
            }
        }

        if (found == 0){
            return false;
        } else{
            node_pointer y;
            node_pointer q;

            if (p->left == nullptr || p->right == nullptr) y = p;
            else y = p->node_Successor();

            if (y->left != nullptr) q = y->left;
            else {
                if (y->right != nullptr) q = y->right;
                else q = nullptr;
            }

            if (q != nullptr) q->father = y->father;

            if (y->father == nullptr) root_ = q;
            else {
                if (y == y->father->left) y->father->left = q;
                else y->father->right = q;
            }

            if (y != p) {
                p->key = y->key;
            }

            size_--;

            return true;
        }
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::erase(BST<T, Allocator>::const_iterator pos) {
        auto ret = iterator(pos.getIter());
        ++ret;
        remove(*pos);

        return ret;
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::erase(BST<T, Allocator>::iterator pos) {
        auto ret = iterator(pos.getIter());
        ++ret;
        remove(*pos);

        return ret;
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::erase(BST<T, Allocator>::iterator first, BST<T, Allocator>::iterator last) {
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
    inline std::size_t BST<T, Allocator>::erase(const_reference key) {
        std::size_t count = 0;

        while (find(key)) {
            remove(key);
            count++;
        }

        return count;
    }

    template <typename T, typename Allocator>
    inline void BST<T, Allocator>::swap(BST<T, Allocator>& other) noexcept {
        std::vector<T> swaper;

        for (auto it = other.begin(); it != other.end(); ++it) {
            swaper.push_back(*it);
        }

        other.clear();
        other.insert(begin(), end());
        clear();

        for (auto& e : swaper) {
            insert(e);
        }
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline void BST<T, Allocator>::copy_from(const BST<T, Allocator>& src) {
        *this = src;
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline void BST<T, Allocator>::copy_from(BST<T, Allocator>&& src) {
        *this = std::move(src);
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline std::pair<typename BST<T, Allocator>::iterator, typename BST<T, Allocator>::iterator> BST<T, Allocator>::bounded_range(const_reference from, const_reference to) {
        if (from <= to) {
            iterator f_;
            iterator t_;

            for (auto it = begin(); it != end(); ++it) {
                if (!f_ && *it == from) {
                    f_ = it;
                }
                if (!t_ && *it == to) {
                    t_ = it;
                }
                if (f_ && t_) {
                    break;
                }
            }

            return { f_ ? f_ : end(), t_ ? t_ : end() };
        }
        else {
            return { end(), end() };
        }
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline std::pair<typename BST<T, Allocator>::const_iterator, typename BST<T, Allocator>::const_iterator> BST<T, Allocator>::bounded_range(const_reference from, const_reference to) const {
        if (from <= to) {
            const_iterator f_;
            const_iterator t_;

            for (const auto it = cbegin(); it != cend(); ++it) {
                if (!f_ && *it == from) {
                    f_ = it;
                }
                if (!t_ && *it == to) {
                    t_ = it;
                }
                if (f_ && t_) {
                    break;
                }
            }

            return { f_ ? f_ : cend(), t_ ? t_ : cend() };
        }
        else {
            return { cend(), cend() };
        }
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline std::pair<typename BST<T, Allocator>::iterator, typename BST<T, Allocator>::iterator> BST<T, Allocator>::equal_range(const_reference x) {
        return { lower_bound(x), upper_bound(x) };
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline std::pair<typename BST<T, Allocator>::const_iterator, typename BST<T, Allocator>::const_iterator> BST<T, Allocator>::equal_range(const_reference x) const {
        return { lower_bound(x), upper_bound(x) };
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::lower_bound(const_reference x) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it >= x) {
                return it;
            }
        }

        return end();
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline typename BST<T, Allocator>::const_iterator BST<T, Allocator>::lower_bound(const_reference x) const {
        for (const auto it = cbegin(); it != cend(); ++it) {
            if (*it >= x) {
                return it;
            }
        }

        return cend();
    }

    template <typename T, typename Allocator>
    inline typename BST<T, Allocator>::iterator BST<T, Allocator>::upper_bound(const_reference x) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it > x) {
                return it;
            }
        }

        return end();
    }

    template <typename T, typename Allocator>
    [[maybe_unused]] inline typename BST<T, Allocator>::const_iterator BST<T, Allocator>::upper_bound(const_reference x) const {
        for (const auto it = cbegin(); it != cend(); ++it) {
            if (*it > x) {
                return it;
            }
        }

        return cend();
    }

    template <typename T, typename Allocator>
    typename BST<T, Allocator>::value_type BST<T, Allocator>::operator[](const size_t &id) {
        if (id < 0 || id >= size_) throw tree::exception::TreeIndexOutOfBoundException();
        else {
            if (id == 0) return minIt()->key;

            auto count = 1;
            auto* n = minIt()->node_Successor();

            while (count++ != id) n = n->node_Successor();

            return n->key;
        }
    }

    template <typename T, typename Allocator>
    typename BST<T, Allocator>::value_type BST<T, Allocator>::operator[](const size_t &id) const {
        if (id < 0 || id >= size_) throw tree::exception::TreeIndexOutOfBoundException();
        else {
            if (id == 0) return minIt()->key;

            auto count = 1;
            auto* n = minIt()->node_Successor();

            while (count++ != id) n = n->node_Successor();

            return n->key;
        }
    }

    template <typename T, typename Allocator>
    BST<T, Allocator>& BST<T, Allocator>::operator=(const BST<T, Allocator>& tree) {
        if (this != &tree) {
            Chop(root_);
            root_ = nullptr;
            size_ = 0;
            Copy(tree.root_);
            al = tree.al;
        }

        return *this;
    }

    template <typename T, typename Allocator>
    BST<T, Allocator>& BST<T, Allocator>::operator=(BST<T, Allocator>&& tree) noexcept {
        Chop(root_);
        root_ = nullptr;
        size_ = 0;
        Copy(tree.root_);
        al = tree.al;
        tree.clear();

        return *this;
    }

    template <typename T, typename Allocator>
    BST<T, Allocator> &BST<T, Allocator>::operator=(std::initializer_list<T> ilist) {
        Chop(root_);
        size_ = 0;

        for(auto &e : ilist){
            insert(e);
        }

        return *this;
    }

    template <typename T, typename Allocator>
    bool BST<T, Allocator>::operator==(const BST<T, Allocator>& tree) const {
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
    bool BST<T, Allocator>::operator<(const BST<T, Allocator>& tree) const {
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

}

#endif //TREES_BST_HPP
