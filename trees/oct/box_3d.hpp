//
// Created by PC on 18.11.2022.
//

#ifndef TREES_BOX_3D_HPP
#define TREES_BOX_3D_HPP

#pragma once

#include "point_3d.hpp"

namespace tree::oct {
    template<typename T, typename PointType = Vec3D<T>>
    struct Box3D {
        PointType center = {0, 0, 0};
        PointType radius = {1, 1, 1};

        Box3D() = default;
        Box3D(const PointType& center, const PointType& radius) : center(center), radius(radius){};
        Box3D(const PointType& center, const T& radius) : center(center), radius{radius, radius, radius}{};
        Box3D(const T& center, const PointType& radius) : center{center, center, center}, radius(radius){};
        Box3D(const T& center, const T& radius) : center{center, center, center}, radius{radius, radius, radius}{};
        ~Box3D() = default;

        bool ContainsPoint(const PointType &point);
        bool IntersectsBox3D(const PointType &other);
    };

    template<typename T, typename PointType>
    bool Box3D<T, PointType>::ContainsPoint(const PointType &point) {
        Vec3D dx = PointType::Abs(point - center);

        return (dx.x <= radius.x) && (dx.y <= radius.y) && (dx.z <= radius.z);
    }

    template<typename T, typename PointType>
    bool Box3D<T, PointType>::IntersectsBox3D(const PointType &other) {
        Vec3D dx = PointType::Abs(other.center - center);

        return (dx.x < (radius.x+other.radius.x)) && (dx.y < (radius.y + other.radius.y)) && (dx.z < (radius.z + other.radius.z));
    }
}

#endif //TREES_BOX_3D_HPP
