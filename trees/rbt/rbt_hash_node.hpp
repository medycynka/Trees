//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BT_HASH_NODE_HPP
#define TREES_BT_HASH_NODE_HPP

#pragma once

#include <iostream>
#include <utility>
#include <memory_resource>
#include "../bst/exceptions.hpp"

namespace tree::rbt::node_impl {

    enum colors { red, black };

    template <typename T, std::size_t Mask = std::numeric_limits<std::size_t>::max(), typename Allocator = std::pmr::polymorphic_allocator<T>, typename Hash = std::hash<T>>
    class RBHashNode {
    public:
        typedef Allocator                            allocator;
        typedef Hash                                 hash_type;
        typedef T                                    value_type;
        typedef T*                                   pointer;
        typedef const T*                             const_pointer;
        typedef T&                                   reference;
        typedef const T&                             const_reference;
        typedef std::size_t                          hash_value_type;
        typedef RBHashNode<T, Mask, Allocator, Hash> self_type;

    public:
        value_type                            key;
        RBHashNode<T, Mask, Allocator, Hash>* father;
        RBHashNode<T, Mask, Allocator, Hash>* left;
        RBHashNode<T, Mask, Allocator, Hash>* right;
        int                                   color;
        allocator                             al{};
        hash_type                             hash_func{};

    private:
        hash_value_type _hash_key{};

    public:
        constexpr RBHashNode()                                                                               : father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ black } {};
        constexpr explicit RBHashNode(value_type input)                                                      : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ red }, _hash_key{hash_func(input) % Mask} {};
        constexpr explicit RBHashNode(Allocator& alloc)                                                      : father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ red }, al{alloc} {};
        constexpr RBHashNode(const RBHashNode<T, Mask, Allocator, Hash>& s, Allocator& alloc)                : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, color{ s.color }, al{alloc} {};
        constexpr RBHashNode(const RBHashNode<T, Mask, Allocator, Hash>& s)                                  : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, color{ s.color }, al{s.al.select_on_container_copy_construction()} {};
        constexpr RBHashNode(RBHashNode<T, Mask, Allocator, Hash>&& s) noexcept                              : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, color{ std::move(s.color) }, al{std::move(s.al)} {};
        constexpr RBHashNode(RBHashNode<T, Mask, Allocator, Hash>&& s, Allocator& alloc) noexcept            : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, color{ std::move(s.color) }, al{alloc} {};
        constexpr RBHashNode(value_type input, RBHashNode<T, Mask, Allocator, Hash>* father_)                : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr }, color{ red }, _hash_key{hash_func(input) % Mask} {};
        constexpr RBHashNode(value_type input, RBHashNode<T, Mask, Allocator, Hash>* father_, int new_color) : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr }, color{ new_color }, _hash_key{hash_func(input) % Mask} {};
        ~RBHashNode() = default;

        RBHashNode<T, Mask, Allocator, Hash>& operator= (const_reference input)                                    { key = input; return *this; };
        RBHashNode<T, Mask, Allocator, Hash>& operator= (const RBHashNode<T, Mask, Allocator, Hash>& input);
        RBHashNode<T, Mask, Allocator, Hash>& operator= (RBHashNode<T, Mask, Allocator, Hash>&& input) noexcept;
        constexpr bool                        operator==(const_reference input)                              const { return key == input; };
        constexpr bool                        operator!=(const_reference input)                              const { return key != input; };
        constexpr bool                        operator> (const_reference input)                              const { return key > input; };
        constexpr bool                        operator< (const_reference input)                              const { return key < input; };
        constexpr bool                        operator>=(const_reference input)                              const { return key >= input; };
        constexpr bool                        operator<=(const_reference input)                              const { return key <= input; };
        constexpr bool                        operator==(const RBHashNode<T, Mask, Allocator, Hash>& source) const { return (_hash_key == source.get_hash_key() && father == source.father); };
        constexpr bool                        operator!=(const RBHashNode<T, Mask, Allocator, Hash>& source) const { return !(*this == source); };
        constexpr bool                        operator> (const RBHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key > source.get_hash_key(); };
        constexpr bool                        operator< (const RBHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key < source.get_hash_key(); };
        constexpr bool                        operator>=(const RBHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key >= source.get_hash_key(); };
        constexpr bool                        operator<=(const RBHashNode<T, Mask, Allocator, Hash>& source) const { return _hash_key <= source.get_hash_key(); };
        constexpr explicit                    operator bool()                                                const { return (father != nullptr ? true : (left != nullptr ? true : right != nullptr)); };
        constexpr value_type                  operator[](const size_t& id);
        constexpr value_type                  operator[](const size_t& id) const;

        friend constexpr std::ostream& operator<<(std::ostream& ofs, const RBHashNode<T, Mask, Allocator, Hash>* pt) {
            ofs << "Key: " << pt->key << ", color: " << (pt->color == black ? "B" : "R") << "\n";
            if (pt->father != nullptr) ofs << "(Father) key: " << pt->father->key << ", color: " << (pt->father->color == black ? "B" : "R") << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt->left != nullptr) ofs << "(Left son) key: " << pt->left->key << ", color: " << (pt->left->color == black ? "B" : "R") << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt->right != nullptr) ofs << "(Right son) key: " << pt->right->key << ", color: " << (pt->right->color == black ? "B" : "R") << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        friend constexpr std::ostream& operator<<(std::ostream& ofs, const RBHashNode<T, Mask, Allocator, Hash>& pt) {
            ofs << "Key: " << pt.key << ", color: " << (pt.color == black ? "B" : "R") << "\n";
            if (pt.father != nullptr) ofs << "(Father) key: " << pt.father->key << ", color: " << (pt.father->color == black ? "B" : "R") << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt.left != nullptr) ofs << "(Left son) key: " << pt.left->key << ", color: " << (pt.left->color == black ? "B" : "R") << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt.right != nullptr) ofs << "(Right son) key: " << pt.right->key << ", color: " << (pt.right->color == black ? "B" : "R") << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        [[nodiscard]] constexpr inline hash_value_type  get_hash_key() const { return _hash_key; };
        constexpr inline void                           set_hash_key() const { _hash_key = hash_func(key) % Mask; };
        [[nodiscard]] constexpr bool                    is_left_son()  const { return ((father != nullptr) && father->left == this); };
        [[nodiscard]] constexpr bool                    is_right_son() const { return ((father != nullptr) && father->right == this); };
        constexpr RBHashNode<T, Mask, Allocator, Hash>* max_node()           { return ((right == nullptr) ? this : right->max_node()); };
        constexpr RBHashNode<T, Mask, Allocator, Hash>* min_node()           { return ((left == nullptr) ? this : left->min_node()); };
        constexpr allocator                             get_allocator()      { return al; };
        constexpr hash_type                             get_hash_func()      { return hash_func; };
        constexpr RBHashNode<T, Mask, Allocator, Hash>* node_Successor();
        constexpr RBHashNode<T, Mask, Allocator, Hash>* node_Predecessor();
    };

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    RBHashNode<T, Mask, Allocator, Hash>& RBHashNode<T, Mask, Allocator, Hash>::operator=(const RBHashNode<T, Mask, Allocator, Hash>& input) {
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
        color = input.color;
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
    RBHashNode<T, Mask, Allocator, Hash>& RBHashNode<T, Mask, Allocator, Hash>::operator=(RBHashNode<T, Mask, Allocator, Hash>&& input) noexcept {
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
        color = std::move(input.color);
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
    inline constexpr typename RBHashNode<T, Mask, Allocator, Hash>::value_type RBHashNode<T, Mask, Allocator, Hash>::operator[](const size_t& id) {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    inline constexpr typename RBHashNode<T, Mask, Allocator, Hash>::value_type RBHashNode<T, Mask, Allocator, Hash>::operator[](const size_t& id) const {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    // For in-oredr walk / increment in iterator
    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    inline constexpr RBHashNode<T, Mask, Allocator, Hash>* RBHashNode<T, Mask, Allocator, Hash>::node_Successor() {
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

    // For reverse in-oredr walk / decrement in iterator
    template<typename T, std::size_t Mask, typename Allocator, typename Hash>
    inline constexpr RBHashNode<T, Mask, Allocator, Hash>* RBHashNode<T, Mask, Allocator, Hash>::node_Predecessor() {
        if (this != nullptr) {
            if (left != nullptr) return left->max_node();
            else if (is_right_son()) return father;

            auto predeccesor = this;

            do {
                predeccesor = predeccesor->father;
            } while ((predeccesor != nullptr) && (predeccesor->is_left_son()));

            if (predeccesor != nullptr) return predeccesor->father;
            else return nullptr;
        }

        return nullptr;
    }

}

#endif //TREES_BT_HASH_NODE_HPP
