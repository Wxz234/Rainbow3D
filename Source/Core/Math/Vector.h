#pragma once
namespace Rainbow3D {
    struct Vector2
    {
        float x;
        float y;

        Vector2() { x = 0.f; y = 0.f; }

        Vector2(const Vector2&) = default;
        Vector2& operator=(const Vector2&) = default;

        Vector2(Vector2&&) = default;
        Vector2& operator=(Vector2&&) = default;

        constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}
        explicit Vector2(const float* pArray)  noexcept : x(pArray[0]), y(pArray[1]) {}
    };

    struct Vector3
    {
        float x;
        float y;
        float z;

        Vector3() { x = 0.f; y = 0.f; z = 0.f; }

        Vector3(const Vector3&) = default;
        Vector3& operator=(const Vector3&) = default;

        Vector3(Vector3&&) = default;
        Vector3& operator=(Vector3&&) = default;

        constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
        explicit Vector3(const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
    };
}