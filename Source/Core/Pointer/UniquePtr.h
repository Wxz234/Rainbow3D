#pragma once
#include "Core/CoreTemplate.h"
#include <memory>
#include <type_traits>
#include <cstddef>
#include <utility>

namespace Rainbow3D {

	template<typename T, typename Deleter = std::default_delete<T>>
	class UniquePtr {
	public:
		using deleter_type = Deleter;
		using element_type = T;
		using pointer = typename _Get_deleter_pointer_type<element_type, std::remove_reference_t<deleter_type>>::type;

		UniquePtr(const UniquePtr&) = delete;

		template <typename = void>
			requires !std::is_pointer_v<deleter_type>&& std::is_default_constructible_v<deleter_type>
		constexpr UniquePtr() noexcept : _ptr(nullptr), _d() {}

		template <typename = void>
			requires !std::is_pointer_v<deleter_type>&& std::is_default_constructible_v<deleter_type>
		constexpr UniquePtr(std::nullptr_t) noexcept : _ptr(nullptr), _d() {}

		//disable CTAD
		template <typename = void>
			requires !std::is_pointer_v<deleter_type>&& std::is_default_constructible_v<deleter_type>
		explicit UniquePtr(pointer p) noexcept : _ptr(p), _d() {}

		template <typename = void>
			requires !std::is_lvalue_reference_v<deleter_type>&& std::is_constructible_v<deleter_type, const deleter_type&>
		UniquePtr(pointer p, const deleter_type& d) noexcept : _ptr(p), _d(d) {}

		template <typename = void>
			requires !std::is_lvalue_reference_v<deleter_type>&& std::is_constructible_v<deleter_type, deleter_type&&>
		UniquePtr(pointer p, deleter_type&& d) noexcept : _ptr(p), _d(std::move(d)) {}

		template <typename = void>
			requires std::is_lvalue_reference_v<deleter_type>&& std::is_constructible_v<deleter_type, deleter_type>
		UniquePtr(pointer p, deleter_type d) noexcept : _ptr(p), _d(d) {}

		template <typename d_nodef = std::remove_reference_t<deleter_type>>
			requires std::is_lvalue_reference_v<deleter_type>&& std::is_constructible_v<deleter_type, d_nodef&&>
		UniquePtr(pointer, d_nodef&&) = delete;

		template <typename = void>
			requires std::is_move_constructible_v<deleter_type>
		UniquePtr(UniquePtr&& r) noexcept : _ptr(r.Release()), _d(std::forward<deleter_type>(r._d)) {}

		template <typename U, typename E>
			requires std::is_convertible_v<typename UniquePtr<U, E>::pointer, pointer> && !std::is_array_v<U>&& std::conditional_t<std::is_reference_v<deleter_type>, std::is_same<deleter_type, E>, std::is_convertible<E, deleter_type>>::value
		UniquePtr(UniquePtr<U, E>&& r) noexcept {
			_ptr = r.Release();
			_d = std::forward<E>(r.GetDeleter());
		}

		~UniquePtr() {
			if (_ptr) {
				_d(_ptr);
			}
		}

		UniquePtr& operator=(const UniquePtr&) = delete;

		template <typename = void>
			requires std::is_move_assignable_v<deleter_type>
		UniquePtr& operator=(UniquePtr&& r) noexcept {
			if (this != std::addressof(r)) {
				Reset(r.Release());
				_d = std::forward<deleter_type>(r._d);
			}
			return *this;
		}

		template <typename U, typename E>
			requires std::is_convertible_v<typename UniquePtr<U, E>::pointer, pointer>&& std::is_assignable_v<deleter_type&, E&&>
		UniquePtr& operator=(UniquePtr<U, E>&& r) noexcept {
			Reset(r.Release());
			_d = std::forward<E>(r.GetDeleter());
			return *this;
		}

		UniquePtr& operator=(std::nullptr_t) noexcept {
			Reset();
			return *this;
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

		void Swap(UniquePtr& other) noexcept {
			UniquePtr temp = std::move(other);
			other = std::move(*this);
			*this = std::move(temp);
		}

		pointer Get() const noexcept {
			return _ptr;
		}

		deleter_type& GetDeleter() noexcept {
			return _d;
		}

		const deleter_type& GetDeleter() const noexcept {
			return _d;
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

	private:
		pointer _ptr;
		deleter_type _d;
	};

	template<typename T, typename Deleter>
	class UniquePtr <T[], Deleter> {
	public:
		using deleter_type = Deleter;
		using element_type = T;
		using pointer = typename _Get_deleter_pointer_type<element_type, std::remove_reference_t<deleter_type>>::type;

		UniquePtr(const UniquePtr&) = delete;

		template <typename = void>
			requires !std::is_pointer_v<deleter_type>&& std::is_default_constructible_v<deleter_type>
		constexpr UniquePtr() noexcept : _ptr(nullptr), _d() {}

		template <typename = void>
			requires !std::is_pointer_v<deleter_type>&& std::is_default_constructible_v<deleter_type>
		constexpr UniquePtr(std::nullptr_t) noexcept : _ptr(nullptr), _d() {}

		template <typename U>
			requires !std::is_pointer_v<deleter_type>&& std::is_default_constructible_v<deleter_type> && (std::is_same_v<U, pointer> || std::is_same_v<U, std::nullptr_t> || (std::is_same_v<pointer, element_type*> && std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>))
		explicit UniquePtr(U p) noexcept :_ptr(p), _d() {}

		template <typename U>
			requires !std::is_lvalue_reference_v<deleter_type>&& std::is_constructible_v<deleter_type, const deleter_type&> && (std::is_same_v<U, pointer> || std::is_same_v<U, std::nullptr_t> || (std::is_same_v<pointer, element_type*> && std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>))
		UniquePtr(U p, const deleter_type& d) noexcept : _ptr(p), _d(d) {}

		template <typename U>
			requires !std::is_lvalue_reference_v<deleter_type>&& std::is_constructible_v<deleter_type, deleter_type&&> && (std::is_same_v<U, pointer> || std::is_same_v<U, std::nullptr_t> || (std::is_same_v<pointer, element_type*> && std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>))
		UniquePtr(U p, deleter_type&& d) noexcept : _ptr(p), _d(std::move(d)) {}

		template <typename U>
			requires
		std::is_lvalue_reference_v<deleter_type>&&
			std::is_constructible_v<deleter_type, deleter_type> &&
			(std::is_same_v<U, pointer> || std::is_same_v<U, std::nullptr_t> || (std::is_same_v<pointer, element_type*> && std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>))
			UniquePtr(U p, deleter_type d) noexcept : _ptr(p), _d(d) {}

		template <typename U, typename d_nodef = std::remove_reference_t<deleter_type>>
			requires
		std::is_lvalue_reference_v<deleter_type>&&
			std::is_constructible_v<deleter_type, d_nodef&&> &&
			(std::is_same_v<U, pointer> || std::is_same_v<U, std::nullptr_t> || (std::is_same_v<pointer, element_type*> && std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>))
			UniquePtr(U, d_nodef&&) = delete;

		template <typename = void>
			requires std::is_move_constructible_v<deleter_type>
		UniquePtr(UniquePtr&& r) noexcept {
			_ptr = r.Release();
			_d = std::forward<deleter_type>(r._d);
		}

		template <typename U, typename E, typename other_emement_type = typename UniquePtr<U, E>::element_type>
			requires
		std::is_array_v<U>&&
			std::is_same_v<pointer, element_type*>&&
			std::is_same_v<typename UniquePtr<U, E>::pointer, other_emement_type*>&&
			std::is_convertible_v<other_emement_type(*)[], element_type(*)[]>&&
			std::conditional_t<std::is_reference_v<deleter_type>, std::is_same<deleter_type, E>, std::is_convertible<E, deleter_type>>::value
			UniquePtr(UniquePtr<U, E>&& r) noexcept {
			_ptr = r.Release();
			_d = std::forward<E>(r.GetDeleter());
		}

		~UniquePtr() {
			if (_ptr) {
				_d(_ptr);
			}
		}

		UniquePtr& operator=(const UniquePtr&) = delete;

		template <typename = void>
			requires std::is_move_assignable_v<deleter_type>
		UniquePtr& operator=(UniquePtr&& r) noexcept {
			if (this != std::addressof(r)) {
				Reset(r.Release());
				_d = std::forward<deleter_type>(r._d);
			}
			return *this;
		}

		template <typename U, typename E, typename other_emement_type = typename UniquePtr<U, E>::element_type>
			requires
		std::is_array_v<U>&&
			std::is_same_v<pointer, element_type*>&&
			std::is_same_v<typename UniquePtr<U, E>::pointer, other_emement_type*>&&
			std::is_convertible_v<other_emement_type(*)[], element_type(*)[]>&&
			std::is_assignable_v<deleter_type&, E&&>
			UniquePtr& operator=(UniquePtr<U, E>&& r) noexcept {
			Reset(r.Release());
			_d = std::forward<E>(r.GetDeleter());
			return *this;
		}

		UniquePtr& operator=(std::nullptr_t) noexcept {
			Reset();
			return *this;
		}

		pointer Release() noexcept {
			auto temp = _ptr;
			_ptr = nullptr;
			return temp;
		}

		template <typename U>
			requires std::is_same_v<U, pointer> || (std::is_same_v<pointer, element_type*> && std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)
		void Reset(U p) noexcept {
			if (_ptr) {
				_d(_ptr);
			}
			_ptr = p;
		}

		void Reset(std::nullptr_t p = nullptr) noexcept {
			if (_ptr) {
				_d(_ptr);
			}
			_ptr = p;
		}

		void Swap(UniquePtr& other) noexcept {
			UniquePtr temp = std::move(other);
			other = std::move(*this);
			*this = std::move(temp);
		}

		pointer Get() const noexcept {
			return _ptr;
		}

		deleter_type& GetDeleter() noexcept {
			return _d;
		}

		const deleter_type& GetDeleter() const noexcept {
			return _d;
		}

		explicit operator bool() const noexcept {
			return Get() != nullptr;
		}

		T& operator[](std::size_t i) const {
			return Get[i];
		}

	private:
		pointer _ptr;
		deleter_type _d;
	};
}