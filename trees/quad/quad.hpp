//
// Created by PC on 16.11.2022.
//

#ifndef TREES_QUAD_HPP
#define TREES_QUAD_HPP

#pragma once

#include <cassert>
#include <algorithm>
#include <array>
#include <memory>
#include <type_traits>
#include <vector>
#include "bounding_box.hpp"

namespace tree::quad
{

    template<typename T, typename GetBox, typename Equal = std::equal_to<T>, typename Float = float>
    class Quadtree
    {
        static_assert(std::is_convertible_v<std::invoke_result_t<GetBox, const T&>, Box<Float>>,
        "GetBox must be a callable of signature Box<Float>(const T&)");
        static_assert(std::is_convertible_v<std::invoke_result_t<Equal, const T&, const T&>, bool>,
                      "Equal must be a callable of signature bool(const T&, const T&)");
        static_assert(std::is_arithmetic_v<Float>);

    public:
        explicit Quadtree(const Box<Float>& box, const GetBox& getBox = GetBox(), const Equal& equal = Equal()) : mBox(box), mRoot(std::make_unique<Node>()), mGetBox(getBox), mEqual(equal) {}

        void add(const T& value){
            add(mRoot.get(), 0, mBox, value);
        }

        void remove(const T& value){
            remove(mRoot.get(), mBox, value);
        }

        std::vector<T> query(const Box<Float>& box) const{
            auto values = std::vector<T>();
            query(mRoot.get(), mBox, box, values);
            return values;
        }

        std::vector<std::pair<T, T>> findAllIntersections() const{
            auto intersections = std::vector<std::pair<T, T>>();
            findAllIntersections(mRoot.get(), intersections);
            return intersections;
        }

    private:
        static constexpr auto Threshold = std::size_t(16);
        static constexpr auto MaxDepth = std::size_t(8);

        struct Node{
            std::array<std::unique_ptr<Node>, 4> children;
            std::vector<T> values;
        };

        Box<Float> mBox;
        std::unique_ptr<Node> mRoot;
        GetBox mGetBox;
        Equal mEqual;

        bool isLeaf(const Node* node) const{
            return !static_cast<bool>(node->children[0]);
        }

        Box<Float> computeBox(const Box<Float>& box, int i) const{
            auto origin = box.getTopLeft();
            auto childSize = box.getSize() / static_cast<Float>(2);

            switch (i){
                case 0:
                    return Box<Float>(origin, childSize);
                case 1:
                    return Box<Float>(Point2D<Float>(origin.x + childSize.x, origin.y), childSize);
                case 2:
                    return Box<Float>(Point2D<Float>(origin.x, origin.y + childSize.y), childSize);
                case 3:
                    return Box<Float>(origin + childSize, childSize);
                default:
                    return Box<Float>();
            }
        }

        int getQuadrant(const Box<Float>& nodeBox, const Box<Float>& valueBox) const{
            auto center = nodeBox.getCenter();

            if (valueBox.getRight() < center.x){
                if (valueBox.getBottom() < center.y) return 0;
                else if (valueBox.top >= center.y) return 2;
                else return -1;
            } else if (valueBox.left >= center.x) {
                if (valueBox.getBottom() < center.y) return 1;
                else if (valueBox.top >= center.y) return 3;
                else return -1;
            } else return -1;
        }

        void add(Node* node, std::size_t depth, const Box<Float>& box, const T& value) {
            if (node != nullptr && box.contains(mGetBox(value))) {
                if (isLeaf(node)) {
                    if (depth >= MaxDepth || node->values.size() < Threshold) node->values.push_back(value);
                    else {
                        split(node, box);
                        add(node, depth, box, value);
                    }
                } else {
                    auto i = getQuadrant(box, mGetBox(value));

                    if (i != -1)
                        add(node->children[static_cast<std::size_t>(i)].get(), depth + 1, computeBox(box, i), value);
                    else node->values.push_back(value);
                }
            }
        }

        void split(Node* node, const Box<Float>& box){
            if (node != nullptr && isLeaf(node)) {
                for (auto &child: node->children) child = std::make_unique<Node>();

                auto newValues = std::vector<T>();

                for (const auto &value: node->values) {
                    auto i = getQuadrant(box, mGetBox(value));

                    if (i != -1) node->children[static_cast<std::size_t>(i)]->values.push_back(value);
                    else newValues.push_back(value);
                }

                node->values = std::move(newValues);
            }
        }

        bool remove(Node* node, const Box<Float>& box, const T& value) {
            if (node != nullptr && box.contains(mGetBox(value))) {
                if (isLeaf(node)) {
                    removeValue(node, value);

                    return true;
                } else {
                    auto i = getQuadrant(box, mGetBox(value));

                    if (i != -1) {
                        if (remove(node->children[static_cast<std::size_t>(i)].get(), computeBox(box, i), value))
                            return tryMerge(node);
                    } else removeValue(node, value);

                    return false;
                }
            }
        }

        void removeValue(Node* node, const T& value) {
            auto it = std::find_if(std::begin(node->values), std::end(node->values),
                                   [this, &value](const auto &rhs) { return mEqual(value, rhs); });

            if (it != std::end(node->values)) {
                *it = std::move(node->values.back());
                node->values.pop_back();
            }
        }

        bool tryMerge(Node* node) {
            if (node != nullptr && !isLeaf(node)) {
                auto nbValues = node->values.size();

                for (const auto &child: node->children) {
                    if (!isLeaf(child.get())) return false;

                    nbValues += child->values.size();
                }
                if (nbValues <= Threshold) {
                    node->values.reserve(nbValues);

                    for (const auto &child: node->children) {
                        for (const auto &value: child->values) node->values.push_back(value);
                    }

                    for (auto &child: node->children) child.reset();

                    return true;
                } else return false;
            }
        }

        void query(Node* node, const Box<Float>& box, const Box<Float>& queryBox, std::vector<T>& values) const {
            if (node != nullptr && queryBox.intersects(box)) {
                for (const auto &value: node->values) {
                    if (queryBox.intersects(mGetBox(value))) values.push_back(value);
                }

                if (!isLeaf(node)) {
                    for (auto i = std::size_t(0); i < node->children.size(); ++i) {
                        auto childBox = computeBox(box, static_cast<int>(i));

                        if (queryBox.intersects(childBox)) query(node->children[i].get(), childBox, queryBox, values);
                    }
                }
            }
        }

        void findAllIntersections(Node* node, std::vector<std::pair<T, T>>& intersections) const {
            for (auto i = std::size_t(0); i < node->values.size(); ++i) {
                for (auto j = std::size_t(0); j < i; ++j) {
                    if (mGetBox(node->values[i]).intersects(mGetBox(node->values[j])))
                        intersections.emplace_back(node->values[i], node->values[j]);
                }
            }

            if (!isLeaf(node)) {
                for (const auto &child: node->children) {
                    for (const auto &value: node->values)
                        findIntersectionsInDescendants(child.get(), value, intersections);
                }

                for (const auto &child: node->children)
                    findAllIntersections(child.get(), intersections);
            }
        }

        void findIntersectionsInDescendants(Node* node, const T& value, std::vector<std::pair<T, T>>& intersections) const
        {
            // Test against the values stored in this node
            for (const auto& other : node->values)
            {
                if (mGetBox(value).intersects(mGetBox(other)))
                    intersections.emplace_back(value, other);
            }
            // Test against values stored into descendants of this node
            if (!isLeaf(node))
            {
                for (const auto& child : node->children)
                    findIntersectionsInDescendants(child.get(), value, intersections);
            }
        }
    };

}

#endif //TREES_QUAD_HPP
