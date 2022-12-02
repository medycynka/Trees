//
// Created by PC on 23.11.2022.
//

#ifndef TREES_TREE_SORTING_HPP
#define TREES_TREE_SORTING_HPP

#pragma once

#include <vector>
#include "../rbt/rbt.hpp"
#include "../rbt/rbt_bucket.hpp"

namespace tree::algo::sorting{

    template <typename T, typename TreeType = tree::rbt::RBTree<T>>
    std::vector<T, std::pmr::polymorphic_allocator<T>> tree_unique_sort(std::initializer_list<T> ilist, bool reverse = false){
        TreeType sort_tree;

        for(auto &e : ilist){
            sort_tree.insert(e);
        }

        std::vector<T> ret_v(sort_tree.size());

        if (reverse){
            for(auto it = sort_tree.rbegin(); it != sort_tree.rend(); ++it){
                ret_v.push_back(*it);
            }
        } else{
            for (auto &n : sort_tree) {
                ret_v.push_back(n);
            }
        }

        return ret_v;
    }

    template <typename T, typename Collection, typename TreeType = tree::rbt::RBTree<T>>
    Collection tree_unique_sort(const Collection& collection, bool reverse = false){
        TreeType sort_tree;

        for(auto &e : collection){
            sort_tree.insert(e);
        }

        if (reverse){
            Collection ret_c(sort_tree.rbegin(), sort_tree.rend());

            return ret_c;
        } else {
            Collection ret_c(sort_tree.begin(), sort_tree.end());

            return ret_c;
        }
    }

    template <typename T, typename Collection, typename TreeType = tree::rbt::RBTree<T>>
    void tree_unique_sort(const Collection& collection, bool reverse = false){
        TreeType sort_tree;

        for(auto &e : collection){
            sort_tree.insert(e);
        }

        collection.clear();

        if (reverse){
            collection.insert(collection.cend(), sort_tree.rbegin(), sort_tree.rend());
        } else {
            collection.insert(collection.cend(), sort_tree.begin(), sort_tree.end());
        }
    }

    template <typename T, typename InputIt, typename TreeType = tree::rbt::RBTree<T>>
    std::vector<T, std::pmr::polymorphic_allocator<T>> tree_unique_sort(InputIt first, InputIt last, bool reverse = false){
        TreeType sort_tree;
        auto tmp = first;

        while(tmp != last){
            sort_tree.insert(*tmp);
            ++tmp;
        }

        std::vector<T> ret_v(sort_tree.size());

        if (reverse){
            for(auto it = sort_tree.rbegin(); it != sort_tree.rend(); ++it){
                ret_v.push_back(*it);
            }
        } else{
            for (auto &n : sort_tree) {
                ret_v.push_back(n);
            }
        }

        return ret_v;
    }

    template <typename T, typename TreeType = tree::rbt::RBBucketTree<T>>
    std::vector<T, std::pmr::polymorphic_allocator<T>> tree_sort(std::initializer_list<T> ilist, bool reverse = false){
        TreeType sort_tree;

        for(auto &e : ilist){
            sort_tree.insert(e);
        }

        std::vector<T> ret_v(sort_tree.size());

        if (reverse){
            for(auto it = sort_tree.rbegin(); it != sort_tree.rend(); ++it){
                ret_v.push_back(*it);
            }
        } else{
            for (auto &n : sort_tree) {
                ret_v.push_back(n);
            }
        }

        return ret_v;
    }

    template <typename T, typename Collection, typename TreeType = tree::rbt::RBBucketTree<T>>
    Collection tree_sort(const Collection& collection, bool reverse = false){
        TreeType sort_tree;

        for(auto &e : collection){
            sort_tree.insert(e);
        }

        if (reverse){
            Collection ret_c(sort_tree.rbegin(), sort_tree.rend());

            return ret_c;
        } else {
            Collection ret_c(sort_tree.begin(), sort_tree.end());

            return ret_c;
        }
    }

    template <typename T, typename Collection, typename TreeType = tree::rbt::RBBucketTree<T>>
    void tree_sort(const Collection& collection, bool reverse = false){
        TreeType sort_tree;

        for(auto &e : collection){
            sort_tree.insert(e);
        }

        collection.clear();

        if (reverse){
            collection.insert(collection.cend(), sort_tree.rbegin(), sort_tree.rend());
        } else {
            collection.insert(collection.cend(), sort_tree.begin(), sort_tree.end());
        }
    }

    template <typename T, typename InputIt, typename TreeType = tree::rbt::RBBucketTree<T>>
    std::vector<T, std::pmr::polymorphic_allocator<T>> tree_sort(InputIt first, InputIt last, bool reverse = false){
        TreeType sort_tree;
        auto tmp = first;

        while(tmp != last){
            sort_tree.insert(*tmp);
            ++tmp;
        }

        std::vector<T> ret_v(sort_tree.size());

        if (reverse){
            for(auto it = sort_tree.rbegin(); it != sort_tree.rend(); ++it){
                ret_v.push_back(*it);
            }
        } else{
            for (auto &n : sort_tree) {
                ret_v.push_back(n);
            }
        }

        return ret_v;
    }

}

#endif //TREES_TREE_SORTING_HPP
