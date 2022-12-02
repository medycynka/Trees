//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BST_BUCKET_NODE_HPP
#define TREES_BST_BUCKET_NODE_HPP

#pragma once

#include <iostream>
#include <memory_resource>
#include "exceptions.hpp"

namespace tree::bst::node_impl {

    template <typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
    class BSTBucketNode {
    public:
        typedef Allocator                   allocator;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef const T*                    const_pointer;
        typedef T&                          reference;
        typedef const T&                    const_reference;
        typedef std::size_t                 bucket_size_type;
        typedef BSTBucketNode<T, Allocator> self_type;

        value_type                   key;
        BSTBucketNode<T, Allocator>* father;
        BSTBucketNode<T, Allocator>* left;
        BSTBucketNode<T, Allocator>* right;
        allocator                    al{};
        bucket_size_type             bucket_size{1};
        bucket_size_type             bucket_index{1};

        BSTBucketNode()                                                           : father{ nullptr }, left{ nullptr }, right{ nullptr } {};
        explicit BSTBucketNode(Allocator& alloc)                                  : father{ nullptr }, left{ nullptr }, right{ nullptr }, al{alloc} {};
        explicit BSTBucketNode(value_type input)                                  : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr } {};
        explicit BSTBucketNode(value_type input, bucket_size_type buckets)        : key{ input }, father{ nullptr }, left{ nullptr }, right{ nullptr }, bucket_size{buckets} {};
        BSTBucketNode(const BSTBucketNode<T, Allocator>& s)                       : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, al{s.al.select_on_container_copy_construction()}, bucket_size{s.bucket_size}, bucket_index{s.bucket_index} {};
        BSTBucketNode(const BSTBucketNode<T, Allocator>& s, Allocator& alloc)     : key{ s.key }, father{ s.father }, left{ s.left }, right{ s.right }, al{alloc}, bucket_size{s.bucket_size}, bucket_index{s.bucket_index} {};
        BSTBucketNode(BSTBucketNode<T, Allocator>&& s) noexcept                   : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, al{std::move(s.al)}, bucket_size{std::move(s.bucket_size)}, bucket_index{std::move(s.bucket_index)} {};
        BSTBucketNode(BSTBucketNode<T, Allocator>&& s, Allocator& alloc) noexcept : key{ std::move(s.key) }, father{ std::move(s.father) }, left{ std::move(s.left) }, right{ std::move(s.right) }, al{alloc}, bucket_size{std::move(s.bucket_size)}, bucket_index{std::move(s.bucket_index)} {};
        BSTBucketNode(value_type input, BSTBucketNode<T, Allocator>* father_)     : key{ input }, father{ father_ }, left{ nullptr }, right{ nullptr } {};
        ~BSTBucketNode() = default;

        BSTBucketNode<T, Allocator>& operator= (const_reference input)                     { key = input; return *this; };
        BSTBucketNode<T, Allocator>& operator= (const BSTBucketNode<T, Allocator>& input);
        BSTBucketNode<T, Allocator>& operator= (BSTBucketNode<T, Allocator>&& input) noexcept;
        bool                   operator==(const_reference input)               const { return key == input; };
        bool                   operator!=(const_reference input)               const { return key != input; };
        bool                   operator> (const_reference input)               const { return key > input; };
        bool                   operator< (const_reference input)               const { return key < input; };
        bool                   operator>=(const_reference input)               const { return key >= input; };
        bool                   operator<=(const_reference input)               const { return key <= input; };
        bool                   operator==(const BSTBucketNode<T, Allocator>& source) const { return (key == source.key && bucket_size == source.bucket_size && bucket_index == source.bucket_index && father == source.father); };
        bool                   operator!=(const BSTBucketNode<T, Allocator>& source) const { return (!(*this == source)); };
        bool                   operator> (const BSTBucketNode<T, Allocator>& source) const { return key > source.key; };
        bool                   operator< (const BSTBucketNode<T, Allocator>& source) const { return key < source.key; };
        bool                   operator>=(const BSTBucketNode<T, Allocator>& source) const { return key >= source.key; };
        bool                   operator<=(const BSTBucketNode<T, Allocator>& source) const { return key <= source.key; };
        explicit               operator bool()                                 const { return (father != nullptr ? true : (left != nullptr ? true : right != nullptr)); };
        value_type             operator[](const size_t& id);
        value_type             operator[](const size_t& id) const;

        friend std::ostream& operator<<(std::ostream& ofs, const BSTBucketNode<T, Allocator>* pt) {
            if (pt->bucket_size < 2) {
                ofs << "Key: " << pt->key << "\n";
            } else{
                ofs << "Keys: ";
                for (auto i = 0; i < pt->bucket_size - 1; ++i) ofs << pt->key << ", ";
                ofs << pt->key << "\n";
            }
            if (pt->father != nullptr) ofs << "(Father) key: " << pt->father->key << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt->left != nullptr) ofs << "(Left son) key: " << pt->left->key << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt->right != nullptr) ofs << "(Right son) key: " << pt->right->key << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        friend std::ostream& operator<<(std::ostream& ofs, const BSTBucketNode<T, Allocator>& pt) {
            if (pt.bucket_size < 2) {
                ofs << "Key: " << pt.key << "\n";
            } else{
                ofs << "Keys: ";
                for (auto i = 0; i < pt.bucket_size - 1; ++i) ofs << pt.key << ", ";
                ofs << pt.key << "\n";
            }
            if (pt.father != nullptr) ofs << "(Father) key: " << pt.father->key << "\n";
            else ofs << "No father (root)" << "\n";
            if (pt.left != nullptr) ofs << "(Left son) key: " << pt.left->key << "\n";
            else ofs << "(No left son)" << "\n";
            if (pt.right != nullptr) ofs << "(Right son) key: " << pt.right->key << "\n\n";
            else ofs << "(No right son)" << "\n\n";

            return ofs;
        };

        [[nodiscard]] bool                       is_left_son()  const { return ((father != nullptr) && father->left == this); };
        [[nodiscard]] bool                       is_right_son() const { return ((father != nullptr) && father->right == this); };
        BSTBucketNode<T, Allocator>*             max_node()           { return ((right == nullptr) ? this : right->max_node()); };
        BSTBucketNode<T, Allocator>*             min_node()           { return ((left == nullptr) ? this : left->min_node()); };
        allocator                                get_allocator()      { return al; };
        [[maybe_unused]] inline bucket_size_type get_bucket_size()    { return bucket_size; }
        [[maybe_unused]] inline void             add_to_bucket()      { bucket_size++; }
        [[maybe_unused]] inline void             remove_from_bucket() { bucket_size--; if(bucket_size < 1) { bucket_size = 1; } }
        BSTBucketNode<T, Allocator>*             node_Successor();
        BSTBucketNode<T, Allocator>*             node_Predecessor();
    };

    template<typename T, typename Allocator>
    BSTBucketNode<T, Allocator>& BSTBucketNode<T, Allocator>::operator=(const BSTBucketNode<T, Allocator> &input) {
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
    BSTBucketNode<T, Allocator>& BSTBucketNode<T, Allocator>::operator=(BSTBucketNode<T, Allocator> &&input) noexcept {
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
    typename BSTBucketNode<T, Allocator>::value_type BSTBucketNode<T, Allocator>::operator[](const size_t &id) {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, typename Allocator>
    typename BSTBucketNode<T, Allocator>::value_type BSTBucketNode<T, Allocator>::operator[](const size_t &id) const {
        if (id < 0 || id > 3) throw tree::exception::NodeIndexOutOfBoundException();
        else if (id == 0) return this->key;
        else if (id == 1) return father->key;
        else if (id == 2) return left->key;
        else return right->key;
    }

    template<typename T, typename Allocator>
    inline BSTBucketNode<T, Allocator>* BSTBucketNode<T, Allocator>::node_Successor() {
        if (this != nullptr) {
            if (this->bucket_index < this->bucket_size){
                this->bucket_index++;

                return this;
            } else this->bucket_index = 1;

            if (this->right != nullptr) return this->right->min_node();
            else if (this->is_left_son()) return this->father;

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
    inline BSTBucketNode<T, Allocator>* BSTBucketNode<T, Allocator>::node_Predecessor() {
        if (this != nullptr) {
            if (this->bucket_index < this->bucket_size){
                this->bucket_index++;

                return this;
            } else this->bucket_index = 1;

            if (this->left != nullptr) return this->left->max_node();
            else if (this->is_right_son()) return this->father;

            auto predecessor = this;

            do {
                predecessor = predecessor->father;
            } while ((predecessor != nullptr) && (predecessor->is_left_son()));

            if (predecessor != nullptr) return predecessor->father;
            else return nullptr;
        }

        return nullptr;
    }

}

#endif //TREES_BST_BUCKET_NODE_HPP
