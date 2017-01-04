#include "../tests_common.h"

#if COMPV_OS_WINDOWS
#	define COMPV_TEST_IMAGE_FOLDER			"C:/Projects/GitHub/data/test_images"
#elif COMPV_OS_OSX
#	define COMPV_TEST_IMAGE_FOLDER			"/Users/mamadou/Projects/GitHub/data/test_images"
#else
#	define COMPV_TEST_IMAGE_FOLDER			NULL
#endif
#define COMPV_TEST_PATH_TO_FILE(filename)		compv_tests_path_from_file(filename, COMPV_TEST_IMAGE_FOLDER)

#define THRESHOLD			10
#define NONMAXIMA			1
#define FASTTYPE			COMPV_FAST_TYPE_12
#define MAXFEATURES			-1

#define JPEG_IMG_OPENGLBOOK			"opengl_programming_guide_8th_edition_200x258_gray.yuv"
#define JPEG_IMG_GRIOTS				"mandekalou_480x640_gray.yuv"
#define JPEG_IMG_EQUIRECTANGULAR	"equirectangular_1282x720_gray.yuv"

#define TEST_TYPE_OPENGLBOOK			0
#define TEST_TYPE_GRIOTS				1
#define TEST_TYPE_EQUIRECTANGULAR		2

#define TEST_TYPE						TEST_TYPE_EQUIRECTANGULAR

#if TEST_TYPE == TEST_TYPE_OPENGLBOOK
#	define JPEG_IMG						JPEG_IMG_OPENGLBOOK
#	define FAST9_T10_CORNERS_COUNT		5540
#	define FAST9_T10_CORNERS_SCORES		173438.000
#	define FAST9_T10_XF					501490.000
#	define FAST9_T10_YF					708067.000
#	define FAST9_T10_NONMAX_COUNT		1282
#	define FAST9_T10_NONMAX_SCORES		52936.0000
#	define FAST9_T10_NONMAX_XF			117722.000
#	define FAST9_T10_NONMAX_YF			160696.000
#	define FAST12_T10_CORNERS_COUNT		2930
#	define FAST12_T10_CORNERS_SCORES	79671.f
#	define FAST12_T10_XF				260098.f
#	define FAST12_T10_YF				368964.f
#	define FAST12_T10_NONMAX_COUNT		978
#	define FAST12_T10_NONMAX_SCORES		31952.f
#	define FAST12_T10_NONMAX_XF			88469.f
#	define FAST12_T10_NONMAX_YF			121621.f
#elif TEST_TYPE == TEST_TYPE_GRIOTS
#	define JPEG_IMG						JPEG_IMG_GRIOTS
#	define FAST9_T10_CORNERS_COUNT		27208
#	define FAST9_T10_CORNERS_SCORES		720203.f
#	define FAST9_T10_XF					5722419.f
#	define FAST9_T10_YF					9740405.f
#	define FAST9_T10_NONMAX_COUNT		5405
#	define FAST9_T10_NONMAX_SCORES		176389.f
#	define FAST9_T10_NONMAX_XF			1166708.f
#	define FAST9_T10_NONMAX_YF			1973623.f
#	define FAST12_T10_CORNERS_COUNT		12113
#	define FAST12_T10_CORNERS_SCORES	295977.f
#	define FAST12_T10_XF				2646636.f
#	define FAST12_T10_YF				4152440.f
#	define FAST12_T10_NONMAX_COUNT		3425
#	define FAST12_T10_NONMAX_SCORES		95590.0000
#	define FAST12_T10_NONMAX_XF			750394.000
#	define FAST12_T10_NONMAX_YF			1198537.00
#elif TEST_TYPE == TEST_TYPE_EQUIRECTANGULAR
#	define JPEG_IMG						JPEG_IMG_EQUIRECTANGULAR
#	define FAST9_T10_CORNERS_COUNT		24105
#	define FAST9_T10_CORNERS_SCORES		574969.f
#	define FAST9_T10_XF					15148142.f
#	define FAST9_T10_YF					9577924.f
#	define FAST9_T10_NONMAX_COUNT		6598
#	define FAST9_T10_NONMAX_SCORES		167156.f
#	define FAST9_T10_NONMAX_XF			4326974.f
#	define FAST9_T10_NONMAX_YF			2669475.f
#	define FAST12_T10_CORNERS_COUNT		10812
#	define FAST12_T10_CORNERS_SCORES	239835.f
#	define FAST12_T10_XF				6885661.f
#	define FAST12_T10_YF				4326231.f
#	define FAST12_T10_NONMAX_COUNT		3920
#	define FAST12_T10_NONMAX_SCORES		89172.f
#	define FAST12_T10_NONMAX_XF			2584280.f
#	define FAST12_T10_NONMAX_YF			1576105.f
#endif

#define FAST_LOOP_COUNT	1

COMPV_ERROR_CODE feature_fast()
{
	CompVCornerDetePtr fast;
	CompVMatPtr image;
	CompVBoxInterestPointPtr interestPoints;
	uint64_t timeStart, timeEnd;

#if TEST_TYPE == TEST_TYPE_OPENGLBOOK
	COMPV_CHECK_CODE_RETURN(CompVImage::readPixels(COMPV_SUBTYPE_PIXELS_Y, 200, 258, 200, COMPV_TEST_PATH_TO_FILE(JPEG_IMG).c_str(), &image));
#elif TEST_TYPE == TEST_TYPE_GRIOTS
	COMPV_CHECK_CODE_RETURN(CompVImage::readPixels(COMPV_SUBTYPE_PIXELS_Y, 480, 640, 480, COMPV_TEST_PATH_TO_FILE(JPEG_IMG).c_str(), &image));
#elif TEST_TYPE == TEST_TYPE_EQUIRECTANGULAR
	COMPV_CHECK_CODE_RETURN(CompVImage::readPixels(COMPV_SUBTYPE_PIXELS_Y, 1282, 720, 1282, COMPV_TEST_PATH_TO_FILE(JPEG_IMG).c_str(), &image));
#else
#error "Unexpected"
#endif

	// Create the FAST feature detector
	COMPV_CHECK_CODE_RETURN(CompVCornerDete::newObj(&fast,COMPV_FAST_ID));

	// Set the default values
	COMPV_CHECK_CODE_RETURN(fast->setInt(COMPV_FAST_SET_INT_THRESHOLD, THRESHOLD));
	COMPV_CHECK_CODE_RETURN(fast->setInt(COMPV_FAST_SET_INT_FAST_TYPE, FASTTYPE));
	COMPV_CHECK_CODE_RETURN(fast->setInt(COMPV_FAST_SET_INT_MAX_FEATURES, MAXFEATURES));
	COMPV_CHECK_CODE_RETURN(fast->setBool(COMPV_FAST_SET_BOOL_NON_MAXIMA_SUPP, !!NONMAXIMA));

	// Detect keypoints
	timeStart = CompVTime::nowMillis();
	for (size_t i = 0; i < FAST_LOOP_COUNT; ++i) {
		COMPV_CHECK_CODE_RETURN(fast->process(image, &interestPoints));
	}
	timeEnd = CompVTime::nowMillis();
	COMPV_DEBUG_INFO("Elapsed time (TestFAST) = [[[ %llu millis ]]]", (timeEnd - timeStart));

	// Regression test
#if THRESHOLD == 10
	float sum_scores = 0.f;
	float xf = 0.f;
	float yf = 0.f;
	for (size_t i = 0; i < interestPoints->size(); ++i) {
		sum_scores += interestPoints->ptr(i)->strength;
		xf += interestPoints->ptr(i)->x;
		yf += interestPoints->ptr(i)->y;
	}
#	if NONMAXIMA == 1
	COMPV_CHECK_EXP_RETURN(interestPoints->size() != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_NONMAX_COUNT : FAST12_T10_NONMAX_COUNT), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
	COMPV_CHECK_EXP_RETURN(sum_scores != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_NONMAX_SCORES : FAST12_T10_NONMAX_SCORES), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
	COMPV_CHECK_EXP_RETURN(xf != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_NONMAX_XF : FAST12_T10_NONMAX_XF), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
	COMPV_CHECK_EXP_RETURN(yf != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_NONMAX_YF : FAST12_T10_NONMAX_YF), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
#	else
	COMPV_CHECK_EXP_RETURN(interestPoints->size() != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_CORNERS_COUNT : FAST12_T10_CORNERS_COUNT), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
	COMPV_CHECK_EXP_RETURN(sum_scores != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_CORNERS_SCORES : FAST12_T10_CORNERS_SCORES), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
	COMPV_CHECK_EXP_RETURN(xf != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_XF : FAST12_T10_XF), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
	COMPV_CHECK_EXP_RETURN(yf != ((FASTTYPE == COMPV_FAST_TYPE_9) ? FAST9_T10_YF : FAST12_T10_YF), COMPV_ERROR_CODE_E_UNITTEST_FAILED);
#	endif
#endif
	return COMPV_ERROR_CODE_S_OK;
}