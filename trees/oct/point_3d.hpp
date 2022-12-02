//
// Created by PC on 18.11.2022.
//

#ifndef TREES_POINT_3D_HPP
#define TREES_POINT_3D_HPP

#pragma once

#include <iostream>
#include <cmath>

namespace tree::oct {

    template<typename T>
    struct Vec3D {
    public:
        T x;
        T y;
        T z;

        Vec3D() : x{T(0)}, y{T(0)}, z{T(0)}{};
        Vec3D(const T& x, const T& y, const T& z) : x(x), y(y), z(z){};
        ~Vec3D() = default;

        Vec3D<T> &operator+=(const Vec3D &v);
        Vec3D<T> &operator+=(const T &scalar);
        Vec3D<T> &operator-=(const Vec3D &v);
        Vec3D<T> &operator-=(const T &scalar);
        template<typename ScalarType>
        Vec3D<T> &operator*=(ScalarType scalar);
        template<typename ScalarType>
        Vec3D<T> &operator/=(ScalarType scalar);

        double length();
        double lengthSquare();
        double angle(const Vec3D &v2);
        void abs() { this->x = std::abs(this->x); this->y = std::abs(this->y); this->z = std::abs(this->z);}
        Vec3D<T> r_abs() const { return Vec3D<T>(std::abs(this->x), std::abs(this->y), std::abs(this->z)); }
        void PN_normalization();
        void normalize();

        static double angle(const Vec3D<T> &v1, const Vec3D<T> &v2);
        static Vec3D<T> Cross(const Vec3D<T> &v1, const Vec3D<T> &v2);
        static Vec3D<T> Dot(const Vec3D<T> &v1, const Vec3D<T> &v2);
        static Vec3D<T> Abs(const Vec3D<T> &v1){ return v1.r_abs(); }
    };

    template<typename T>
    Vec3D<T> &Vec3D<T>::operator+=(const Vec3D &v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;

        return *this;
    }

    template<typename T>
    Vec3D<T> &Vec3D<T>::operator+=(const T &scalar) {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;

        return *this;
    }

    template<typename T>
    Vec3D<T> &Vec3D<T>::operator-=(const Vec3D &v) {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;

        return *this;
    }

    template<typename T>
    Vec3D<T> &Vec3D<T>::operator-=(const T &scalar) {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;

        return *this;
    }

    template<typename T>
    template<typename ScalarType>
    Vec3D<T> &Vec3D<T>::operator*=(ScalarType scalar) {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;

        return *this;
    }

    template<typename T>
    template<typename ScalarType>
    Vec3D<T> &Vec3D<T>::operator/=(ScalarType scalar) {
        this->x += scalar;
        this->y += scalar;
        this->z += scalar;

        return *this;
    }

    template<typename T>
    Vec3D<T> operator+(const Vec3D<T> &v1, const Vec3D<T> &v2){
        return Vec3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    template<typename T>
    Vec3D<T> operator-(const Vec3D<T> &v1, const Vec3D<T> &v2){
        return Vec3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    template<typename T>
    double operator*(const Vec3D<T> &v1, const Vec3D<T> &v2) {
        return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    }

    template<typename T, typename ScalarType>
    Vec3D<T> operator*(const Vec3D<T> &v1, ScalarType scalar) {
        return Vec3D(T(v1.x * scalar), T(v1.y * scalar), T(v1.z * scalar));
    }

    template<typename T, typename ScalarType>
    Vec3D<T> operator*(ScalarType scalar, const Vec3D<T> &v1) {
        return Vec3D(T(v1.x * scalar), T(v1.y * scalar), T(v1.z * scalar));
    }

    template<typename T>
    Vec3D<T> operator/(const Vec3D<T> &v1, float scalar) {
        return Vec3D(T(v1.x / scalar), T(v1.y / scalar), T(v1.z / scalar));
    }

    template<typename T>
    double Vec3D<T>::length() {
        return std::sqrt(x * x + y * y + z * z);
    }

    template<typename T>
    double Vec3D<T>::lengthSquare() {
        return x * x + y * y + z * z;
    }

    template<typename T>
    double Vec3D<T>::angle(const Vec3D<T> &v2) {
        return std::acos(*this * v2 / (this->length() * v2.length()));
    }

    template<typename T>
    void Vec3D<T>::PN_normalization() {
        x *= std::sqrt(1.0f - y * y / 2.0f - z * z / 2.0f + y * y * z * z / 3.0f);
        y *= std::sqrt(1.0f - x * x / 2.0f - z * z / 2.0f + x * x * z * z / 3.0f);
        z *= std::sqrt(1.0f - y * y / 2.0f - x * x / 2.0f + y * y * x * x / 3.0f);
    }

    template<typename T>
    void Vec3D<T>::normalize() {
        double length = this->length();

        x /= length;
        y /= length;
        z /= length;
    }

    template<typename T>
    double Vec3D<T>::angle(const Vec3D<T> &v1, const Vec3D<T> &v2) {
        return std::acos(v1 * v2 / (v1.length() * v2.length()));
    }

    template<typename T>
    Vec3D<T> Vec3D<T>::Cross(const Vec3D<T> &v1, const Vec3D<T> &v2) {
        return Vec3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template<typename T>
    Vec3D<T> Vec3D<T>::Dot(const Vec3D<T> &v1, const Vec3D<T> &v2) {
        return v1 * v2;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &stream, const Vec3D<T> &vec){
        stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

        return stream;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &stream, Vec3D<T> *vec){
        stream << "(" << vec->x << ", " << vec->y << ", " << vec->z << ")";

        return stream;
    }

}

#endif //TREES_POINT_3D_HPP
