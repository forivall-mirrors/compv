/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/

#include "compv/gl/compv_gl.h"
#include "compv/gl/compv_gl_headers.h"
#include "compv/gl/compv_gl_vao.h"
#include "compv/base/compv_debug.h"
#include "compv/base/compv_base.h"

COMPV_NAMESPACE_BEGIN()

bool CompVGL::s_bInitialized = false;

COMPV_ERROR_CODE CompVGL::init()
{
	if (CompVGL::s_bInitialized) {
		return COMPV_ERROR_CODE_S_OK;
	}

	COMPV_ERROR_CODE err = COMPV_ERROR_CODE_S_OK;

	COMPV_DEBUG_INFO("Initializing [gl] module (v %s)...", COMPV_VERSION_STRING);

	COMPV_CHECK_CODE_BAIL(err = CompVBase::init());

	/* Print Android API version */
#if COMPV_OS_ANDROID
	COMPV_DEBUG_INFO("[GL] module: android API version: %d", __ANDROID_API__);
#endif

#if defined(HAVE_GL_GLEW_H)
	COMPV_DEBUG_INFO("GLEW version being used: %d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
#endif /* HAVE_GL_GLEW_H */

#if	defined(HAVE_OPENGLES)
	COMPV_DEBUG_INFO("OpenGL-ES implementation enabled");
#	if defined(GL_ES_VERSION_3_0) && GL_ES_VERSION_3_0
		COMPV_DEBUG_INFO("OpenGL-ES 3.0 API");
#	elif defined(GL_ES_VERSION_2_0) && GL_ES_VERSION_2_0
		COMPV_DEBUG_INFO("OpenGL-ES 2.0 API");
#	else
		COMPV_DEBUG_INFO("OpenGL-ES 1.0 API");
#	endif
#elif defined(HAVE_OPENGL)
	COMPV_DEBUG_INFO("OpenGL implementation enabled");
#endif

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	COMPV_CHECK_CODE_BAIL(err = CompVGLVAO::init());
#endif

	CompVGL::s_bInitialized = true;

bail:
	if (COMPV_ERROR_CODE_IS_NOK(err)) {
		COMPV_CHECK_CODE_ASSERT(CompVGL::deInit());
	}
	return err;
}
	
COMPV_ERROR_CODE CompVGL::deInit()
{
	COMPV_CHECK_CODE_ASSERT(CompVBase::deInit());

#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
	COMPV_CHECK_CODE_ASSERT(CompVGLVAO::deInit());
#endif

	CompVGL::s_bInitialized = false;

	return COMPV_ERROR_CODE_S_OK;
}

COMPV_NAMESPACE_END()
