//
// Created by PC on 16.11.2022.
//

#ifndef TREES_BOUNDING_BOX_HPP
#define TREES_BOUNDING_BOX_HPP

#pragma once

#include "pointd_2d.hpp"

namespace tree::quad
{

    template<typename T>
    class Box
    {
    public:
        typedef T          value_type;
        typedef T*         pointer;
        typedef const T*   const_pointer;
        typedef T&         reference;
        typedef const T&   const_reference;
        typedef Box<T> self_type;

        constexpr Box() noexcept : left(0), top(0), width(0), height(0){}
        constexpr explicit Box(value_type Left, value_type Top, value_type Width, value_type Height) noexcept : left(Left), top(Top), width(Width), height(Height){}
        constexpr Box(const Point2D<T>& position, const Point2D<T>& size) noexcept : left(position.x), top(position.y), width(size.x), height(size.y){}
        constexpr Box(const Box<T>& box) noexcept : left(box.left), top(box.top), width(box.width), height(box.height){}
        ~Box() = default;

        constexpr T getRight() const noexcept{
            return left + width;
        }

        constexpr T getBottom() const noexcept{
            return top + height;
        }

        constexpr Point2D<T> getTopLeft() const noexcept{
            return Point2D<T>(left, top);
        }

        constexpr Point2D<T> getCenter() const noexcept
        {
            return Point2D<T>(left + width / 2, top + height / 2);
        }

        constexpr Point2D<T> getSize() const noexcept
        {
            return Point2D<T>(width, height);
        }

        constexpr bool contains(const Box<T>& box) const noexcept
        {
            return left <= box.left && box.getRight() <= getRight() && top <= box.top && box.getBottom() <= getBottom();
        }

        constexpr bool intersects(const Box<T>& box) const noexcept
        {
            return !(left >= box.getRight() || getRight() <= box.left || top >= box.getBottom() || getBottom() <= box.top);
        }

        friend std::ostream& operator<<(std::ostream& ofs, const Box<T>* pt) {
            ofs << "[" << pt->getCenter() << ", " << pt->getSize() << "]";

            return ofs;
        };

        friend std::ostream& operator<<(std::ostream& ofs, const Box<T>& ref) {
            ofs << "[" << ref.getCenter() << ", " << ref.getSize() << "]";

            return ofs;
        };

    public:
        value_type left;
        value_type top;
        value_type width;
        value_type height;
    };

}

#endif //TREES_BOUNDING_BOX_HPP
