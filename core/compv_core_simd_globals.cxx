/* Copyright (C) 2016-2018 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#include "compv/core/compv_core_simd_globals.h"

#if COMPV_ARCH_X86

#define COMPV_MM_SHUFFLE_EPI8_AVX(a, b, c, d) COMPV_MM_SHUFFLE_EPI8(a+16, b+16, c+16, d+16)

COMPV_EXTERNC COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_16s7_32s[] = { // To be used with _mm_shuffle_epi8 to duplicate the last (7th) element in epi16, use vdup.u16 q0, d1[] for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(15, 14, 15, 14), COMPV_MM_SHUFFLE_EPI8(15, 14, 15, 14), COMPV_MM_SHUFFLE_EPI8(15, 14, 15, 14), COMPV_MM_SHUFFLE_EPI8(15, 14, 15, 14), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(15, 14, 15, 14), COMPV_MM_SHUFFLE_EPI8_AVX(15, 14, 15, 14), COMPV_MM_SHUFFLE_EPI8_AVX(15, 14, 15, 14), COMPV_MM_SHUFFLE_EPI8_AVX(15, 14, 15, 14) // 256bits AVX register
};

COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL0_8u_32s[]{ // To be used with _mm_shuffle_epi8, use vdup.u8[0]/vshl.u8#0 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0, 0, 0, 0), COMPV_MM_SHUFFLE_EPI8(0, 0, 0, 0), COMPV_MM_SHUFFLE_EPI8(0, 0, 0, 0), COMPV_MM_SHUFFLE_EPI8(0, 0, 0, 0), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0, 0, 0, 0), COMPV_MM_SHUFFLE_EPI8_AVX(0, 0, 0, 0), COMPV_MM_SHUFFLE_EPI8_AVX(0, 0, 0, 0), COMPV_MM_SHUFFLE_EPI8_AVX(0, 0, 0, 0) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL1_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[1]/vshl.u8#1 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(1, 1, 1, 0x80), COMPV_MM_SHUFFLE_EPI8(1, 1, 1, 1), COMPV_MM_SHUFFLE_EPI8(1, 1, 1, 1), COMPV_MM_SHUFFLE_EPI8(1, 1, 1, 1), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(1, 1, 1, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(1, 1, 1, 1), COMPV_MM_SHUFFLE_EPI8_AVX(1, 1, 1, 1), COMPV_MM_SHUFFLE_EPI8_AVX(1, 1, 1, 1) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL2_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[2]/vshl.u8#2 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(2, 2, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(2, 2, 2, 2), COMPV_MM_SHUFFLE_EPI8(2, 2, 2, 2), COMPV_MM_SHUFFLE_EPI8(2, 2, 2, 2), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(2, 2, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(2, 2, 2, 2), COMPV_MM_SHUFFLE_EPI8_AVX(2, 2, 2, 2), COMPV_MM_SHUFFLE_EPI8_AVX(2, 2, 2, 2) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL3_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[3]/vshl.u8#3 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(3, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(3, 3, 3, 3), COMPV_MM_SHUFFLE_EPI8(3, 3, 3, 3), COMPV_MM_SHUFFLE_EPI8(3, 3, 3, 3), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(3, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(3, 3, 3, 3), COMPV_MM_SHUFFLE_EPI8_AVX(3, 3, 3, 3), COMPV_MM_SHUFFLE_EPI8_AVX(3, 3, 3, 3) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL4_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[4]/vshl.u8#4 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(4, 4, 4, 4), COMPV_MM_SHUFFLE_EPI8(4, 4, 4, 4), COMPV_MM_SHUFFLE_EPI8(4, 4, 4, 4), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(4, 4, 4, 4), COMPV_MM_SHUFFLE_EPI8_AVX(4, 4, 4, 4), COMPV_MM_SHUFFLE_EPI8_AVX(4, 4, 4, 4) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL5_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[5]/vshl.u8#5 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(5, 5, 5, 0x80), COMPV_MM_SHUFFLE_EPI8(5, 5, 5, 5), COMPV_MM_SHUFFLE_EPI8(5, 5, 5, 5), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(5, 5, 5, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(5, 5, 5, 5), COMPV_MM_SHUFFLE_EPI8_AVX(5, 5, 5, 5) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL6_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[6]/vshl.u8#6 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(6, 6, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(6, 6, 6, 6), COMPV_MM_SHUFFLE_EPI8(6, 6, 6, 6), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(6, 6, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(6, 6, 6, 6), COMPV_MM_SHUFFLE_EPI8_AVX(6, 6, 6, 6) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL7_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[7]/vshl.u8#7 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(7, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(7, 7, 7, 7), COMPV_MM_SHUFFLE_EPI8(7, 7, 7, 7), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(7, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(7, 7, 7, 7), COMPV_MM_SHUFFLE_EPI8_AVX(7, 7, 7, 7) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL8_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[8]/vshl.u8#8 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(8, 8, 8, 8), COMPV_MM_SHUFFLE_EPI8(8, 8, 8, 8), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(8, 8, 8, 8), COMPV_MM_SHUFFLE_EPI8_AVX(8, 8, 8, 8) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL9_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[9]/vshl.u8#9 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(9, 9, 9, 0x80), COMPV_MM_SHUFFLE_EPI8(9, 9, 9, 9), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(9, 9, 9, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(9, 9, 9, 9) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL10_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[10]/vshl.u8#10 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(10, 10, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(10, 10, 10, 10), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(10, 10, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(10, 10, 10, 10) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL11_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[11]/vshl.u8#11 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(11, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(11, 11, 11, 11), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(11, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(11, 11, 11, 11) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL12_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[12]/vshl.u8#12 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(12, 12, 12, 12), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(12, 12, 12, 12) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL13_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[13]/vshl.u8#13 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(13, 13, 13, 0x80), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(13, 13, 13, 0x80) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL14_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[14]/vshl.u8#14 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(14, 14, 0x80, 0x80), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(14, 14, 0x80, 0x80) // 256bits AVX register
};
COMPV_CORE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_DUP_SHL15_8u_32s[] = { // To be used with _mm_shuffle_epi8, use vdup.u8[15]/vshl.u8#15 for ARM NEON
	COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8(15, 0x80, 0x80, 0x80), // 128bits SSE register
	COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(0x80, 0x80, 0x80, 0x80), COMPV_MM_SHUFFLE_EPI8_AVX(15, 0x80, 0x80, 0x80) // 256bits AVX register
};

#endif /* COMPV_ARCH_X86 */