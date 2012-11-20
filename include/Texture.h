#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "IEffectManager.h"
#include "Platform.h"

namespace GLSLEFX
{

class Pass;

class Texture : public EFObject
{
private:
	DECLARE_CLASS(Texture);

	void Load(GLuint xDim, GLuint nLods, void **data, GLuint internalFormat, GLuint format, GLuint type);
	void Load(GLuint xDim, GLuint yDim, GLuint nLods, void **data, GLuint internalFormat, GLuint format, GLuint type);
	void Load(GLuint xDim, GLuint yDim, GLuint zDim, GLuint nLods, void **data, GLuint internalFormat, GLuint format, GLuint type);

public:
	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false);

	// archive the object
	virtual void DoArchive(Archive &ar, bool reading = true);

	/// Sets the
	void SetActive();
	/// Bind the texture in OpenGL
	void Bind();
	/// Unbind the texture in OpenGL
	void Unbind();

	void SetAuxiliaryTexID(GLuint texID) { m_TexID = texID; }

	// activate the texture for usage
	void Activate();

	// deactive the texture
	void Deactivate();

private:
	GLuint m_TexID;				// the GL texture ID allocated for this texture
	std::string m_SourceName;	// the source stream to load from
	GLenum m_GLTextureType;		/// the GL texture type GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D
	GLuint m_TextureUnit;
	GLenum m_sClampMode;		/// the clamp mode for s component of texture
	GLenum m_tClampMode;		/// the clamp mode for s component of texture
	GLenum m_rClampMode;		/// the clamp mode for s component of texture
	GLenum m_MinFilter;			/// the minification type
	GLenum m_MagFilter;			/// the magnification type
	GLuint m_Auxiliary;			/// if auxiliary texture, ID != 0

	static const EnumMap<GLenum> TextureTypes;
	static const EnumMap<GLenum> ClampTypes;
	static const EnumMap<GLenum> MinTypes;
	static const EnumMap<GLenum> MagTypes;

	unsigned int m_TexWidth;
	unsigned int m_TexHeight;
	unsigned int m_TexDepth;


	bool m_bIsInitialized;
};

}

#endif
