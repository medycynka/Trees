//
// Created by PC on 22.11.2022.
//

#ifndef TREES_SET_HPP
#define TREES_SET_HPP

#pragma once

#include "rbt.hpp"
#include <vector>

namespace tree::rbt::structs{

    template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    class Set {
    public:
        typedef Allocator                                allocator;
        typedef tree::rbt::RBTree<T, Allocator>          struct_t;
        typedef Set<T, Allocator>                        self_type;
        typedef T                                        value_type;
        typedef T*                                       pointer;
        typedef const T*                                 const_pointer;
        typedef T&                                       reference;
        typedef const T&                                 const_reference;
        typedef tree::iterators::Iterator<tree::rbt::node_impl::RBNode<T>>             iterator;
        typedef tree::iterators::ConstIterator<tree::rbt::node_impl::RBNode<T>>        const_iterator;
        typedef tree::iterators::ReverseIterator<tree::rbt::node_impl::RBNode<T>>      reverse_iterator;
        typedef tree::iterators::ConstReverseIterator<tree::rbt::node_impl::RBNode<T>> const_reverse_iterator;

        Set() = default;
        Set(const Set<T, Allocator>& other)     : tree_set{ other.tree_set }, size_{ other.size_ } {};
        Set(Set<T, Allocator>&& other) noexcept : tree_set{ std::move(other.tree_set) }, size_{ std::move(other.size_) } {};
        Set(std::initializer_list<T> init) { for (auto& e : init) { insert(e); } }
        template<typename InputIt>
        Set(InputIt first, InputIt last)   { while (first != last) { insert(*first); first++; } }

        Set<T, Allocator>& operator= (const Set<T, Allocator>& other)     { if (this != &other) { tree_set = other.tree_set; size_ = other.size_; } return *this; };
        Set<T, Allocator>& operator= (Set<T, Allocator>&& other) noexcept { tree_set = other.tree_set; size_ = other.size_; return *this; };
        bool      operator==(const Set<T, Allocator>& other)              { return size_ == other.size_ && tree_set == other.tree_set; };
        bool      operator!=(const Set<T, Allocator>& other)              { return !(*this == other); };
        bool      operator< (const Set<T, Allocator>& other)              { return (size_ == other.size_ ? tree_set < other.tree_set : false); }
        bool      operator<=(const Set<T, Allocator>& other)              { return !(*this > other); }
        bool      operator> (const Set<T, Allocator>& other)              { return (size_ == other.size_ ? tree_set > other.tree_set : false); }
        bool      operator>=(const Set<T, Allocator>& other)              { return !(*this < other); }


        allocator get_allocator()                                                      { return tree_set.get_allocator(); };
        [[nodiscard]] std::size_t                 size()                         const { return size_; }
        [[nodiscard]] bool                        empty()                        const { return tree_set.isEmpty() && size_ == 0; }
        void                                      clear()                              { tree_set.clear(); size_ = 0; }
        bool                                      find_inSet(const_reference x)        { return tree_set.find(x); }
        void                                      show()                               { tree_set.display(); }
        bool                                      remove(const_reference x)            { if (tree_set.remove(x)) { size_--; return true; } return false; };
        void                                      insert_(const_reference x);
        std::pair<iterator, bool>                 insert(const_reference x);
        std::pair<iterator, bool>                 insert(T&& x);
        template< class InputIt >
        void                                      insert(InputIt first, InputIt last);
        void                                      insert(std::initializer_list<T> ilist);
        template< class... Args >
        std::pair<iterator, bool>                 emplace(Args&&... args);
        const_iterator                            find(const_reference x);
        void                                      merge(Set<T, Allocator>& other);
        void                                      merge(Set<T, Allocator>&& other);
        void                                      swap(Set<T, Allocator>& other) noexcept;
        std::size_t                               count(const_reference x)       const;
        std::pair<iterator, iterator>             equal_range(const_reference x)       { return { lower_bound(x), upper_bound(x) }; };
        std::pair<const_iterator, const_iterator> equal_range(const_reference x) const { return { lower_bound(x), upper_bound(x) }; };
        iterator                                  lower_bound(const_reference x);
        const_iterator                            lower_bound(const_reference x) const;
        iterator                                  upper_bound(const_reference x);
        const_iterator                            upper_bound(const_reference x) const;

        const_iterator         begin()   const noexcept { return tree_set.begin(); }
        const_iterator         end()     const noexcept { return tree_set.end(); }
        const_iterator         cbegin()  const noexcept { return tree_set.cbegin(); }
        const_iterator         cend()    const noexcept { return tree_set.cend(); }
        reverse_iterator       rbegin()  const noexcept { return tree_set.rbegin(); }
        reverse_iterator       rend()    const noexcept { return tree_set.rend(); }
        const_reverse_iterator crbegin() const noexcept { return tree_set.crbegin(); }
        const_reverse_iterator crend()   const noexcept { return tree_set.crend(); }

        friend std::ostream& operator<<(std::ostream& ofs, const Set<T, Allocator>& s) {
            ofs << s.tree_set << "\n";

            return ofs;
        }

    private:
        struct_t    tree_set;
        std::size_t size_ = 0;
    };

    template<typename T, typename Allocator>
    inline void Set<T, Allocator>::insert_(const_reference x) {
        if (!tree_set.find(x)) {
            tree_set.insert(x);
            size_++;
        }
        else {
            std::cout << "Value already exists in set" << std::endl;
        }
    }

    template<typename T, typename Allocator>
    inline std::pair<typename Set<T, Allocator>::iterator, bool> Set<T, Allocator>::insert(const_reference x) {
        if (!tree_set.find(x)) {
            tree_set.insert(x);
            size_++;

            for (auto it = tree_set.begin(); it != tree_set.end(); it++) {
                if (*it == x) {
                    return { it, true };
                }
            }

            std::cerr << "Error during insertion" << std::endl;
        }

        return { tree_set.end(), false };
    }

    template<typename T, typename Allocator>
    inline std::pair<typename Set<T, Allocator>::iterator, bool> Set<T, Allocator>::insert(T&& x)
    {
        if (!tree_set.find(x)) {
            tree_set.insert(x);
            size_++;

            for (auto it = tree_set.begin(); it != tree_set.end(); it++) {
                if (*it == x) {
                    return { it, true };
                }
            }

            std::cerr << "Error during insertion" << std::endl;
        }

        return { tree_set.end(), false };
    }

    template<typename T, typename Allocator>
    inline void Set<T, Allocator>::insert(std::initializer_list<T> ilist)
    {
        for (auto& e : ilist) {
            insert(e);
        }
    }

    template<typename T, typename Allocator>
    inline typename Set<T, Allocator>::const_iterator Set<T, Allocator>::find(const_reference x) {
        for (auto it = tree_set.cbegin(); it != tree_set.cend(); it++) {
            if (*it == x) {
                return it;
            }
        }

        return tree_set.cend();
    }

    template<typename T, typename Allocator>
    inline void Set<T, Allocator>::merge(Set<T, Allocator>& other) {
        std::vector<T> to_delete;
        for (auto it = other.begin(); it != other.end(); ++it) {
            if (insert(*it).second) {
                to_delete.push_back((*it).first);
            }
        }
        for (auto& e : to_delete) {
            other.remove(e);
        }
    }

    template<typename T, typename Allocator>
    inline void Set<T, Allocator>::merge(Set<T, Allocator>&& other) {
        std::vector<T> to_delete;
        for (auto it = other.begin(); it != other.end(); ++it) {
            if (insert(*it).second) {
                to_delete.push_back((*it).first);
            }
        }
        for (auto& e : to_delete) {
            other.remove(e);
        }
    }

    template<typename T, typename Allocator>
    inline void Set<T, Allocator>::swap(Set<T, Allocator>& other) noexcept {
        std::vector<T> helper;

        for (auto it = other.begin(); it != other.end(); ++it) {
            helper.push_back(*it);
        }

        other.clear();

        for (auto it = begin(); it != end(); ++it) {
            other.insert(*it);
        }

        clear();

        for (auto& e : helper) {
            insert(e);
        }

        helper.clear();
    }

    template<typename T, typename Allocator>
    inline std::size_t Set<T, Allocator>::count(const_reference x) const {
        std::size_t count_ = 0;

        for (auto it = begin(); it != end(); ++it) {
            if (x == *it) {
                count_++;
            }
        }

        return count_;
    }

    template<typename T, typename Allocator>
    inline typename Set<T, Allocator>::iterator Set<T, Allocator>::lower_bound(const_reference x) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it >= x) {
                return it;
            }
        }
        return end();
    }

    template<typename T, typename Allocator>
    inline typename Set<T, Allocator>::const_iterator Set<T, Allocator>::lower_bound(const_reference x) const {
        for (const auto it = cbegin(); it != cend(); ++it) {
            if (*it >= x) {
                return it;
            }
        }
        return cend();
    }

    template<typename T, typename Allocator>
    inline typename Set<T, Allocator>::iterator Set<T, Allocator>::upper_bound(const_reference x) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it > x) {
                return it;
            }
        }
        return end();
    }

    template<typename T, typename Allocator>
    inline typename Set<T, Allocator>::const_iterator Set<T, Allocator>::upper_bound(const_reference x) const {
        for (const auto it = cbegin(); it != cend(); ++it) {
            if (*it > x) {
                return it;
            }
        }
        return cend();
    }

    template<typename T, typename Allocator>
    template<class InputIt>
    inline void Set<T, Allocator>::insert(InputIt first, InputIt last)
    {
        while (first != last) {
            insert(*first);
            first++;
        }
    }

    template<typename T, typename Allocator>
    template<class ...Args>
    inline std::pair<typename Set<T, Allocator>::iterator, bool> Set<T, Allocator>::emplace(Args&& ...args)
    {
        T new_element{ std::forward<Args>(args)... };

        return insert(new_element);
    }
    
}

#endif //TREES_SET_HPP
