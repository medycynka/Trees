//
// Created by PC on 18.11.2022.
//

#ifndef TREES_OCT_TREE_HPP
#define TREES_OCT_TREE_HPP

#pragma once

#include <vector>
#include "oct_node.hpp"

namespace tree::oct {

    template<typename T, typename PointType = Vec3D<T>, typename BoxType = Box3D<T, PointType>>
    class OctTree {
    public:
        OctTree();
        explicit OctTree(const BoxType &box);

        bool insert(const PointType &point, const T &user_data);
        std::vector<T> query_range(const BoxType &range);
        void query_range(const BoxType &range, std::vector<T> &result, int node_idx);
        void clear() { nodes_.clear(); };

    private:
        bool InsertIntoNode(const PointType &point, const T &user_data, int node_idx);
        constexpr void SubdivideNode(int node_idx);

    private:
        std::vector<OctNode<T, PointType, BoxType>> nodes_;
    };

    template<typename T, typename PointType, typename BoxType>
    OctTree<T, PointType, BoxType>::OctTree() : nodes_() {
        nodes_.emplace_back();
    }

    template<typename T, typename PointType, typename BoxType>
    OctTree<T, PointType, BoxType>::OctTree(const BoxType &box) : nodes_() {
        nodes_.emplace_back(box);
    }

    template<typename T, typename PointType, typename BoxType>
    bool OctTree<T, PointType, BoxType>::insert(const PointType &point, const T &user_data) {
        return InsertIntoNode(point, user_data, 0);
    }

    template<typename T, typename PointType, typename BoxType>
    std::vector<T> OctTree<T, PointType, BoxType>::query_range(const BoxType &range) {
        std::vector<T> result;
        query_range(range, result, 0);

        return result;
    }

    template<typename T, typename PointType, typename BoxType>
    void OctTree<T, PointType, BoxType>::query_range(const BoxType &range, std::vector<T> &result, int node_idx) {
        if (!nodes_[node_idx].boundary.IntersectsBox3D(range)) return;

        for (int i = 0; i < nodes_[node_idx].element_count; i++) {
            if (range.ContainsPoint(nodes_[node_idx].points[i])) result.push_back(nodes_[node_idx].data[i]);
        }

        int firstChildIdx = nodes_[node_idx].first_child_index;
        
        if (firstChildIdx == -1) return;

        query_range(range, result, firstChildIdx + 0);
        query_range(range, result, firstChildIdx + 1);
        query_range(range, result, firstChildIdx + 2);
        query_range(range, result, firstChildIdx + 3);
        query_range(range, result, firstChildIdx + 4);
        query_range(range, result, firstChildIdx + 5);
        query_range(range, result, firstChildIdx + 6);
        query_range(range, result, firstChildIdx + 7);
    }

    template<typename T, typename PointType, typename BoxType>
    bool OctTree<T, PointType, BoxType>::InsertIntoNode(const PointType &point, const T &user_data, int node_idx) {
        if (!nodes_[node_idx].boundary.ContainsPoint(point)) return false;

        int nodeElementCount = nodes_[node_idx].element_count;
        if (nodeElementCount < OCTREE_NODE_CAPACITY) {
            nodes_[node_idx].points[nodeElementCount] = point;
            nodes_[node_idx].data[nodeElementCount] = user_data;
            nodes_[node_idx].element_count++;

            return true;
        }

        if (nodes_[node_idx].first_child_index == -1) SubdivideNode(node_idx);

        int child_idx = nodes_[node_idx].first_child_index;
        if (InsertIntoNode(point, user_data, child_idx + 0)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 1)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 2)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 3)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 4)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 5)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 6)) return true;
        if (InsertIntoNode(point, user_data, child_idx + 7)) return true;

        return false;
    }

    template<typename T, typename PointType, typename BoxType>
    constexpr void OctTree<T, PointType, BoxType>::SubdivideNode(int node_idx) {
        int firstChildIdx = nodes_.size();

        nodes_[node_idx].first_child_index = firstChildIdx;

        PointType center = nodes_[node_idx].boundary.center;
        PointType radius = nodes_[node_idx].boundary.radius;
        BoxType box;

        box.radius = radius / 2.0;

        for (auto i = 0; i < 8; i++) nodes_.emplace_back();

        box.center = PointType(center.x - radius.x / 2.0, center.y - radius.y / 2.0, center.z - radius.z / 2.0);
        nodes_[firstChildIdx + 0].boundary = box;
        box.center = PointType(center.x - radius.x / 2.0, center.y - radius.y / 2.0, center.z + radius.z / 2.0);
        nodes_[firstChildIdx + 1].boundary = box;
        box.center = PointType(center.x - radius.x / 2.0, center.y + radius.y / 2.0, center.z - radius.z / 2.0);
        nodes_[firstChildIdx + 2].boundary = box;
        box.center = PointType(center.x - radius.x / 2.0, center.y + radius.y / 2.0, center.z + radius.z / 2.0);
        nodes_[firstChildIdx + 3].boundary = box;
        box.center = PointType(center.x + radius.x / 2.0, center.y - radius.y / 2.0, center.z - radius.z / 2.0);
        nodes_[firstChildIdx + 4].boundary = box;
        box.center = PointType(center.x + radius.x / 2.0, center.y - radius.y / 2.0, center.z + radius.z / 2.0);
        nodes_[firstChildIdx + 5].boundary = box;
        box.center = PointType(center.x + radius.x / 2.0, center.y + radius.y / 2.0, center.z - radius.z / 2.0);
        nodes_[firstChildIdx + 6].boundary = box;
        box.center = PointType(center.x + radius.x / 2.0, center.y + radius.y / 2.0, center.z + radius.z / 2.0);
        nodes_[firstChildIdx + 7].boundary = box;
    }

}

#endif //TREES_OCT_TREE_HPP
