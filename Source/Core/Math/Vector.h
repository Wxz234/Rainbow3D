#pragma once

namespace Rainbow3D {
	class Vector2 {
	public:
		Vector2() :x(0.f), y(0.f) {}
		Vector2(float x, float y) :x(x), y(y) {}
		Vector2(const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator=(const Vector2&) = default;
		Vector2& operator=(Vector2&&) = default;
		float x, y;
	};

	class Vector3 {
	public:
		Vector3() :x(0.f), y(0.f), z(0.f) {}
		Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
		Vector3(const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator=(const Vector3&) = default;
		Vector3& operator=(Vector3&&) = default;
		float x, y, z;
	};

	class Vector4 {
	public:
		Vector4() :x(0.f), y(0.f), z(0.f), w(0.f) {}
		Vector4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
		Vector4(const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator=(const Vector4&) = default;
		Vector4& operator=(Vector4&&) = default;
		float x, y, z, w;
	};
}