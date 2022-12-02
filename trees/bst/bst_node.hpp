//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BST_NODE_HPP
#define TREES_BST_NODE_HPP

#pragma once

#include <iostream>
#include <memory_resource>
#include "exceptions.hpp"

namespace tree::bst::node_impl {

    template <typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    class BSTNode {
    public:
        typedef Allocator             allocator;
        typedef T                     value_type;
        typedef T*                    pointer;
        typedef const T*              const_pointer;
        typedef T&                    reference;
        typedef const T&              const_reference;
        typedef BSTNode<T, Allocator> self_type;;

        value_type             key;
        BSTNode<T, Allocator>* father;
        BSTNode<T, Allocator>* left;
        BSTNode<T, Allocator>* right;
        allocator              al{};

        BSTNode()                                                                : father{ nullptr }, left{ nullptr }, right{ nullptr } {};
        explicit BSTNode(Allocator& alloc)                                       : father{ nullptr }, left{ nullptr }, right{ nullptr }, al{alloc} {};
        explicit BSTNode(value_type input)                                       : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr } {};
        BSTNode(const BSTNode<T, Allocator>& s)                                  : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, al{s.al.select_on_container_copy_construction()} {};
        BSTNode(const BSTNode<T, Allocator>& s, Allocator& alloc)                : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, al{alloc} {};
        BSTNode(BSTNode<T, Allocator>&& s) noexcept                              : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, al{std::move(s.al)} {};
        BSTNode(BSTNode<T, Allocator>&& s, Allocator& alloc) noexcept            : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, al{alloc} {};
        BSTNode(value_type input, BSTNode<T, Allocator>* father_)                : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr } {};
        ~BSTNode() = default;

        BSTNode<T, Allocator>& operator= (const_reference input)                     { key = input; return *this; };
        BSTNode<T, Allocator>& operator= (const BSTNode<T, Allocator>& input);
        BSTNode<T, Allocator>& operator= (BSTNode<T, Allocator>&& input) noexcept;
        bool                   operator==(const_reference input)               const { return key == input; };
        bool                   operator!=(const_reference input)               const { return key != input; };
        bool                   operator> (const_reference input)               const { return key > input; };
        bool                   operator< (const_reference input)               const { return key < input; };
        bool                   operator>=(const_reference input)               const { return key >= input; };
        bool                   operator<=(const_reference input)               const { return key <= input; };
        bool                   operator==(const BSTNode<T, Allocator>& source) const { return ((key == source.key) && (father == source.father)); };
        bool                   operator!=(const BSTNode<T, Allocator>& source) const { return (!(*this == source)); };
        bool                   operator> (const BSTNode<T, Allocator>& source) const { return key > source.key; };
        bool                   operator< (const BSTNode<T, Allocator>& source) const { return key < source.key; };
        bool                   operator>=(const BSTNode<T, Allocator>& source) const { return key >= source.key; };
        bool                   operator<=(const BSTNode<T, Allocator>& source) const { return key <= source.key; };
        explicit               operator bool()                                 const { return (father != nullptr ? true : (left != nullptr ? true : right != nullptr)); };
        value_type             operator[](const size_t& id);
        value_type             operator[](const size_t& id) const;

        friend std::ostream& operator<<(std::ostream& ofs, const BSTNode<T, Allocator>* pt) {
            ofs << "Key: " << pt->key << "\n";
            if (pt->father != nullptr) ofs << "(Father) key: " << pt->father->key << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt->left != nullptr) ofs << "(Left son) key: " << pt->left->key << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt->right != nullptr) ofs << "(Right son) key: " << pt->right->key << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        friend std::ostream& operator<<(std::ostream& ofs, const BSTNode<T, Allocator>& pt) {
            ofs << "Key: " << pt.key << "\n";
            if (pt.father != nullptr) ofs << "(Father) key: " << pt.father->key << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt.left != nullptr) ofs << "(Left son) key: " << pt.left->key << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt.right != nullptr) ofs << "(Right son) key: " << pt.right->key << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        [[nodiscard]] bool                      is_left_son()  const { return ((father != nullptr) && father->left == this); };
        [[nodiscard]] bool                      is_right_son() const { return ((father != nullptr) && father->right == this); };
        BSTNode<T, Allocator>*                  max_node()           { return ((right == nullptr) ? this : right->max_node()); };
        BSTNode<T, Allocator>*                  min_node()           { return ((left == nullptr) ? this : left->min_node()); };
        allocator                               get_allocator()      { return al; };
        virtual BSTNode<T, Allocator>*          node_Successor();
        virtual BSTNode<T, Allocator>*          node_Predecessor();
        [[maybe_unused]] BSTNode<T, Allocator>* node_Sibling();
    };

    template<typename T, typename Allocator>
    BSTNode<T, Allocator>& BSTNode<T, Allocator>::operator=(const BSTNode<T, Allocator> &input) {
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

        al.destroy(newFather);
        al.deallocate(newFather, 1);
        al.destroy(newLeft);
        al.deallocate(newLeft, 1);
        al.destroy(newRight);
        al.deallocate(newRight, 1);

        return *this;
    }

    template<typename T, typename Allocator>
    BSTNode<T, Allocator>& BSTNode<T, Allocator>::operator=(BSTNode<T, Allocator> &&input) noexcept {
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

        al.destroy(newFather);
        al.deallocate(newFather, 1);
        al.destroy(newLeft);
        al.deallocate(newLeft, 1);
        al.destroy(newRight);
        al.deallocate(newRight, 1);

        return *this;
    }

    template<typename T, typename Allocator>
    typename BSTNode<T, Allocator>::value_type BSTNode<T, Allocator>::operator[](const size_t &id) {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, typename Allocator>
    typename BSTNode<T, Allocator>::value_type BSTNode<T, Allocator>::operator[](const size_t &id) const {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, typename Allocator>
    inline BSTNode<T, Allocator>* BSTNode<T, Allocator>::node_Successor() {
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

    template<typename T, typename Allocator>
    inline BSTNode<T, Allocator>* BSTNode<T, Allocator>::node_Predecessor() {
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

    template<typename T, typename Allocator>
    [[maybe_unused]] inline BSTNode<T, Allocator>* BSTNode<T, Allocator>::node_Sibling() {
        if (this != nullptr) return (father == nullptr ? nullptr : (is_left_son() ? father->right : father->left));
        else return nullptr;
    }

}

#endif //TREES_BST_NODE_HPP
