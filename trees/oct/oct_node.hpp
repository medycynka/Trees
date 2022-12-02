//
// Created by PC on 18.11.2022.
//

#ifndef TREES_OCT_NODE_HPP
#define TREES_OCT_NODE_HPP

#pragma once

#include "box_3d.hpp"
#include "point_3d.hpp"

namespace tree::oct {
    constexpr static int OCTREE_NODE_CAPACITY = 32;

    template<typename T, typename PointType = Vec3D<T>, typename BoxType = Box3D<T, PointType>>
    struct OctNode {
    public:
        BoxType boundary;
        PointType points[OCTREE_NODE_CAPACITY];
        T data[OCTREE_NODE_CAPACITY];
        unsigned short element_count = 0;
        int first_child_index = -1;

        OctNode() : boundary(){};
        explicit OctNode(const BoxType &box) : boundary(box){};
    };

}

#endif //TREES_OCT_NODE_HPP
