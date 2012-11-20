#ifndef _SHADER_MANAGER_ERROR_
#define _SHADER_MANAGER_ERROR_

#define CHECKGLERRORONCE(method)														\
{																						\
	static bool bErrorOnce = false;														\
	GLenum error = glGetError();														\
	if (error)																			\
	{																					\
		if (!bErrorOnce)																\
		{																				\
			LOG_ERROR("%s -> %s (Reported once)", method, gluErrorString(error));		\
			bErrorOnce = true;															\
		}																				\
	}																					\
}

namespace GLSLEFX
{
	void CheckGLError(const char* method);
}

#endif
