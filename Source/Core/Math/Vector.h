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
}