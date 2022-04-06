#pragma once
namespace Rainbow3D {
	struct Vector2 {
		Vector2() { x = y = 0.f; }
		Vector2(float x, float y, float z) :x(x), y(y) {}
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) noexcept = default;
		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) noexcept = default;
		float x;
		float y;
	};

	struct Vector3 {
		Vector3() { x = y = z = 0.f; }
		Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
		Vector3(const Vector3&) = default;
		Vector3(Vector3&&) noexcept = default;
		Vector3& operator=(const Vector3&) = default;
		Vector3& operator=(Vector3&&) noexcept = default;
		float x;
		float y;
		float z;
	};
}