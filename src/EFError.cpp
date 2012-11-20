#include "EFError.h"

#include <iostream>
using namespace std;

#include "Platform.h"

namespace GLSLEFX
{
	void CheckGLError(const char* method)
	{
		GLenum error = glGetError();

		if (error)
		{
			LOG_ERROR("%s -> %s", method, gluErrorString(error));
		}
	}
}

