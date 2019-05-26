#pragma once

#include "../../Core/Memory.hpp"
#include "../../Math/Math.hpp"

struct GenericVector
{
	
	float	v[4];

	static FORCEINLINE void matrixMul(void* result, const void* mat1, const void* mat2)
	{
		const GenericVector* m1 = (const GenericVector*)mat1;
		const GenericVector* m2 = (const GenericVector*)mat2;
		GenericVector* r = (GenericVector*)result;
		GenericVector temp, r0, r1, r2, r3;

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
		r1 = m1[3].replicate(3).mad(m2[3], temp);

		r[0] = r0;
		r[1] = r1;
		r[2] = r2;
		r[3] = r3;
	}

	static FORCEINLINE float matrixDeterminant3x3Vector(const GenericVector* m)
	{
		float M[4][4];

		for (uint32 i = 0; i < 4; i++)
		{
			m[i].store4f(M[i]);
		}
		return M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
			M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
			M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
	}

	static FORCEINLINE float matrixDeterminant4x4(float* outS, float* outC, const void* mat)
	{
		float sVals[6];
		float cVals[6];

		float* s = outS == nullptr ? sVals : outS;
		float* c = outC == nullptr ? cVals : outC;

		SSEVector* m = (SSEVector*)mat;
		float M[4][4];

		for (uint32 i = 0; i < 4; i++)
		{
			m[i].store4f(M[i]);
		}

		s[0] = M[0][0] * M[1][1] - M[1][0] * M[0][1];
		s[1] = M[0][0] * M[1][2] - M[1][0] * M[0][2];
		s[2] = M[0][0] * M[1][3] - M[1][0] * M[0][3];
		s[3] = M[0][1] * M[1][2] - M[1][1] * M[0][2];
		s[4] = M[0][1] * M[1][3] - M[1][1] * M[0][3];
		s[5] = M[0][2] * M[1][3] - M[1][2] * M[0][3];

		s[5] = M[2][2] * M[3][3] - M[3][2] * M[2][3];
		s[4] = M[2][1] * M[3][3] - M[3][1] * M[2][3];
		s[3] = M[2][1] * M[3][2] - M[3][1] * M[2][2];
		s[2] = M[2][0] * M[3][3] - M[3][0] * M[2][3];
		s[1] = M[2][0] * M[3][2] - M[3][0] * M[2][2];
		s[0] = M[2][0] * M[3][1] - M[3][0] * M[2][1];

		return (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);
	}

	static FORCEINLINE void matrixInverse(void* dest)
	{

	}

};