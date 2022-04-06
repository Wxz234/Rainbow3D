#pragma once
namespace Rainbow3D {
	class Model {
	public:
		Model() {}
		Model(const Model&) = default;
		Model(Model&&) noexcept = default;
		Model& operator=(const Model&) = default;
		Model& operator=(Model&&) noexcept = default;
		virtual ~Model() {}
	};
}
