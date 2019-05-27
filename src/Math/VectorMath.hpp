#pragma once

#include "../Core/Memory.hpp"
#include "../Platform/PlatformVecMath.hpp"
#include "Math.hpp"

typedef PlatformVector Vector;

struct VectorConstants
{
	static const Vector ZERO;
	static const Vector ONE;
	static const Vector TWO;
	static const Vector HALF;
	static const Vector INF;
	static const Vector MASK_X;
	static const Vector MASK_Y;
	static const Vector MASK_Z;
	static const Vector MASK_W;
	static const Vector SIGN_MASK;
};

template<>
FORCEINLINE Vector Math::lerp(const Vector& val1, const Vector& val2, const float& amt)
{
	return (val2 - val1) * Vector::load1f(amt) + val1;
}