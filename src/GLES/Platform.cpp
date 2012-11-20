
#include "Platform.h"

void glTexImage1D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
	glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, 1, border, format, type, data);
}

void glTexSubImage1D(GLenum target, GLint level, GLuint xOff, GLsizei width, GLenum format, GLenum type, const GLvoid * data)
{
	glTexSubImage2D(GL_TEXTURE_2D, level, xOff, 0, width, 1, format, type, data);
}

void glTexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
}

void glTexSubImage3D(GLenum target, GLint level, GLuint xoff, GLuint yoff, GLuint zOff, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid * data)
{
}

const char *gluErrorString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR:
		return "GL_NO_ERROR: No Error has been recorded";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE: A numeric argument is out of range";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The command is trying to render to or read from the framebuffer "
	              "while the currently bound framebuffer is not framebuffer complete (i.e. the return value from"
	              "glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE).";
	case GL_OUT_OF_MEMORY:
	    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
}

GL_APICALL void GL_APIENTRY glUniformMatrix2x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	if (transpose)
	{
		glUniform3fv(location, 1, value);
		glUniform3fv(location+3, 1, value+3);
	}
	else
	{
		glUniform2fv(location, 1, value);
		glUniform2fv(location+2, 1, value+2);
		glUniform2fv(location+4, 1, value+4);
	}
}

GL_APICALL void GL_APIENTRY glUniformMatrix2x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	if (transpose)
	{
		glUniform4fv(location, 1, value);
		glUniform4fv(location+4, 1, value+4);
	}
	else
	{
		glUniform2fv(location, 1, value);
		glUniform2fv(location+2, 1, value+2);
		glUniform2fv(location+4, 1, value+4);
		glUniform2fv(location+6, 1, value+6);
	}
}

GL_APICALL void GL_APIENTRY glUniformMatrix3x4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	if (transpose)
	{
		glUniform4fv(location, 1, value);
		glUniform4fv(location+4, 1, value+4);
		glUniform4fv(location+8, 1, value+8);
	}
	else
	{
		glUniform3fv(location, 1, value);
		glUniform3fv(location+3, 1, value+3);
		glUniform3fv(location+6, 1, value+6);
		glUniform3fv(location+9, 1, value+9);
	}
}

GL_APICALL void GL_APIENTRY glUniformMatrix3x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix2x3fv(location, count, !transpose, value);
}

GL_APICALL void GL_APIENTRY glUniformMatrix4x2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix2x4fv(location, count, !transpose, value);
}

GL_APICALL void GL_APIENTRY glUniformMatrix4x3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix3x4fv(location, count, !transpose, value);

}
