/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#include "compv/ui/opengl/compv_window_sdl.h"
#if defined(HAVE_SDL_H)
#include "compv/ui/compv_ui.h"

#if defined(HAVE_GL_GLEW_H)
#include <GL/glew.h>
#endif /* HAVE_GL_GLEW_H */

COMPV_NAMESPACE_BEGIN()

CompVWindowSDL::CompVWindowSDL(int width, int height, const char* title)
	: CompVWindow(width, height, title)
	, m_pSDLWindow(NULL)
	, m_pSDLContext(NULL)
{
#   if COMPV_OS_APPLE
	if (!pthread_main_np()) {
		COMPV_DEBUG_WARN("MacOS: Creating window outside main thread");
	}
#   endif /* COMPV_OS_APPLE */
	m_pSDLWindow = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	if (!m_pSDLWindow) {
		COMPV_DEBUG_ERROR("SDL_CreateWindow(%d, %d, %s) failed: %s", width, height, title, SDL_GetError());
		return;
	}
	m_pSDLContext = SDL_GL_CreateContext(m_pSDLWindow);
	if (!m_pSDLContext) {
		COMPV_DEBUG_ERROR("SDL_GL_CreateContext() failed: %s", SDL_GetError());
		return;
	}
	
	COMPV_CHECK_CODE_ASSERT(CompVMutex::newObj(&m_SDLMutex));
	SDL_SetWindowData(m_pSDLWindow, "This", this);
	COMPV_ASSERT(SDL_GL_MakeCurrent(m_pSDLWindow, m_pSDLContext) == 0);
	SDL_GL_SetSwapInterval(1);
	SDL_SetEventFilter(CompVWindowSDL::FilterEvents, this);
	SDL_GL_MakeCurrent(m_pSDLWindow, NULL);
}

CompVWindowSDL::~CompVWindowSDL()
{
	COMPV_CHECK_CODE_ASSERT(close());
	m_SDLMutex = NULL;
	m_Program = NULL;
}

bool CompVWindowSDL::isClosed()
{
	return !m_pSDLWindow;
}

COMPV_ERROR_CODE CompVWindowSDL::close()
{
	COMPV_CHECK_CODE_ASSERT(unregister());
	COMPV_CHECK_CODE_ASSERT(m_SDLMutex->lock());
	if (m_pSDLContext) {
		SDL_GL_DeleteContext(m_pSDLContext);
		m_pSDLContext = NULL;
	}
	if (m_pSDLWindow) {
		SDL_SetWindowData(m_pSDLWindow, "This", NULL);
		SDL_DestroyWindow(m_pSDLWindow);
		m_pSDLWindow = NULL;
	}
	COMPV_CHECK_CODE_ASSERT(m_SDLMutex->unlock());
	return COMPV_ERROR_CODE_S_OK;
}

COMPV_ERROR_CODE CompVWindowSDL::draw(CompVMatPtr mat)
{
	COMPV_CHECK_EXP_RETURN(!mat || mat->isEmpty(), COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	COMPV_ASSERT(mat->subType() == COMPV_MAT_SUBTYPE_PIXELS_R8G8B8);
	COMPV_CHECK_CODE_ASSERT(m_SDLMutex->lock());
	if (!m_pSDLWindow || !m_pSDLContext) {
		COMPV_CHECK_CODE_ASSERT(m_SDLMutex->unlock());
		// COMPV_DEBUG_INFO("Window closed. Ignoring draw() instruction");
		return COMPV_ERROR_CODE_W_WINDOW_CLOSED;
	}
	/*if (!glfwWindowShouldClose(m_pGLFWwindow))*/ {
		COMPV_ASSERT(SDL_GL_MakeCurrent(m_pSDLWindow, m_pSDLContext) == 0);
#if 1
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor((GLclampf)(rand() % 255) / 255.f,
			(GLclampf)(rand() % 255) / 255.f,
			(GLclampf)(rand() % 255) / 255.f,
			(GLclampf)(rand() % 255) / 255.f);
		SDL_GL_SwapWindow(m_pSDLWindow);
		SDL_GL_MakeCurrent(m_pSDLWindow, NULL);
#else
		if (!m_Program) {
			COMPV_CHECK_CODE_ASSERT(CompVProgram::newObj(&m_Program));
			COMPV_CHECK_CODE_ASSERT(m_Program->shadAttachVertexFile("C:/Projects/GitHub/compv/ui/glsl/test.vert.glsl"));
			COMPV_CHECK_CODE_ASSERT(m_Program->shadAttachFragmentFile("C:/Projects/GitHub/compv/ui/glsl/test.frag.glsl"));
			COMPV_CHECK_CODE_ASSERT(m_Program->link());
		}

		static GLuint tex = 0;
		if (!tex) {
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			/* OpenGL-2 or later is assumed; OpenGL-2 supports NPOT textures. */
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if ((mat->stride() & 3)) { // multiple of 4?
				glPixelStorei(GL_UNPACK_ROW_LENGTH, (GLint)mat->stride());
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			}

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				static_cast<GLsizei>(mat->stride()),
				static_cast<GLsizei>(mat->rows()),
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glBindTexture(GL_TEXTURE_2D, tex);
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
		glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			static_cast<GLsizei>(mat->stride()),
			static_cast<GLsizei>(mat->rows()),
			GL_RGB,
			GL_UNSIGNED_BYTE,
			mat->ptr());

		COMPV_CHECK_CODE_ASSERT(m_Program->useBegin());

		glClearColor(0.f, 0.f, 0.f, 1.f);
		int width, height;
		SDL_GL_GetDrawableSize(m_pSDLWindow, &width, &height);
		glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluOrtho2D((GLdouble)0, static_cast<GLdouble>(mat->stride()), static_cast<GLdouble>(mat->rows()), (GLdouble)0); 
		glOrtho((GLdouble)0, static_cast<GLdouble>(mat->stride()), static_cast<GLdouble>(mat->rows()), (GLdouble)0, (GLdouble)-1, (GLdouble)1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2i(0, 0);
		glTexCoord2i(0, 1);
		glVertex2i(0, static_cast<GLint>(mat->rows()));
		glTexCoord2i(1, 1);
		glVertex2i(static_cast<GLint>(mat->stride()), static_cast<GLint>(mat->rows()));
		glTexCoord2i(1, 0);
		glVertex2i(static_cast<GLint>(mat->stride()), 0);
		glEnd();

		COMPV_CHECK_CODE_ASSERT(m_Program->useEnd());

		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_GL_SwapWindow(m_pSDLWindow);
		SDL_GL_MakeCurrent(m_pSDLWindow, NULL);
#endif		
	}
	COMPV_CHECK_CODE_ASSERT(m_SDLMutex->unlock());

	return COMPV_ERROR_CODE_S_OK;
}

COMPV_ERROR_CODE CompVWindowSDL::newObj(CompVWindowSDLPtrPtr sdlWindow, int width, int height, const char* title)
{
	COMPV_CHECK_CODE_RETURN(CompVUI::init());
	COMPV_CHECK_EXP_RETURN(sdlWindow == NULL || width <= 0 || height <= 0 || !title || !::strlen(title), COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	CompVWindowSDLPtr sdlWindow_ = new CompVWindowSDL(width, height, title);
	COMPV_CHECK_EXP_RETURN(!sdlWindow_, COMPV_ERROR_CODE_E_OUT_OF_MEMORY);
	COMPV_CHECK_EXP_RETURN(!sdlWindow_->m_pSDLWindow, COMPV_ERROR_CODE_E_GLFW);
	COMPV_CHECK_EXP_RETURN(!sdlWindow_->m_pSDLContext, COMPV_ERROR_CODE_E_GLFW);
	COMPV_CHECK_EXP_RETURN(!sdlWindow_->m_SDLMutex, COMPV_ERROR_CODE_E_SYSTEM);
	*sdlWindow = sdlWindow_;
	return COMPV_ERROR_CODE_S_OK;
}

int CompVWindowSDL::FilterEvents(void *userdata, SDL_Event* event)
{
	/*if (event->type == SDL_WINDOWEVENT) {
		CompVWindowSDLPtr This = static_cast<CompVWindowSDL*>(userdata);
		switch (event->window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			COMPV_CHECK_CODE_ASSERT(This->m_SDLMutex->lock());
			COMPV_CHECK_CODE_ASSERT(This->unregister());
			COMPV_CHECK_CODE_ASSERT(This->close());
			COMPV_CHECK_CODE_ASSERT(This->m_SDLMutex->unlock());
			return 0;
		default:
			break;
		}
	}*/
	return (1);
}

COMPV_NAMESPACE_END()

#endif /* HAVE_SDL_H */
