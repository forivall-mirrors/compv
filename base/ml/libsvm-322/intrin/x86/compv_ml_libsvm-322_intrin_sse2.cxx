/* Copyright (C) 2016-2018 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#include "compv/base/ml/libsvm-322/intrin/x86/compv_ml_libsvm-322_intrin_sse2.h"

#if COMPV_ARCH_X86 && COMPV_INTRINSIC
#include "compv/base/compv_debug.h"

COMPV_NAMESPACE_BEGIN()

// Training function, no need for ASM implementation
void CompVLibSVM322KernelRbf0Out_64f64f_SSE2(const double& gamma, const double* xSquarePtr, const double* dotMatPtr, double* outPtr, const size_t count)
{
	COMPV_DEBUG_INFO_CHECK_SSE2();
	const size_t count8 = count & -8;
	const size_t count2 = count & -2;
	size_t i = 0;
	const __m128d vecGammaTimes2Minus = _mm_set1_pd(-(2.0 * gamma));

	for (; i < count8; i += 8) {		
		__m128d vec0 = _mm_sub_pd(_mm_loadu_pd(&xSquarePtr[i]), _mm_loadu_pd(&dotMatPtr[i]));
		__m128d vec1 = _mm_sub_pd(_mm_loadu_pd(&xSquarePtr[i + 2]), _mm_loadu_pd(&dotMatPtr[i + 2]));
		__m128d vec2 = _mm_sub_pd(_mm_loadu_pd(&xSquarePtr[i + 4]), _mm_loadu_pd(&dotMatPtr[i + 4]));
		__m128d vec3 = _mm_sub_pd(_mm_loadu_pd(&xSquarePtr[i + 6]), _mm_loadu_pd(&dotMatPtr[i + 6]));
		vec0 = _mm_mul_pd(vec0, vecGammaTimes2Minus);
		vec1 = _mm_mul_pd(vec1, vecGammaTimes2Minus);
		vec2 = _mm_mul_pd(vec2, vecGammaTimes2Minus);
		vec3 = _mm_mul_pd(vec3, vecGammaTimes2Minus);
		_mm_storeu_pd(&outPtr[i], vec0);
		_mm_storeu_pd(&outPtr[i + 2], vec1);
		_mm_storeu_pd(&outPtr[i + 4], vec2);
		_mm_storeu_pd(&outPtr[i + 6], vec3);
	}
	for (; i < count2; i += 2) {
		__m128d vec0 = _mm_sub_pd(_mm_loadu_pd(&xSquarePtr[i]), _mm_loadu_pd(&dotMatPtr[i]));
		vec0 = _mm_mul_pd(vec0, vecGammaTimes2Minus);
		_mm_storeu_pd(&outPtr[i], vec0);
	}
	for (; i < count; i += 1) {
		__m128d vec0 = _mm_sub_sd(_mm_load_sd(&xSquarePtr[i]), _mm_load_sd(&dotMatPtr[i]));
		vec0 = _mm_mul_sd(vec0, vecGammaTimes2Minus);
		_mm_store_sd(&outPtr[i], vec0);
	}
}

COMPV_NAMESPACE_END()

#endif /* COMPV_ARCH_X86 && COMPV_INTRINSIC */
