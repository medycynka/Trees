//
// Created by PC on 23.11.2022.
//

#ifndef TREES_MAP_HPP
#define TREES_MAP_HPP

#pragma once

#include "rbt.hpp"
#include <vector>

namespace tree::rbt::structs{

    template<typename K, typename V, typename Allocator = std::pmr::polymorphic_allocator<std::pair<const K, V>>>
    class Map {
    public:
        class MapIterator : public tree::iterators::Iterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>> {
            public:
            MapIterator() = default;
            explicit MapIterator(tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>* ptr) : tree::iterators::Iterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>(ptr) {};

            V& operator*() { return tree::iterators::Iterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>::Iter->key.second; };
        };

        class MapReverseIterator : public tree::iterators::ReverseIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>> {
            public:
            MapReverseIterator() = default;
            explicit MapReverseIterator(tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>* ptr) : tree::iterators::ReverseIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>(ptr) {};

            V& operator*() { return tree::iterators::ReverseIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>::Iter->key.second; };
        };

        class MapConstIterator : public tree::iterators::ConstIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>> {
            public:
            MapConstIterator() = default;
            explicit MapConstIterator(tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>* ptr) : tree::iterators::ConstIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>(ptr) {};

            V const& operator*() { return tree::iterators::ConstIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>::Iter->key.second; };
        };

        class MapConstReverseIterator : public tree::iterators::ConstReverseIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>> {
            public:
            MapConstReverseIterator() = default;
            explicit MapConstReverseIterator(tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>* ptr) : tree::iterators::ConstReverseIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>(ptr) {};

            V const& operator*() { return tree::iterators::ConstReverseIterator<tree::rbt::node_impl::RBNode<std::pair<const K, V>, Allocator>>::Iter->key.second; };
        };

    public:
        typedef Map<K, V, Allocator>                                self_type;
        typedef K                                                   key_t;
        typedef K*                                                  key_ptr_t;
        typedef const K*                                            const_key_ptr_t;
        typedef K&                                                  key_ref_t;
        typedef const K&                                            const_key_ref_t;
        typedef V                                                   value_type;
        typedef V*                                                  value_pointer;
        typedef V&                                                  value_reference;
        typedef const V&                                            value_const_reference;
        typedef std::pair<const K, V>                               node_type;
        typedef std::pair<const K, V>*                              node_pointer;
        typedef std::pair<const K, V>&                              node_reference;
        typedef const std::pair<const K, V>&                        node_const_reference;
        typedef MapIterator                                         iterator;
        typedef MapConstIterator                                    const_iterator;
        typedef MapReverseIterator                                  reverse_iterator;
        typedef MapConstReverseIterator                             const_reverse_iterator;
        typedef tree::rbt::RBTree<std::pair<const K, V>, Allocator> struct_t;

        Map() = default;
        Map(const Map<K, V, Allocator>& other)     : tree_map{ other.tree_map }, size_{ other.size_ } {};
        Map(Map<K, V, Allocator>&& other) noexcept : tree_map{ std::move(other.tree_map) }, size_{ std::move(other.size_) } {};
        Map(std::initializer_list<node_type> init) { for (auto& e : init) { insert(e); } };
        template<typename InputIt>
        Map(InputIt first, InputIt last) { while (first != last) { insert(*first); first++; } };

        [[nodiscard]] std::size_t                 size()  const { return this->size_; };
        [[nodiscard]] bool                        empty() const { return size_ == 0; };
        void                                      clear() { tree_map.clear(); size_ = 0; };
        template< class... Args >
        std::pair<iterator, bool>                 emplace(Args&&... args);
        template <class... Args>
        std::pair<iterator, bool>                 try_emplace(const_key_ref_t k, Args&&... args);
        template <class... Args>
        std::pair<iterator, bool>                 try_emplace(K&& k, Args&&... args);
        std::pair<iterator, bool>                 insert(node_const_reference key_value);
        std::pair<iterator, bool>                 insert(std::pair<const K, V>&& key_value);
        template< class InputIt >
        void                                      insert(InputIt first, InputIt last);
        void                                      insert(std::initializer_list<node_type> ilist);
        const_iterator                            find(const_key_ref_t key) const;
        iterator                                  find(const_key_ref_t key);
        bool                                      remove(const_key_ref_t key);
        iterator                                  erase(iterator pos);
        iterator                                  erase(const_iterator first, const_iterator last);
        std::size_t                               erase(const_key_ref_t key);
        value_reference                           at(const_key_ref_t key);
        value_const_reference                     at(const_key_ref_t key) const;
        void                                      swap(Map<K, V, Allocator>& other) noexcept;
        void                                      merge(Map<K, V, Allocator>& other);
        void                                      merge(Map<K, V, Allocator>&& other);
        std::size_t                               count(const_key_ref_t x);
        std::pair<iterator, iterator>             equal_range(const_key_ref_t x);
        std::pair<const_iterator, const_iterator> equal_range(const_key_ref_t x) const;
        iterator                                  lower_bound(const_key_ref_t x);
        const_iterator                            lower_bound(const_key_ref_t x) const;
        iterator                                  upper_bound(const_key_ref_t x);
        const_iterator                            upper_bound(const_key_ref_t x) const;

        Map<K, V, Allocator>& operator= (const Map<K, V, Allocator>& source)     { if (this != &source) { this->tree_map = source.tree_map; this->size_ = source.size_; } return *this; };
        Map<K, V, Allocator>& operator= (Map<K, V, Allocator>&& source) noexcept { this->tree_map = source.tree_map; this->size_ = source.size_; return *this; };
        bool                  operator==(const Map<K, V, Allocator>& source)     { return (size_ == source.size_ && tree_map == source.tree_map); };
        bool                  operator!=(const Map<K, V, Allocator>& source)     { return !(*this == source); };
        value_reference       operator[](K&& key);

        iterator               begin()         { return iterator(tree_map.minIt()); };
        iterator               end()           { return iterator(); };
        const_iterator         begin()   const { return const_iterator(tree_map.minIt()); };
        const_iterator         end()     const { return const_iterator(); };
        const_iterator         cbegin()  const { return const_iterator(tree_map.minIt()); };
        const_iterator         cend()    const { return const_iterator(); }
        reverse_iterator       rbegin()  const { return reverse_iterator(tree_map.maxIt()); };
        reverse_iterator       rend()    const { return reverse_iterator(); }
        const_reverse_iterator rbegin()        { return const_reverse_iterator(tree_map.maxIt()); };
        const_reverse_iterator rend()          { return const_reverse_iterator(); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(tree_map.maxIt()); };
        const_reverse_iterator crend()   const { return const_reverse_iterator(); }

        friend std::ostream& operator<<(std::ostream& ofs, const Map<K, V, Allocator>& m) {
            for(auto it = m.begin(); it != m.end(); ++it){
                std::cout << *it << ", ";
            }

            ofs << "\n";

            return ofs;
        }

    private:
        struct_t    tree_map;
        std::size_t size_ = 0;
    };

    template<typename K, typename V, typename Allocator>
    inline std::pair<typename Map<K, V, Allocator>::iterator, bool> Map<K, V, Allocator>::insert(node_const_reference key_value) {
        auto pom = find(key_value.first);

        if (pom) {
            return { pom, false };
        }
        else {
            tree_map.insert(key_value);
            size_++;

            return { find(key_value.first), true };
        }
    }

    template<typename K, typename V, typename Allocator>
    inline std::pair<typename Map<K, V, Allocator>::iterator, bool> Map<K, V, Allocator>::insert(std::pair<const K, V>&& key_value)
    {
        auto pom = find(key_value.first);

        if (pom) {
            return { pom, false };
        }
        else {
            tree_map.insert(key_value);
            size_++;

            return { find(key_value.first), true };
        }
    }

    template<typename K, typename V, typename Allocator>
    inline void Map<K, V, Allocator>::insert(std::initializer_list<node_type> ilist)
    {
        for (auto& e : ilist) {
            insert(e);
        }
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::const_iterator Map<K, V, Allocator>::find(const_key_ref_t key) const {
        for (auto it = tree_map.cbegin(); it != tree_map.cend(); ++it) {
            if ((*it).first == key) {
                return const_iterator(it.getIter());
            }
        }

        return cend();
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::iterator Map<K, V, Allocator>::find(const_key_ref_t key) {
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first == key) {
                return iterator(it.getIter());
            }
        }

        return end();
    }

    template<typename K, typename V, typename Allocator>
    inline bool Map<K, V, Allocator>::remove(const_key_ref_t key) {
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first == key) {
                size_--;

                return tree_map.remove(*it);
            }
        }

        return false;
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::iterator Map<K, V, Allocator>::erase(iterator pos)
    {
        V next_val = *(++find(*pos));
        remove(pos->first);

        return find(next_val);
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::iterator Map<K, V, Allocator>::erase(const_iterator first, const_iterator last)
    {
        V ret_val = *last;

        for (const_iterator it = first; it != last; ++it) {
            remove(it->first);
        }

        return find(ret_val);
    }

    template<typename K, typename V, typename Allocator>
    inline std::size_t Map<K, V, Allocator>::erase(const_key_ref_t key)
    {
        std::size_t count = 0;

        while (find(key) != end()) {
            remove(key);
            count++;
        }

        return count;
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::value_reference Map<K, V, Allocator>::at(const_key_ref_t key) {
        auto look_for = find(key);

        if (look_for != end()) {
            return *look_for;
        }
        else {
            return *end();
        }
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::value_const_reference Map<K, V, Allocator>::at(const_key_ref_t key) const {
        auto look_for = find(key);

        if (look_for != cend()) {
            return *look_for;
        }
        else {
            return *cend();
        }
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::value_reference Map<K, V, Allocator>::operator[](K&& key) {
        auto look_for = find(key);

        if (look_for != end()) {
            return *look_for;
        }
        else {
            return *(insert({ std::move(key), V() })).first;
        }
    }

    template<typename K, typename V, typename Allocator>
    inline void Map<K, V, Allocator>::merge(Map<K, V, Allocator>& other) {
        std::vector<K> to_delete;
        for (auto it = other.begin(); it != other.end(); ++it) {
            if (insert(it->key).second) {
                to_delete.push_back(it->key.first);
            }
        }
        for (auto& e : to_delete) {
            other.remove(e);
        }
    }

    template<typename K, typename V, typename Allocator>
    inline void Map<K, V, Allocator>::merge(Map<K, V, Allocator>&& other) {
        std::vector<K> to_delete;
        for (auto it = other.begin(); it != other.end(); ++it) {
            if (insert(it->key).second) {
                to_delete.push_back(it->key.first);
            }
        }
        for (auto& e : to_delete) {
            other.remove(e);
        }
    }

    template<typename K, typename V, typename Allocator>
    inline std::size_t Map<K, V, Allocator>::count(const_key_ref_t x) {
        std::size_t count_ = 0;
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first == x) {
                count_++;
            }
        }

        return count_;
    }

    template<typename K, typename V, typename Allocator>
    inline void Map<K, V, Allocator>::swap(Map<K, V, Allocator>& other) noexcept {
        std::vector<std::pair<K, V>> helper;

        for (auto it = other.tree_map.begin(); it != other.tree_map.end(); ++it) {
            helper.push_back(*it);
        }

        other.clear();

        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            other.insert(*it);
        }

        clear();

        for (auto& e : helper) {
            insert(e);
        }

        helper.clear();
    }

    template<typename K, typename V, typename Allocator>
    inline std::pair<typename Map<K, V, Allocator>::iterator, typename Map<K, V, Allocator>::iterator> Map<K, V, Allocator>::equal_range(const_key_ref_t x) {
        return { lower_bound(x), upper_bound(x) };
    }

    template<typename K, typename V, typename Allocator>
    inline std::pair<typename Map<K, V, Allocator>::const_iterator, typename Map<K, V, Allocator>::const_iterator> Map<K, V, Allocator>::equal_range(const_key_ref_t x) const {
        return { lower_bound(x), upper_bound(x) };
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::iterator Map<K, V, Allocator>::lower_bound(const_key_ref_t x) {
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first >= x) {
                return Map<K, V, Allocator>::iterator(it.getIter());
            }
        }

        return end();
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::const_iterator Map<K, V, Allocator>::lower_bound(const_key_ref_t x) const {
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first >= x) {
                return Map<K, V, Allocator>::const_iterator(it.getIter());
            }
        }

        return cend();
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::iterator Map<K, V, Allocator>::upper_bound(const_key_ref_t x) {
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first > x) {
                return Map<K, V, Allocator>::iterator(it.getIter());
            }
        }

        return end();
    }

    template<typename K, typename V, typename Allocator>
    inline typename Map<K, V, Allocator>::const_iterator Map<K, V, Allocator>::upper_bound(const_key_ref_t x) const {
        for (auto it = tree_map.begin(); it != tree_map.end(); ++it) {
            if ((*it).first > x) {
                return Map<K, V, Allocator>::const_iterator(it.getIter());
            }
        }

        return cend();
    }

    template<typename K, typename V, typename Allocator>
    template<class ...Args>
    inline std::pair<typename Map<K, V, Allocator>::iterator, bool> Map<K, V, Allocator>::emplace(Args&& ...args)
    {
        std::pair<const K, V> to_emplace(std::forward<Args>(args)...);
        return insert(to_emplace);
    }

    template<typename K, typename V, typename Allocator>
    template<class ...Args>
    inline std::pair<typename Map<K, V, Allocator>::iterator, bool> Map<K, V, Allocator>::try_emplace(const_key_ref_t k, Args&& ...args)
    {
        std::pair<const K, V> to_emplace(std::piecewise_construct, std::forward_as_tuple(k), std::forward_as_tuple(std::forward<Args>(args)...));
        return insert(to_emplace);
    }

    template<typename K, typename V, typename Allocator>
    template<class ...Args>
    inline std::pair<typename Map<K, V, Allocator>::iterator, bool> Map<K, V, Allocator>::try_emplace(K&& k, Args&& ...args)
    {
        std::pair<const K, V> to_emplace(std::piecewise_construct, std::forward_as_tuple(std::move(k)), std::forward_as_tuple(std::forward<Args>(args)...));
        return insert(to_emplace);
    }

    template<typename K, typename V, typename Allocator>
    template<class InputIt>
    inline void Map<K, V, Allocator>::insert(InputIt first, InputIt last)
    {
        while (first != last) {
            insert(*first);
            first++;
        }
    }

}

#endif //TREES_MAP_HPP
