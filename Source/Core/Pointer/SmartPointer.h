#pragma once
#include <memory>

namespace Rainbow3D {

	template <class T, class Deleter = std::default_delete<T>>
	class UniquePtr : private std::unique_ptr<T, Deleter> {
	public:
		UniquePtr() {}

	private:
	};



}