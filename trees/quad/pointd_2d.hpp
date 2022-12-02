//
// Created by PC on 16.11.2022.
//

#ifndef TREES_POINTD_2D_HPP
#define TREES_POINTD_2D_HPP

#pragma once

#include <iostream>
#include <utility>

namespace tree::quad{

    template<typename T>
    class Point2D {
    public:
        typedef T          value_type;
        typedef T*         pointer;
        typedef const T*   const_pointer;
        typedef T&         reference;
        typedef const T&   const_reference;
        typedef Point2D<T> self_type;

        Point2D() noexcept : x(0), y(0){}
        constexpr explicit Point2D(value_type X = 0, value_type Y = 0) noexcept : x(X), y(Y){}
        Point2D(const Point2D<T>& source) : x(source.x), y(source.y){}
        Point2D(Point2D<T>&& source) noexcept : x(std::move(source.x)), y(std::move(source.y)){}
        ~Point2D() = default;

        constexpr Point2D<T>& operator+=(const Point2D<T>& other) noexcept{
            x += other.x;
            y += other.y;

            return *this;
        }

        constexpr Point2D<T>& operator-=(const Point2D<T>& other) noexcept{
            x -= other.x;
            y -= other.y;

            return *this;
        }

        constexpr Point2D<T>& operator*=(value_type t) noexcept{
            x *= t;
            y *= t;

            return *this;
        }

        constexpr Point2D<T>& operator/=(value_type t) noexcept{
            x /= t;
            y /= t;

            return *this;
        }

        friend std::ostream& operator<<(std::ostream& ofs, const Point2D<T>* pt) {
            ofs << "(" << pt->x << ", " << pt->y << ")";

            return ofs;
        };

        friend std::ostream& operator<<(std::ostream& ofs, const Point2D<T>& ref) {
            ofs << "(" << ref.x << ", " << ref.y << ")";

            return ofs;
        };

    public:
        value_type x;
        value_type y;
    };

    template<typename T>
    constexpr Point2D<T> operator+(Point2D<T> lhs, const Point2D<T>& rhs) noexcept{
        lhs += rhs;

        return lhs;
    }

    template<typename T>
    constexpr Point2D<T> operator-(Point2D<T> lhs, const Point2D<T>& rhs) noexcept{
        lhs -= rhs;

        return lhs;
    }

    template<typename T>
    constexpr Point2D<T> operator*(Point2D<T> vec, T t) noexcept{
        vec *= t;

        return vec;
    }

    template<typename T>
    constexpr Point2D<T> operator/(Point2D<T> vec, T t) noexcept{
        vec /= t;

        return vec;
    }

}

#endif //TREES_POINTD_2D_HPP
