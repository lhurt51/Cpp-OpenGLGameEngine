#pragma once

#include "Platform.hpp"

#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
	#include "SSE/SSEVector.hpp"
	typedef SSEVector PlatformVector;
#else
	#include "Generic/GenericVecMath.hpp"
	typedef GenerocVector PlatformVector;
#endif