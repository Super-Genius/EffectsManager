

#ifndef GLES_IPHONE_PLATFORM_H
#define GLES_IPHONE_PLATFORM_H

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define GL_GEOMETRY_SHADER 				  0x8DD9
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_1D 					  0x0DE0
#define GL_TEXTURE_MAX_LEVEL			  0x813D
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_WRAP_R                 0x8072

#define GL_APICALL GL_API

void glTexImage1D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * data);
void glTexSubImage1D(GLenum target, GLint level, GLuint xOff, GLsizei width, GLenum format, GLenum type, const GLvoid * data);
void glTexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);
void glTexSubImage3D(GLenum target, GLint level, GLuint xOff, GLuint yOff, GLuint zOff, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data);

const char *gluErrorString(GLenum error);

GL_APICALL void GL_APIENTRY glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GL_APICALL void GL_APIENTRY glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GL_APICALL void GL_APIENTRY glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GL_APICALL void GL_APIENTRY glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GL_APICALL void GL_APIENTRY glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
GL_APICALL void GL_APIENTRY glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

#ifdef DEBUG
#define LOG_INFO(...) printf("Info: %s", __FILE__); \
                      printf(__VA_ARGS__)
#define LOG_ERROR(...) printf("Error: %s", __FILE__); \
                        printf(__VA_ARGS__)
#endif

#endif	// GLES_IPHONE_PLATFORM_H
