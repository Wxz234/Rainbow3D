#pragma once

namespace Rainbow3D {
	class RObject {
	public:
		RObject() {}
		virtual ~RObject() {}
		RObject(const RObject&) = default;
		RObject& operator=(const RObject&) = default;
		RObject(RObject&&) noexcept = default;
		RObject& operator=(RObject&&) noexcept = default;
	};

	void DestroyRObject(RObject* obj);
}