#pragma once

#include <cassert>
#include <stdio.h>
#include <stdexcept>

#include "../Platform/Platform.hpp"

// Include known-size int files, based on compiler, some comilers need these files to be created manually
#if defined(__GNUC__) || defined(__clang__) || (defined(_MSC_VER) && _MSC_VER >= 1600)
	#include <stdint.h>
#elif defined(_MSC_VER)
	typedef signed		__int8	int8_t;
	typedef unsigned	__int8	uint8_t;
	typedef signed		__int16 int16_t;
	typedef unsigned	__int16 uint16_t;
	typedef signed		__int32 int32_t;
	typedef unsigned	__int32 uint32_t;
	typedef signed		__int64 int64_t;
	typedef unsigned	__int64 uint64_t;
	typedef uint64_t			uintptr_t;
	typedef int64_t				intptr_t;
	typedef int16_t				wchar_t;
#else
	typedef signed char			int8_t;
	typedef unsigned char		uint8_t;
	typedef signed short int	int16_t;
	typedef unsigned short int	uint16_t;
	typedef signed int			int32_t;
	typedef unsigned int		uint32_t;
	typedef long long			int64_t;
	typedef unsigned long long	uint64_t;
	typedef uint64_t			uintptr_t;
	typedef int64_t				intptr_t;
	typedef int16_t				wchar_t;
#endif

typedef uint8_t					CHART;

typedef int8_t					int8;
typedef int16_t					int16;
typedef int32_t					int32;
typedef int64_t					int64;
typedef uint8_t					uint8;
typedef uint16_t				uint16;
typedef uint32_t				uint32;
typedef uint64_t				uint64;
typedef intptr_t				intptr;
typedef uintptr_t				uintptr;

#ifdef DEBUG
	#define assertCheck assert
#else
	#define assertCheck (void)
#endif

#ifdef COMPILER_MSVC
	#define FORCEINLINE __forceinline
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define FORCEINLINE inline __attribute__ ((always_inline))
#else
	#define FORCEINLINE inline
#endif

// For some reason __cplusplus predefinition does not work properly
#define CONSTEXPR constexpr

/*
#if __cplusplus < 201103L || __cplusplus_cli < 201103L || __cplusplus_winrt < 201103L
	#define nullptr NULL
	#define CONSTEXPR
#else
	#define CONSTEXPR constexpr
#endif
*/

#define NULL_COPY_AND_ASSIGN(T) \
	T(const T& other) { (void)other; } \
	void operator=(const T& other) { (void)other; }

#define LOG_ERROR "Error"
#define LOG_WARNING "Warning"
#define LOG_TYPE_RENDERER "Renderer"
#define LOG_TYPE_IO "IO"
#define DEBUG_LOG(category, level, message, ...) \
	fprintf(stderr, "[%s] ", category); \
	fprintf(stderr, "[%s] (%s:%d): ", level, __FILE__, __LINE__); \
	fprintf(stderr, message, ##__VA_ARGS__); \
	fprintf(stderr, "\n")
#define DEBUG_LOG_TEMP(message, ...) DEBUG_LOG("TEMP", "TEMP", message, ##__VA_ARGS__)
#define DEBUG_LOG_TEMP2(message) DEBUG_LOG("TEMP", "TEMP", "%s", message)
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))