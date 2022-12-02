//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BST_HASH_NODE_HPP
#define TREES_BST_HASH_NODE_HPP

#pragma once

#include <iostream>
#include <memory_resource>
#include "exceptions.hpp"

namespace tree::bst::node_impl {

    template <typename T, std::size_t Mask = std::numeric_limits<std::size_t>::max(), typename Allocator = std::pmr::polymorphic_allocator<T>, typename Hash = std::hash<T>>
    class BSTHashNode {
    public:
        typedef Allocator                       allocator;
        typedef Hash                            hash_type;
        typedef T                               value_type;
        typedef T*                              pointer;
        typedef const T*                        const_pointer;
        typedef T&                              reference;
        typedef const T&                        const_reference;
        typedef std::size_t                     hash_value_type;
        typedef BSTHashNode<T, Mask, Allocator, Hash> self_type;

    public:
        value_type                             key;
        BSTHashNode<T, Mask, Allocator, Hash>* father;
        BSTHashNode<T, Mask, Allocator, Hash>* left;
        BSTHashNode<T, Mask, Allocator, Hash>* right;
        allocator                              al{};
        hash_type                              hash_func{};

    private:
        hash_value_type _hash_key{};

    public:
        BSTHashNode()                                                                     : father{ nullptr }, left{ nullptr }, right{ nullptr } {};
        explicit BSTHashNode(Allocator& alloc)                                            : father{ nullptr }, left{ nullptr }, right{ nullptr }, al{alloc} {};
        explicit BSTHashNode(value_type input)                                            : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr }, _hash_key{hash_func(input) % Mask} {};
        BSTHashNode(const BSTHashNode<T, Mask, Allocator, Hash>& s)                       : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, al{s.al.select_on_container_copy_construction()} {};
        BSTHashNode(const BSTHashNode<T, Mask, Allocator, Hash>& s, Allocator& alloc)     : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, al{alloc} {};
        BSTHashNode(BSTHashNode<T, Mask, Allocator, Hash>&& s) noexcept                   : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, al{std::move(s.al)} {};
        BSTHashNode(BSTHashNode<T, Mask, Allocator, Hash>&& s, Allocator& alloc) noexcept : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, al{alloc} {};
        BSTHashNode(value_type input, BSTHashNode<T, Mask, Allocator, Hash>* father_)     : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr }, _hash_key{hash_func(input) % Mask} {};
        ~BSTHashNode() = default;

        BSTHashNode<T, Mask, Allocator, Hash>& operator= (const_reference input)                     { key = input; _hash_key = hash_func(input) % Mask; return *this; };
        BSTHashNode<T, Mask, Allocator, Hash>& operator= (const BSTHashNode<T, Mask, Allocator, Hash>& input);
        BSTHashNode<T, Mask, Allocator, Hash>& operator= (BSTHashNode<T, Mask, Allocator, Hash>&& input) noexcept;
        bool                   operator==(const_reference input)                               const { return key == input; };
        bool                   operator!=(const_reference input)                               const { return key != input; };
        bool                   operator> (const_reference input)                               const { return key > input; };
        bool                   operator< (const_reference input)                               const { return key < input; };
        bool                   operator>=(const_reference input)                               const { return key >= input; };
        bool                   operator<=(const_reference input)                               const { return key <= input; };
        bool                   operator==(const BSTHashNode<T, Mask, Allocator, Hash>& source) const { return (_hash_key == source.get_hash_key() && father == source.father); };
        bool                   operator!=(const BSTHashNode<T, Mask, Allocator, Hash>& source) const { return (!(*this == source)); };
        bool                   operator> (const BSTHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key > source.get_hash_key(); };
        bool                   operator< (const BSTHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key < source.get_hash_key(); };
        bool                   operator>=(const BSTHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key >= source.get_hash_key(); };
        bool                   operator<=(const BSTHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key <= source.get_hash_key(); };
        explicit               operator bool()                                                 const { return (father != nullptr ? true : (left != nullptr ? true : right != nullptr)); };
        value_type             operator[](const size_t& id);
        value_type             operator[](const size_t& id) const;

        friend std::ostream& operator<<(std::ostream& ofs, const BSTHashNode<T, Mask, Allocator, Hash>* pt) {
            ofs << "Key: " << pt->key << ", Hash: " << pt->get_hash_key() << ", Mask: " << Mask << "\n";
            if (pt->father != nullptr) ofs << "(Father) key: " << pt->father->key << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt->left != nullptr) ofs << "(Left son) key: " << pt->left->key << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt->right != nullptr) ofs << "(Right son) key: " << pt->right->key << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        friend std::ostream& operator<<(std::ostream& ofs, const BSTHashNode<T, Mask, Allocator, Hash>& pt) {
            ofs << "Key: " << pt.key << ", Hash: " << pt.get_hash_key() << ", Mask: " << Mask << "\n";
            if (pt.father != nullptr) ofs << "(Father) key: " << pt.father->key << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt.left != nullptr) ofs << "(Left son) key: " << pt.left->key << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt.right != nullptr) ofs << "(Right son) key: " << pt.right->key << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        [[nodiscard]] inline hash_value_type                    get_hash_key() const { return _hash_key; };
        constexpr inline void                                   set_hash_key() const { _hash_key = hash_func(key) % Mask; };
        [[nodiscard]] bool                                      is_left_son()  const { return ((father != nullptr) && father->left == this); };
        [[nodiscard]] bool                                      is_right_son() const { return ((father != nullptr) && father->right == this); };
        BSTHashNode<T, Mask, Allocator, Hash>*                  max_node()           { return ((right == nullptr) ? this : right->max_node()); };
        BSTHashNode<T, Mask, Allocator, Hash>*                  min_node()           { return ((left == nullptr) ? this : left->min_node()); };
        allocator                                               get_allocator()      { return al; };
        hash_type                                               get_hash_func()      { return hash_func; };
        BSTHashNode<T, Mask, Allocator, Hash>*                  node_Successor();
        BSTHashNode<T, Mask, Allocator, Hash>*                  node_Predecessor();
        [[maybe_unused]] BSTHashNode<T, Mask, Allocator, Hash>* node_Sibling();
    };

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    BSTHashNode<T, Mask, Allocator, Hash>& BSTHashNode<T, Mask, Allocator, Hash>::operator=(const BSTHashNode<T, Mask, Allocator, Hash> &input) {
        if (this == &input) {
            return *this;
        }

        auto* newFather = al.allocate(1);
        auto* newLeft = al.allocate(1);
        auto* newRight = al.allocate(1);

        try {
            al.construct(newFather, *input.father);
            al.construct(newLeft, *input.left);
            al.construct(newRight, *input.right);
        }
        catch (...) {
            al.destroy(newFather);
            al.deallocate(newFather, 1);
            al.destroy(newLeft);
            al.deallocate(newLeft, 1);
            al.destroy(newRight);
            al.deallocate(newRight, 1);

            throw tree::exception::NodeFailedAllocException();
        }

        key = input.key;
        std::swap(father, newFather);
        std::swap(left, newLeft);
        std::swap(right, newRight);
        al = input.al;
        hash_func = input.hash_func;
        _hash_key = input.get_hash_key();

        al.destroy(newFather);
        al.deallocate(newFather, 1);
        al.destroy(newLeft);
        al.deallocate(newLeft, 1);
        al.destroy(newRight);
        al.deallocate(newRight, 1);

        return *this;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    BSTHashNode<T, Mask, Allocator, Hash>& BSTHashNode<T, Mask, Allocator, Hash>::operator=(BSTHashNode<T, Mask, Allocator, Hash> &&input) noexcept {
        auto* newFather = al.allocate(1);
        auto* newLeft = al.allocate(1);
        auto* newRight = al.allocate(1);

        try {
            al.construct(newFather, *input.father);
            al.construct(newLeft, *input.left);
            al.construct(newRight, *input.right);
        }
        catch (...) {
            al.destroy(newFather);
            al.deallocate(newFather, 1);
            al.destroy(newLeft);
            al.deallocate(newLeft, 1);
            al.destroy(newRight);
            al.deallocate(newRight, 1);

            throw tree::exception::NodeFailedAllocException();
        }

        key = std::move(input.key);
        std::swap(father, newFather);
        std::swap(left, newLeft);
        std::swap(right, newRight);
        al = std::move(input.al);
        hash_func = std::move(input.hash_func);
        _hash_key = std::move(input.get_hash_key());

        al.destroy(newFather);
        al.deallocate(newFather, 1);
        al.destroy(newLeft);
        al.deallocate(newLeft, 1);
        al.destroy(newRight);
        al.deallocate(newRight, 1);

        return *this;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    typename BSTHashNode<T, Mask, Allocator, Hash>::value_type BSTHashNode<T, Mask, Allocator, Hash>::operator[](const size_t &id) {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    typename BSTHashNode<T, Mask, Allocator, Hash>::value_type BSTHashNode<T, Mask, Allocator, Hash>::operator[](const size_t &id) const {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    inline BSTHashNode<T, Mask, Allocator, Hash>* BSTHashNode<T, Mask, Allocator, Hash>::node_Successor() {
        if (this != nullptr) {
            if (right != nullptr) return right->min_node();
            else if (is_left_son()) return father;

            auto successor = this;

            do {
                successor = successor->father;
            } while ((successor != nullptr) && (successor->is_right_son()));

            if (successor != nullptr) return successor->father;
            else return nullptr;
        }

        return nullptr;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    inline BSTHashNode<T, Mask, Allocator, Hash>* BSTHashNode<T, Mask, Allocator, Hash>::node_Predecessor() {
        if (this != nullptr) {
            if (left != nullptr) return left->max_node();
            else if (is_right_son()) return father;

            auto predecessor = this;

            do {
                predecessor = predecessor->father;
            } while ((predecessor != nullptr) && (predecessor->is_left_son()));

            if (predecessor != nullptr) return predecessor->father;
            else return nullptr;
        }

        return nullptr;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    [[maybe_unused]] inline BSTHashNode<T, Mask, Allocator, Hash>* BSTHashNode<T, Mask, Allocator, Hash>::node_Sibling() {
        if (this != nullptr) return (father == nullptr ? nullptr : (is_left_son() ? father->right : father->left));
        else return nullptr;
    }

}

#endif //TREES_BST_HASH_NODE_HPP
