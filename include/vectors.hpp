//
// Created by Rodrigo Llanes on 01/10/2023.
//

#ifndef GRAPHIC_ENGINE_VECTORS_HPP
#define GRAPHIC_ENGINE_VECTORS_HPP

template <typename T>
class Vector2 {
public:
    T x, y;
    Vector2() :x(0), y(0) {};
    Vector2(T x, T y) : x(x), y(y) {};
};

template <typename T>
class Vector3 : public Vector2<T> {
public:
    T z;
    Vector3() : Vector2<T>(), z(0) {};
    Vector3(T x, T y, T z) : Vector2<T>(x, y), z(z) {};
};

template <typename T>
class Vector4 : public Vector3<T> {
public:
    T w;
    Vector4() : Vector3<T>(), w(0) {};
    Vector4(T x, T y, T z, T w) : Vector3<T>(x, y, z), w(w) {};
};

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

#endif //GRAPHIC_ENGINE_VECTORS_HPP
