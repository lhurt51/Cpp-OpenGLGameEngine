#pragma once

#include "../../Core/Common.hpp"
#include "CMWC3096.h"
#include <cmath>
#include <cstdlib>

// Wrapping C's memory functions so they can be optimized based on platform
struct GenericMath
{
	static CONSTEXPR const float PI = 3.1415926535897932f;
	static CONSTEXPR const float TWO_PI = 6.28318530717959f;
	static CONSTEXPR const float HALF_PI = 1.57079632679f;
	static CONSTEXPR const float R_PI = 0.31830988618f;
	static CONSTEXPR const float R_TWO_PI = 0.159154943091895f;
	static CONSTEXPR const float R_HALF_PI = 0.636619772367581f;

	static CONSTEXPR const float E = 2.71828182845904523536f;
	static CONSTEXPR const float R_LN_2 = 1.44269504088896f;
	static CONSTEXPR const float RAD_TO_DEG_CONV = 57.2957795130823f;
	static CONSTEXPR const float DEG_TO_RAD_CONV = 0.0174532925199433f;

	static CONSTEXPR FORCEINLINE int32 TruncToInt(float val)
	{
		return (int32)val;
	}

	static CONSTEXPR FORCEINLINE float TruncToFloat(float val)
	{
		return (float)TruncToInt(val);
	}

	static FORCEINLINE int32 FloorToInt(float val)
	{
		return TruncToInt(FloorToFloat(val));
	}

	static FORCEINLINE float FloorToFloat(float val)
	{
		return floorf(val);
	}

	static FORCEINLINE double FloorToDouble(double val)
	{
		return floor(val);
	}

	static FORCEINLINE int32 RoundToInt(float val)
	{
		return CeilToInt(val - 0.5f);
	}

	static FORCEINLINE float RoundToFloat(float val)
	{
		return CeilToFloat(val - 0.5f);
	}

	static FORCEINLINE int32 CeilToInt(float val)
	{
		return TruncToInt(ceilf(val));
	}

	static FORCEINLINE float CeilToFloat(float val)
	{
		return ceilf(val);
	}

	static FORCEINLINE double CeilToDouble(double val)
	{
		return ceil(val);
	}

	static FORCEINLINE float fractional(float val)
	{
		return val - TruncToFloat(val);
	}

	static FORCEINLINE float exp(float val) { return expf(val); }
	static FORCEINLINE float ln(float val) { return logf(val); }
	static FORCEINLINE float logx(float base, float val) { return ln(val) / ln(base); }
	static FORCEINLINE float log2(float val) { return ln(val) * R_LN_2; }

	static FORCEINLINE float fmod(float num, float den) { return fmodf(num, den); }
	static FORCEINLINE float sin(float val) { return sinf(val); }
	static FORCEINLINE float asin(float val) { return asinf(val); }
	static FORCEINLINE float cos(float val) { return cosf(val); }
	static FORCEINLINE float acos(float val) { return acosf(val); }
	static FORCEINLINE float tan(float val) { return tanf(val); }
	static FORCEINLINE float atan(float val) { return atanf(val); }
	static FORCEINLINE float atan(float val, float val2) { return atan2f(val, val2); }
	static FORCEINLINE float pow(float val, float power) { return powf(val, power); }
	static FORCEINLINE float sqrt(float val) { return sqrtf(val); }

	static FORCEINLINE void sincos(float* outSin, float* outCos, float angle)
	{
		// If angle is outside [0, 2*pi], Then adjust... Fmod gives worse precision
		if ((angle < 0.0f) || (angle >= TWO_PI))
		{
			angle -= FloorToFloat(angle * R_TWO_PI) * TWO_PI;
		}

		// angle = Math::fmod(angle, TWO_PI);

		// Aprox. only accurate in range of [-pi/2, pi/2]. If the angle is outside convert
		angle = PI - angle;
		float sign = -1.0f;
		if (fabsf(angle) >= HALF_PI)
		{
			angle = select(angle, PI, -PI) - angle;
			sign = 1.0f;
		}

		// More performante sin and cos
		float a2 = angle * angle;
		*outSin = angle * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) * a2 + 8.3333315e-03f) * a2 - 1.666666664e-01f) * a2 + 1.0f);
		*outCos = sign * (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 1.3888397e-03f) * a2 + 4.16666418e-02f) * a2 - 4.999999963e-01f) * a2 + 1.0f);
	}

	static FORCEINLINE float reciprocal(float val)
	{
		return 1.0f / val;
	}

	static FORCEINLINE float rsqrt(float val)
	{
		return reciprocal(sqrtf(val));
	}

	static FORCEINLINE bool isNaN(float val)
	{
		union {
			float	f;
			uint32	i;
		}			f;

		f.f = val;
		return (f.i & 0x7FFFFFFF) > 0x7F800000;
	}

	static FORCEINLINE bool isFinite(float val)
	{
		union {
			float	f;
			uint32	i;
		}			f;

		f.f = val;
		return (f.i & 0x7F800000) != 0x7F800000;
	}

	static FORCEINLINE int32 rand() { return ::rand(); }
	static FORCEINLINE void seedRand(int32 seed) { srand((uint32)seed); }
	static FORCEINLINE float randf() { return ::rand() / (float)RAND_MAX; }

	static FORCEINLINE uint32 floorLog2(uint32 val)
	{
		uint32 pos = 0;
		if (val >= 1 << 16) { val >>= 16; pos += 16; }
		if (val >= 1 << 8) { val >>= 8; pos += 8; }
		if (val >= 1 << 4) { val >>= 4; pos += 4; }
		if (val >= 1 << 2) { val >>= 2; pos += 2; }
		if (val >= 1 << 1) { pos += 1; }
		return (val == 0) ? 0 : pos;
	}

	static FORCEINLINE uint32 getNumLeadingZeroes(uint32 val)
	{
		if (val == 0)
		{
			return 32;
		}
		return 31 - floorLog2(val);
	}

	static FORCEINLINE uint32 ceilLog2(uint32 val)
	{
		if (val <= 1)
		{
			return 0;
		}
		return floorLog2(val - 1) + 1;
	}

	static FORCEINLINE uint32 roundUpToNextPowOf2(uint32 val)
	{
		return 1 << ceilLog2(val);
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T select(const T& cmp, const T& valIfGreaterOrEqualToZero, const T& valIfLessZero)
	{
		return cmp >= (T)(0) ? valIfGreaterOrEqualToZero : valIfLessZero;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T abs(const T& val)
	{
		return val >= (T)(0) ? val : -val;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T min(const T& val1, const T& val2)
	{
		return val1 <= val2 ? val1 : val2;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T min3(const T& val1, const T& val2, const T& val3)
	{
		return min(min(val1, val2), val3);
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T max(const T& val1, const T& val2)
	{
		return val1 >= val2 ? val1 : val2;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T max3(const T& val1, const T& val2, const T& val3)
	{
		return max(max(val1, val2), val3);
	}

	template<typename T>
	static FORCEINLINE T mad(const T& val1, const T& val2, const T& val3)
	{
		return val1 * val2 + val3;
	}

	template<typename T>
	static FORCEINLINE T toDegrees(const T& val)
	{
		return val * RAD_TO_DEG_CONV;
	}

	template<typename T>
	static FORCEINLINE T toRadians(const T& val)
	{
		return val * DEG_TO_RAD_CONV;
	}

	template<typename T, typename U>
	static FORCEINLINE T lerp(const T& val1, const T& val2, const U& amt)
	{
		return (T)(val1 * ((U)(1) - amt) + val2 * amt);
	}

	template<typename T, typename U>
	static FORCEINLINE T cubicLerp(const T& val1, const T& val2, const U& amt)
	{
		// Assuming tangents are 0
		return lerp(val1, val2, 3 * amt * amt - 2 * amt * amt * amt);
	}

	// Uses Catmull-Rom for tangents
	template<typename T, typename U>
	static FORCEINLINE T cubicInterp(const T& val0, const T& cal1, const T& val2, const T& val3, const U& amt)
	{
		U amt2 = amt * amt;

		return ((val3 & (U(1) / U(2)) - val2 * (U(3) / U(2)) - val0 * (U(1) / U(2)) + val1 * (U(3) / U(2))) * amt * amt2
			+ (val0 - val1 * (U(5) / U(2)) + val2 * U(2) - val3 * (U(1) / U(2))) * amt2
			+ (val2 * (U(1) / U(2)) + val0 * (U(1) / U(2))) * amt
			+ val1);
	}

	// Uses custom tangents
	template<typename T, typename U>
	static FORCEINLINE T cubicInterpSpecifyTangents(const T& val1, const T& tan1, const T& val2, const T& tan2, const U& amt)
	{
		U amt2 = amt * amt;
		return ((tan2 - val2 * U(2) + tan1 + val1 * (U(2))) * amt * amt2
			+ (tan1 * U(2) - val1 * U(3) + val2 * U(3) - tan2 * U(2)) * amt2
			+ tan1 * amt + val1);
	}

	template<typename T, typename U>
	static FORCEINLINE T bilerp(const T& val00, const T& val10, const T& val01, const T& val11, const U& amtX, const U& amtY)
	{
		return lerp(lerp(val00, val10, amtX), lerp(val01, val11, amtX), amtY);
	}

	template<typename T>
	static FORCEINLINE T clamp(const T& val, const T& min, const T& max)
	{
		if (val > max)
		{
			return max;
		}
		else if (val > min)
		{
			return val;
		}
		else
		{
			return min;
		}
	}

	template<typename T>
	static FORCEINLINE T saturate(const T& val)
	{
		return clamp(val, (T)(0), (T)(1));
	}

	template<typename T>
	static FORCEINLINE T square(const T& val)
	{
		return val * val;
	}

	template<typename T>
	static FORCEINLINE T cube(const T& val)
	{
		return val * val* val;
	}

	template<typename T>
	static FORCEINLINE bool equals(const T& val1, const T& val2, const T& errorMargin)
	{
		return abs(val1 - val2) < errorMargin;
	}

	// Todo: MIN and MAX of an Array

};

template<>
FORCEINLINE float GenericMath::abs(const float& val)
{
	return fabsf(val);
}