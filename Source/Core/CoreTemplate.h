#pragma once
#include <type_traits>

namespace Rainbow3D {
	template <typename _Ty, typename _Dx_noref, typename = void>
	struct _Get_deleter_pointer_type {
		using type = _Ty*;
	};

	template <typename _Ty, typename _Dx_noref>
	struct _Get_deleter_pointer_type<_Ty, _Dx_noref, std::void_t<typename _Dx_noref::pointer>> {
		using type = typename _Dx_noref::pointer;
	};

}