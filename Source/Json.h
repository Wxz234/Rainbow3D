#pragma once

namespace Rainbow3D {
	class Json final {
	public:
		Json() noexcept {}
		~Json() {}
		Json(const Json&) = default;
		Json(Json&&) noexcept = default;
		Json& operator=(const Json&) = default;
		Json& operator=(Json&&) noexcept = default;
	private:

	};
}