#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"

#include <functional>
#include <utility>

namespace Rainbow3D {

	class WindowContext;

	class RWindow final {
	public:
		DISABLE_COPY_AND_ASSIGN(RWindow)

		RWindow(const wchar_t* title, uint32 w, uint32 h);
		~RWindow();

		WindowContext* GetContext() const noexcept {
			return _context;
		}

		template <typename _Fn, typename ..._Args>
		void Run(_Fn&& _Fx, _Args&&... _Ax) {
			auto func = std::bind(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...);
			_show();
			void* msg = _get_msg();
			while (!_is_quit(msg)) {
				if (_peek_msg(msg)) {
					_dispatch_msg(msg);
				}
				else {
					func();
				}
			}
			_destroy_msg(msg);
		}
		
	private:
		void _show();
		//msg
		void* _get_msg();
		bool _is_quit(void* msg);
		bool _peek_msg(void* msg);
		void _dispatch_msg(void* msg);
		void _destroy_msg(void* msg);

		WindowContext *_context;
	};

	RWindow* CreateRenderWindow(const wchar_t* title, uint32_t width, uint32_t height);
	void DestroyRenderWindow(RWindow* window);
}