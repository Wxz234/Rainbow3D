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
	class UniquePtr {
	public:
		UniquePtr() = default;
		UniquePtr(std::nullptr_t) {}
		UniquePtr(const UniquePtr&) = delete;

		template <typename U>
		requires IsConvertible<U*, T*>
		explicit UniquePtr(U* InPtr) : _ptr(InPtr) {}

		template <typename U>
		requires IsConvertible<U*, T*>
		explicit UniquePtr(U* InPtr, Deleter&& InDeleter) : _ptr(InPtr, std::move(InDeleter)) {}

		template <typename U>
		requires IsConvertible<U*, T*>
		explicit UniquePtr(U* InPtr, const Deleter& InDeleter) : _ptr(InPtr, InDeleter) {}

		UniquePtr(UniquePtr&& Other) : _ptr(std::move(Other._ptr)) {}

		template <typename OtherT, typename OtherDeleter>
		requires !IsArray<OtherT> && IsConvertible<OtherT*, T*>
		UniquePtr(UniquePtr<OtherT, OtherDeleter>&& Other) {
			_ptr = std::move(Other._ptr);
		}

		UniquePtr& operator=(const UniquePtr&) = delete;

		UniquePtr& operator=(UniquePtr&& Other)
		{
			if (this != std::addressof(Other))
			{
				_ptr = std::move(Other._ptr);
			}
			return *this;
		}

		template <typename OtherT, typename OtherDeleter>
		requires !IsArray<OtherT>&& IsConvertible<OtherT*, T*>
		UniquePtr& operator=(UniquePtr<OtherT, OtherDeleter>&& Other)
		{
			_ptr = std::move(Other._ptr);
			return *this;
		}

		UniquePtr& operator=(std::nullptr_t)
		{
			Reset();
			return *this;
		}

		~UniquePtr() {}

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

	template <typename ObjectType>
	class SharedPtr {
	public:
		SharedPtr() = default;
		SharedPtr(std::nullptr_t) {}

	private:
		std::shared_ptr<ObjectType> _ptr;
	};

}