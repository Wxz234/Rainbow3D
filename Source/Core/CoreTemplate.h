#pragma once

namespace Rainbow3D {

	template <typename T>
	struct TIdentity
	{
		typedef T Type;
	};

	template <typename T>
	inline T ImplicitConv(typename TIdentity<T>::Type Obj)
	{
		return Obj;
	}
}