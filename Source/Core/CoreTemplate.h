#pragma once
#include <type_traits>
namespace Rainbow3D {
    template <class _Ty, class _Dx_noref, class = void>
    struct _Get_deleter_pointer_type { // provide fallback
        using type = _Ty*;
    };

    template <class _Ty, class _Dx_noref>
    struct _Get_deleter_pointer_type<_Ty, _Dx_noref, std::void_t<typename _Dx_noref::pointer>> { // get _Dx_noref::pointer
        using type = typename _Dx_noref::pointer;
    };

}