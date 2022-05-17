#pragma once
#include <cstddef>
#include <type_traits>
namespace Rainbow3D {

	template<typename T> 
	class UniquePtr {
	public:
		UniquePtr() noexcept
		{
			ptr = nullptr;
		}

		UniquePtr(nullptr_t) noexcept
		{
			ptr = nullptr;
		}

		UniquePtr(const UniquePtr&) = delete;

		template <typename U>
		requires std::is_convertible_v<U*, T*>
		explicit UniquePtr(U* InPtr)
			: ptr(InPtr)
		{
		}

		UniquePtr(UniquePtr&& Other) noexcept
		{
			ptr = Other.ptr;
			Other.ptr = nullptr;
		}

		template <typename OtherT>
		requires std::negation_v<std::is_array<OtherT>> && std::is_convertible_v<OtherT*, T*>
		UniquePtr(UniquePtr<OtherT>&& Other) noexcept
		{
			ptr = Other.Release();
		}

		UniquePtr& operator=(const UniquePtr&) = delete;
		UniquePtr& operator=(UniquePtr&& Other) noexcept
		{
			if (this != &Other)
			{
				T* OldPtr = ptr;
				ptr = Other.ptr;
				Other.ptr = nullptr;
				delete OldPtr;
			}
			return *this;
		}

		template <typename OtherT>
		requires std::negation_v<std::is_array<OtherT>> && std::is_convertible_v<OtherT*, T*>
		UniquePtr& operator=(UniquePtr<OtherT>&& Other) noexcept
		{
			delete ptr;
			ptr = Other.Release();
			return *this;
		}

		UniquePtr& operator=(nullptr_t)
		{
			Reset();
			return *this;
		}

		~UniquePtr()
		{
			if (ptr) {
				delete ptr;
			}
		}

		bool IsValid() const
		{
			return ptr != nullptr;
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
			return ptr;
		}

		T& operator*() const
		{
			return *ptr;
		}

		T* Get() const
		{
			return ptr;
		}

		T* Release()
		{
			T* Result = ptr;
			ptr = nullptr;
			return Result;
		}

		void Reset(T* InPtr = nullptr)
		{
			if (ptr != InPtr)
			{
				T* OldPtr = ptr;
				ptr = InPtr;
				delete OldPtr;
			}
		}

	private:
		T* ptr;
	};

	template <typename T>
	class UniquePtr<T[]> {
	public:
		UniquePtr() noexcept
		{
			ptr = nullptr;
		}

		UniquePtr(nullptr_t) noexcept
		{
			ptr = nullptr;
		}

		UniquePtr(const UniquePtr&) = delete;

		template <typename U>
		requires std::is_convertible_v<U(*)[], T(*)[]>
		explicit UniquePtr(U* InPtr)
		{
			ptr = InPtr;
		}

		UniquePtr(UniquePtr&& Other) noexcept
		{
			ptr = Other.ptr;
			Other.ptr = nullptr;
		}

		template <typename OtherT>
		requires std::is_convertible_v<OtherT(*)[], T(*)[]>
		UniquePtr& operator=(UniquePtr<OtherT>&& Other) noexcept
		{
			delete[] ptr;
			ptr = Other.Release();
			return *this;
		}

		UniquePtr& operator=(const UniquePtr& Other) = delete;
		UniquePtr& operator=(UniquePtr&& Other) noexcept
		{
			if (this != &Other)
			{
				T* OldPtr = ptr;
				ptr = Other.ptr;
				Other.ptr = nullptr;
				delete[] OldPtr;
			}
			return *this;
		}

		template <typename OtherT>
		requires std::is_convertible_v<OtherT*, T*>
		UniquePtr& operator=(UniquePtr<OtherT>&& Other) noexcept
		{
			delete[] ptr;
			ptr = Other.Release();
			return *this;
		}

		UniquePtr& operator=(nullptr_t)
		{
			Reset();
			return *this;
		}

		~UniquePtr()
		{
			if (ptr) {
				delete[] ptr;
			}
		}


		bool IsValid() const
		{
			return ptr != nullptr;
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
			return ptr;
		}

		T& operator*() const
		{
			return *ptr;
		}

		T* Get() const
		{
			return ptr;
		}

		T* Release()
		{
			T* Result = ptr;
			ptr = nullptr;
			return Result;
		}

		void Reset(T* InPtr = nullptr)
		{
			if (ptr != InPtr)
			{
				T* OldPtr = ptr;
				ptr = InPtr;
				delete[] OldPtr;
			}
		}

	private:
		T* ptr;
	};
}