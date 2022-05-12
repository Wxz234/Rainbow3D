#pragma once
#include <memory>
#include <cstddef>
#include <utility>
#include <type_traits>

namespace Rainbow3D {

	template <typename T, typename U>
	concept IsConvertible = std::is_convertible_v<T, U>;

	template <typename T>
	concept IsArray = std::is_array_v<T>;

	template <typename T, typename Deleter = std::default_delete<T>>
	class UniquePointer {
	public:
		UniquePointer() = default;
		UniquePointer(std::nullptr_t) {}
		UniquePointer(const UniquePointer&) = delete;

		template <typename U>
		requires IsConvertible<U*, T*>
		explicit UniquePointer(U* InPtr) : _ptr(InPtr) {}

		template <typename U>
		requires IsConvertible<U*, T*>
		explicit UniquePointer(U* InPtr, Deleter&& InDeleter) : _ptr(InPtr, std::move(InDeleter)) {}

		template <typename U>
		requires IsConvertible<U*, T*>
		explicit UniquePointer(U* InPtr, const Deleter& InDeleter) : _ptr(InPtr, InDeleter) {}

		UniquePointer(UniquePointer&& Other) : _ptr(std::move(Other._ptr)) {}

		template <typename OtherT, typename OtherDeleter>
		requires !IsArray<OtherT> && IsConvertible<OtherT*, T*>
		UniquePointer(UniquePointer<OtherT, OtherDeleter>&& Other) {
			_ptr = std::move(Other._ptr);
		}

		UniquePointer& operator=(const UniquePointer&) = delete;

		UniquePointer& operator=(UniquePointer&& Other)
		{
			if (this != std::addressof(Other))
			{
				_ptr = std::move(Other._ptr);
			}
			return *this;
		}

		template <typename OtherT, typename OtherDeleter>
		requires !IsArray<OtherT>&& IsConvertible<OtherT*, T*>
		UniquePointer& operator=(UniquePointer<OtherT, OtherDeleter>&& Other)
		{
			_ptr = std::move(Other._ptr);
			return *this;
		}

		UniquePointer& operator=(std::nullptr_t)
		{
			Reset();
			return *this;
		}

		~UniquePointer() {}

		bool IsValid() const
		{
			return _ptr != nullptr;
		}

		explicit operator bool() const
		{
			return IsValid();
		}

		bool operator!() const
		{
			return !IsValid();
		}

		T* operator->() const
		{
			return _ptr.get();
		}

		T& operator*() const
		{
			return *_ptr;
		}

		T* Get() const
		{
			return _ptr.get();
		}

		T* Release()
		{
			return _ptr.release();
		}

		void Reset(T* InPtr = nullptr)
		{
			if (_ptr.get() != InPtr)
			{
				_ptr.reset(InPtr);
			}
		}

	private:
		std::unique_ptr<T, Deleter> _ptr;
	};

}