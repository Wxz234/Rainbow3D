#pragma once
#include <fstream>

namespace Rainbow3D {
	class JsonObject final {
	public:
		JsonObject() noexcept {}
		~JsonObject() {}
		JsonObject(const JsonObject&) = default;
		JsonObject(JsonObject&&) noexcept = default;
		JsonObject& operator=(const JsonObject&) = default;
		JsonObject& operator=(JsonObject&&) noexcept = default;
	private:

	};

	std::ifstream& operator>>(std::ifstream& i, JsonObject& j);

}