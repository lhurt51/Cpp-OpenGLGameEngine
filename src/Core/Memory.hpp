#pragma once

#include "Common.hpp"
#include "../Platform/PlatformMemory.hpp"

#include <cstring>

// Encapsulate C's memory functions so they can be optimized for different platforms
struct Memory
{
	static FORCEINLINE void* memmove(void* dest, const void* src, uintptr amt)
	{
		return PlatformMemory::memove(dest, src, amt);
	}

	static FORCEINLINE int32 memcmp(const void* dest, const void* src, uintptr amt)
	{
		return PlatformMemory::memcmp(dest, src, amt);
	}

	template<typename T>
	static FORCEINLINE void* memset(void* dest, T val, uintptr amt)
	{
		return PlatformMemory::memset(dest, val, amt);
	}

	static FORCEINLINE void* memzero(void* dest, uintptr amt)
	{
		return PlatformMemory::memset(dest, 0, amt);
	}

	static FORCEINLINE void* memcpy(void* dest, const void* src, uintptr amt)
	{
		return PlatformMemory::memcpy(dest, src, amt);
	}

	static FORCEINLINE void memswap(void* a, void* b, uintptr size)
	{
		return PlatformMemory::memswap(a, b, size);
	}

	enum
	{
		DEFAULT_ALIGNMENT = 0,
		MIN_ALIGNMENT = 8
	};

	template<typename T>
	static FORCEINLINE CONSTEXPR T align(const T ptr, uintptr alignment)
	{
		return PlatformMemory::align(ptr, alignment);
	}

	static FORCEINLINE void* malloc(uintptr amt, uint32 alignment = DEFAULT_ALIGNMENT)
	{
		return PlatformMemory::malloc(amt, alignment);
	}

	static FORCEINLINE void* realloc(void* ptr, uintptr amt, uint32 alignment = DEFAULT_ALIGNMENT)
	{
		return PlatformMemory::realloc(ptr, amt, alignment);
	}

	static FORCEINLINE void* free(void* ptr)
	{
		return PlatformMemory::free(ptr);
	}

	static FORCEINLINE uintptr getAllocSize(void* ptr)
	{
		return PlatformMemory::getAllocSize(ptr);
	}

};