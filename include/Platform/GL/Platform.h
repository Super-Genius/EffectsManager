

#ifndef GL_PLATFORM_H
#define GL_PLATFORM_H

//#define GL_VERSION_2_0
#define GL_GLEXT_PROTOTYPES
//#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <stdio.h>

#ifdef _DEBUG
#define LOG_INFO(...) printf("%s (Info): ", __FILE__);			\
						printf(__VA_ARGS__);					\
						printf("\n");

#define LOG_ERROR(...) printf("%s (Error): ", __FILE__);			\
						printf(__VA_ARGS__);					\
						printf("\n");
#else
#define LOG_INFO(...)
#define LOG_ERROR(...)
#endif

#endif	// GL_PLATFORM_H
