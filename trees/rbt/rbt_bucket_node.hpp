//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BT_BUCKET_NODE_HPP
#define TREES_BT_BUCKET_NODE_HPP

#pragma once

#include <iostream>
#include <utility>
#include <memory_resource>
#include "../bst/exceptions.hpp"

namespace tree::rbt::node_impl {

    template <typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    class RBBucketNode {
    public:
        typedef Allocator                  allocator;
        typedef T                          value_type;
        typedef T*                         pointer;
        typedef const T*                   const_pointer;
        typedef T&                         reference;
        typedef const T&                   const_reference;
        typedef std::size_t                bucket_size_type;
        typedef RBBucketNode<T, Allocator> self_type;

    public:
        value_type                  key;
        RBBucketNode<T, Allocator>* father;
        RBBucketNode<T, Allocator>* left;
        RBBucketNode<T, Allocator>* right;
        int                         color;
        allocator                   al{};
        bucket_size_type            bucket_size{1};
        bucket_size_type            bucket_index{1};

    public:
        constexpr RBBucketNode()                                                                     : father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ black } {};
        constexpr explicit RBBucketNode(value_type input)                                            : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ red } {};
        constexpr explicit RBBucketNode(value_type input, bucket_size_type buckets)                  : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ red }, bucket_size{buckets} {};
        constexpr explicit RBBucketNode(Allocator& alloc)                                            : father{ nullptr }, left{ nullptr }, right{ nullptr }, color{ red }, al{alloc} {};
        constexpr RBBucketNode(const RBBucketNode<T, Allocator>& s, Allocator& alloc)                : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, color{ s.color }, al{alloc}, bucket_size{s.bucket_size}, bucket_index{s.bucket_index} {};
        constexpr RBBucketNode(const RBBucketNode<T, Allocator>& s)                                  : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, color{ s.color }, al{s.al.select_on_container_copy_construction()}, bucket_size{s.bucket_size}, bucket_index{s.bucket_index} {};
        constexpr RBBucketNode(RBBucketNode<T, Allocator>&& s) noexcept                              : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, color{ std::move(s.color) }, al{std::move(s.al)}, bucket_size{std::move(s.bucket_size)}, bucket_index{std::move(s.bucket_index)} {};
        constexpr RBBucketNode(RBBucketNode<T, Allocator>&& s, Allocator& alloc) noexcept            : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, color{ std::move(s.color) }, al{alloc}, bucket_size{std::move(s.bucket_size)}, bucket_index{std::move(s.bucket_index)} {};
        constexpr RBBucketNode(value_type input, RBBucketNode<T, Allocator>* father_)                : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr }, color{ red } {};
        constexpr RBBucketNode(value_type input, RBBucketNode<T, Allocator>* father_, int new_color) : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr }, color{ new_color } {};
        ~RBBucketNode() = default;

        RBBucketNode<T, Allocator>& operator= (const_reference input)                          { key = input; return *this; };
        RBBucketNode<T, Allocator>& operator= (const RBBucketNode<T, Allocator>& input);
        RBBucketNode<T, Allocator>& operator= (RBBucketNode<T, Allocator>&& input) noexcept;
        constexpr bool              operator==(const_reference input)                    const { return key == input; };
        constexpr bool              operator!=(const_reference input)                    const { return key != input; };
        constexpr bool              operator> (const_reference input)                    const { return key > input; };
        constexpr bool              operator< (const_reference input)                    const { return key < input; };
        constexpr bool              operator>=(const_reference input)                    const { return key >= input; };
        constexpr bool              operator<=(const_reference input)                    const { return key <= input; };
        constexpr bool              operator==(const RBBucketNode<T, Allocator>& source) const { return (key == source.key && bucket_size == source.bucket_size && bucket_index == source.bucket_index && father == source.father); };
        constexpr bool              operator!=(const RBBucketNode<T, Allocator>& source) const { return !(*this == source); };
        constexpr bool              operator> (const RBBucketNode<T, Allocator>& source) const { return key > source.key; };
        constexpr bool              operator< (const RBBucketNode<T, Allocator>& source) const { return key < source.key; };
        constexpr bool              operator>=(const RBBucketNode<T, Allocator>& source) const { return key >= source.key; };
        constexpr bool              operator<=(const RBBucketNode<T, Allocator>& source) const { return key <= source.key; };
        constexpr explicit          operator bool()                                      const { return (father != nullptr ? true : (left != nullptr ? true : right != nullptr)); };
        constexpr value_type        operator[](const size_t& id);
        constexpr value_type        operator[](const size_t& id) const;

        friend constexpr std::ostream& operator<<(std::ostream& ofs, const RBBucketNode<T, Allocator>* pt) {
            if (pt->bucket_size < 2) ofs << "Key: " << pt->key << ", color: " << (pt->color == black ? "B" : "R") << "\n";
            else{
                ofs << "Keys: ";
                for (auto i = 0; i < pt->bucket_size - 1; ++i) ofs << pt->key << ", ";
                ofs << pt->key << ", color: " << (pt->color == black ? "B" : "R") << "\n";
            }
            if (pt->father != nullptr) ofs << "(Father) key: " << pt->father->key << ", color: " << (pt->father->color == black ? "B" : "R") << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt->left != nullptr) ofs << "(Left son) key: " << pt->left->key << ", color: " << (pt->left->color == black ? "B" : "R") << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt->right != nullptr) ofs << "(Right son) key: " << pt->right->key << ", color: " << (pt->right->color == black ? "B" : "R") << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        friend constexpr std::ostream& operator<<(std::ostream& ofs, const RBBucketNode<T, Allocator>& pt) {
            if (pt.bucket_size < 2) ofs << "Key: " << pt.key << ", color: " << (pt.color == black ? "B" : "R") << "\n";
            else{
                ofs << "Keys: ";
                for (auto i = 0; i < pt.bucket_size - 1; ++i) ofs << pt.key << ", ";
                ofs << pt.key << ", color: " << (pt.color == black ? "B" : "R") << "\n";
            }
            if (pt.father != nullptr) ofs << "(Father) key: " << pt.father->key << ", color: " << (pt.father->color == black ? "B" : "R") << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt.left != nullptr) ofs << "(Left son) key: " << pt.left->key << ", color: " << (pt.left->color == black ? "B" : "R") << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt.right != nullptr) ofs << "(Right son) key: " << pt.right->key << ", color: " << (pt.right->color == black ? "B" : "R") << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        [[nodiscard]] constexpr bool             is_left_son()  const { return ((father != nullptr) && father->left == this); };
        [[nodiscard]] constexpr bool             is_right_son() const { return ((father != nullptr) && father->right == this); };
        constexpr RBBucketNode<T, Allocator>*    max_node()           { return ((right == nullptr) ? this : right->max_node()); };
        constexpr RBBucketNode<T, Allocator>*    min_node()           { return ((left == nullptr) ? this : left->min_node()); };
        constexpr allocator                      get_allocator()                { return al; };
        [[maybe_unused]] inline bucket_size_type get_bucket_size()    { return bucket_size; }
        [[maybe_unused]] inline void             add_to_bucket()      { bucket_size++; }
        [[maybe_unused]] inline void             remove_from_bucket() { bucket_size--; if(bucket_size < 1) { bucket_size = 1; } }
        constexpr RBBucketNode<T, Allocator>*    node_Successor();
        constexpr RBBucketNode<T, Allocator>*    node_Predecessor();
    };

    template<typename T, typename Allocator>
    RBBucketNode<T, Allocator>& RBBucketNode<T, Allocator>::operator=(const RBBucketNode<T, Allocator>& input) {
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
        bucket_size = input.bucket_size;
        bucket_index = input.bucket_index;

        al.destroy(newFather);
        al.deallocate(newFather, 1);
        al.destroy(newLeft);
        al.deallocate(newLeft, 1);
        al.destroy(newRight);
        al.deallocate(newRight, 1);

        return *this;
    }

    template<typename T, typename Allocator>
    RBBucketNode<T, Allocator>& RBBucketNode<T, Allocator>::operator=(RBBucketNode<T, Allocator>&& input) noexcept {
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
        bucket_size = input.bucket_size;
        bucket_index = input.bucket_index;

        al.destroy(newFather);
        al.deallocate(newFather, 1);
        al.destroy(newLeft);
        al.deallocate(newLeft, 1);
        al.destroy(newRight);
        al.deallocate(newRight, 1);

        return *this;
    }

    template<typename T, typename Allocator>
    inline constexpr typename RBBucketNode<T, Allocator>::value_type RBBucketNode<T, Allocator>::operator[](const size_t& id) {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, typename Allocator>
    inline constexpr typename RBBucketNode<T, Allocator>::value_type RBBucketNode<T, Allocator>::operator[](const size_t& id) const {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    // For in-oredr walk / increment in iterator
    template<typename T, typename Allocator>
    inline constexpr RBBucketNode<T, Allocator>* RBBucketNode<T, Allocator>::node_Successor() {
        if (this != nullptr) {
            if (bucket_index < bucket_size){
                bucket_index++;

                return this;
            } else bucket_index = 1;

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
    template<typename T, typename Allocator>
    inline constexpr RBBucketNode<T, Allocator>* RBBucketNode<T, Allocator>::node_Predecessor() {
        if (this != nullptr) {
            if (bucket_index < bucket_size){
                bucket_index++;

                return this;
            } else bucket_index = 1;

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

#endif //TREES_BT_BUCKET_NODE_HPP
