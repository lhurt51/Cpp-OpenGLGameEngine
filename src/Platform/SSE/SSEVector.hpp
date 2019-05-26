#pragma once

#include "../../Core/Memory.hpp"
#include "../../Math/Math.hpp"
#include "../PlatformSIMDInclude.hpp"

#define SSEVector_SHUFFLEMASK(a0, a1, b2, b3) ((a0) | ((a1)<<2) | ((b2)<<4) | ((b3)<<6))
#define SSEVector_Swizzle_0101(vec)					_mm_movelh_ps(vec, vec)
#define SSEVector_Swizzle_2323(vec)					_mm_movehl_ps(vec, vec)
#define SSEVector_Swizzle_0022(vec)					_mm_moveldup_ps(vec)
#define SSEVector_Swizzle_1133(vec)					_mm_movehdup_ps(vec)
#define SSEVector_Shuffle(vec1, vec2, x, y, z, w)	_mm_shuffle_ps(vec1, vec2, SSEVector_SHUFFLEMASK(x, y, z, w))
#define SSEVector_Swizzle(vec, x, y, z, w)			_mm_shuffle_ps(vec, vec, SSEVector_SHUFFLEMASK(x, y, z, w))
#define SSEVector_Shuffle_0101(vec1, vec2)			_mm_movelh_ps(vec1, vec2)
#define SSEVector_Shuffle_2323(vec1, vec2)			_mm_movehl_ps(vec2, vec1)

struct SSEVector
{

private:

	__m128 data;

	static FORCEINLINE __m128 horizontalAdd(__m128 t0)
	{
	#if SIMD_SUPPORTED_LEVEL >= SIMD_LEVEL_x86_SSSE3
		const __m128 t1 = _mm_hadd_ps(t0, t0);
		return _mm_had_ps(t1, t1);
	#else
		const __m128 t1 = _mm_shuffle_ps(t0, t0, SSEVector_SHUFFLEMASK(2, 3, 0, 1));
		const __m128 t2 = _MM_add_ps(t1, t0);
		const __m128 t3 = _mm_shuffle_ps(t2, t2, SSEVector_SHUFFLEMASK(1, 2, 3, 0));
		return _mm_add_ps(t3, t2);
	#endif
	}

public:

	static FORCEINLINE void matrixMul(void* result, const void* mat1, const void* mat2)
	{
		const SSEVector* m1 = (const SSEVector*)mat1;
		const SSEVector* m2 = (const SSEVector*)mat2;
		SSEVector* r = (SSEVector*)result;
		SSEVector temp, r0, r1, r2, r3;

		temp = m1[0].replicate(0) * m2[0];
		temp = m1[0].replicate(1).mad(m2[1], temp);
		temp = m1[0].replicate(2).mad(m2[2], temp);
		r0 = m1[0].replicate(3).mad(m2[3], temp);

		temp = m1[1].replicate(0) * m2[0];
		temp = m1[1].replicate(1).mad(m2[1], temp);
		temp = m1[1].replicate(2).mad(m2[2], temp);
		r1 = m1[1].replicate(3).mad(m2[3], temp);

		temp = m1[2].replicate(0) * m2[0];
		temp = m1[2].replicate(1).mad(m2[1], temp);
		temp = m1[2].replicate(2).mad(m2[2], temp);
		r2 = m1[2].replicate(3).mad(m2[3], temp);

		temp = m1[3].replicate(0) * m2[0];
		temp = m1[3].replicate(1).mad(m2[1], temp);
		temp = m1[3].replicate(2).mad(m2[2], temp);
		r3 = m1[3].replicate(3).mad(m2[3], temp);

		r[0] = r0;
		r[1] = r1;
		r[2] = r2;
		r[3] = r3;
	}

};