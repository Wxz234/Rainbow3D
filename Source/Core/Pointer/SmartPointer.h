#pragma once
#include "Core/CoreTemplate.h"
#include <memory>
#include <type_traits>
#include <cstddef>
#include <utility>

namespace Rainbow3D {
	template<typename T, typename D = std::default_delete<T>>
	class UniquePtr {
		using D_noref = std::remove_reference_t<D>;
	public:
		using deleter_type = D;
		using element_type = T;
		using pointer = typename _Get_deleter_pointer_type<T, D_noref>::type;

		UniquePtr(const UniquePtr&) = delete;

		template <typename = void>
		requires std::negation_v<std::is_pointer<D>> && std::is_default_constructible_v<D>
		constexpr UniquePtr() noexcept : _ptr(nullptr) , _d() {}

		template <typename = void>
		requires std::negation_v<std::is_pointer<D>> && std::is_default_constructible_v<D>
		constexpr UniquePtr(std::nullptr_t) noexcept : _ptr(nullptr), _d() {  }

		//disable CTAD
		template <typename = void>
		requires std::negation_v<std::is_pointer<D>> && std::is_default_constructible_v<D>
		explicit  UniquePtr(pointer p) noexcept : _ptr(p), _d() {}

		template <typename = void>
		requires std::is_lvalue_reference_v<D> && std::is_constructible_v<D, D>
		UniquePtr(pointer p, D d) noexcept : _ptr(p), _d(std::forward<decltype(d)>(d)) {}

		template <typename = void>
		requires std::is_lvalue_reference_v<D> && std::is_constructible_v<D, D_noref&&>
		UniquePtr(pointer, D_noref&&) = delete;

		template <typename = void>
		requires std::negation_v<std::is_lvalue_reference<D>> && std::is_constructible_v<D, const D&>
		UniquePtr(pointer p, const D& d) noexcept : _ptr(p), _d(std::forward<decltype(d)>(d)) {}

		template <typename = void>
		requires std::negation_v<std::is_lvalue_reference<D>> && std::is_constructible_v<D, D&&>
		UniquePtr(pointer p, D&& d) noexcept : _ptr(p), _d(std::forward<decltype(d)>(d)) {}

		template <typename = void>
		requires std::is_move_constructible_v<D>
		UniquePtr(UniquePtr&& r) noexcept : _ptr(r.Release()), _d(std::forward<D>(r._d)) {}

		template <typename U, typename D2>
		requires std::is_convertible_v<typename UniquePtr<U, D2>::pointer, pointer> && std::negation_v<std::is_pointer<U>> && std::conditional_t<std::is_reference_v<D>, std::is_same<D, D2>, std::is_convertible<D2, D>>::value
		UniquePtr(UniquePtr<U, D2>&& r) noexcept : _ptr(r.Release()), _d(std::forward<D>(r.GetDeleter())) {}

		~UniquePtr() {
			if (_ptr) {
				_d(_ptr);
			}
		}

		template <typename = void>
		requires std::is_move_assignable_v<D>
		UniquePtr& operator=(UniquePtr&& r) noexcept {
			Reset(r.Release());
			_d = std::forward<D>(r.GetDeleter());
		}

		template <typename U, typename D2>
		requires std::negation_v<std::is_array<U>> && std::is_assignable_v<D&, D2&&> && std::is_convertible_v<typename UniquePtr<U, D2>::pointer, pointer>
		UniquePtr& operator=(UniquePtr<U, D2>&& r) noexcept {
			Reset(r.Release());
			_d = std::forward<D>(r.GetDeleter());
		}

		UniquePtr& operator=(std::nullptr_t) noexcept {
			Reset();
			return *this;
		}

		explicit operator bool() const noexcept {
			return Get() != nullptr;
		}

		pointer operator->() const noexcept {
			return Get();
		}

		std::add_lvalue_reference<T>::type operator*() const noexcept(noexcept(*std::declval<pointer>())) {
			return *_ptr;
		}

		void Swap(UniquePtr& other) noexcept {
			UniquePtr temp = std::move(other);
			other = std::move(*this);
			*this = std::move(temp);
		}

		pointer Get() const noexcept {
			return _ptr;
		}

		D& GetDeleter() noexcept {
			return _d;
		}

		const D& GetDeleter() const noexcept {
			return _d;
		}

		pointer Release() noexcept {
			auto temp = _ptr;
			_ptr = nullptr;
			return temp;
		}

		void Reset(pointer _Ptr = nullptr) noexcept {
			pointer _Old = std::exchange(_ptr, _Ptr);
			if (_Old) {
				_d(_Old);
			}
		}

	private:
		pointer _ptr;
		D _d;
	};
}