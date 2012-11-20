
#include "IClassManager.h"
#include "EnumMap.h"
#include "Pass.h"
#include "Texture.h"
#include "Shader.h"
#include "TextureIO.h"
#include "EFError.h"
#include "Archive.h"
#include <assert.h>

namespace GLSLEFX
{

REGISTER_CLASS(Texture);

const EnumMap<GLenum> Texture::TextureTypes = EnumMap<GLenum>
    ( "1d", GL_TEXTURE_1D)
    ( "2d",  GL_TEXTURE_2D)
    ( "3d",  GL_TEXTURE_3D) ;

const EnumMap<GLenum> Texture::ClampTypes = EnumMap<GLenum>
	( "clamp", GL_CLAMP_TO_EDGE)
	( "repeat", GL_REPEAT) ;

const EnumMap<GLenum> Texture::MinTypes = EnumMap<GLenum>
	( "nearest", GL_NEAREST)
	( "linear", GL_LINEAR)
	( "near_mip_near", GL_NEAREST_MIPMAP_NEAREST)
	( "linear_mip_near", GL_LINEAR_MIPMAP_NEAREST)
	( "near_mip_linear", GL_NEAREST_MIPMAP_LINEAR)
	( "linear_mip_linear", GL_LINEAR_MIPMAP_LINEAR) ;

const EnumMap<GLenum> Texture::MagTypes = EnumMap<GLenum>
	( "linear", GL_LINEAR)
	( "nearest", GL_NEAREST) ;

Texture::Texture()
{
	m_Auxiliary = 0;
	// default clamp modes = OpenGL default modes
	m_sClampMode = GL_REPEAT;
	m_tClampMode = GL_REPEAT;
	m_rClampMode = GL_REPEAT;

	// default minification filter modes = OpenGL default filter modes
	m_MinFilter = GL_LINEAR;
	// default magnitification filter modes = OpenGL default filter modes
	m_MagFilter = GL_LINEAR;

	// default to 2d texture if not specified
	m_GLTextureType = GL_TEXTURE_2D;

	m_TexID = 0;

	m_bIsInitialized = false;

	m_TexWidth = 0;
	m_TexHeight = 0;
	m_TexDepth = 0;
}

void Texture::Init(bool reInit)
{
	if (reInit)
	{
		m_bIsInitialized = false;
	}

	if (m_Auxiliary == 0)
	{
		// grab texture loading object
		TextureIO *texIO = (TextureIO *)IClassManager::GetInstance()->CreateObject("TextureIO");
		if (!texIO->Load(m_SourceName.c_str(), m_TexID))
		{
			LOG_ERROR("Unable to load %s reason %s\n", m_SourceName.c_str(), texIO->GetLastError());
		}
	}
	else
	{
		IEffectManager *em = IEffectManager::GetInstance();
		m_TexID = em->GetAuxiliaryTextureID(m_Auxiliary);
		if (m_TexID == 0)
		{
			em->LockContext(true);
			glGenTextures(1, &m_TexID);
			em->SetAuxiliaryTextureID(m_Auxiliary, m_TexID);
			em->LockContext(false);
		}

		if (!reInit)
		{
			((Shader *)m_Parent)->AddAuxTexture(m_Auxiliary, this);
		}
	}

	if (!reInit)
	{
		((Shader *)m_Parent)->AddTexture(this);
	}

}

void Texture::DoArchive(Archive &ar, bool reading)
{
	if (reading)
	{
		ar.Read("Source", m_SourceName);
		ar.Read("Stage", m_TextureUnit);
		ar.Read("Auxiliary", m_Auxiliary);
		ar.Read("Type", EnumFilter<GLenum>(TextureTypes, m_GLTextureType));
		ar.Read("SClampmode", EnumFilter<GLenum>(ClampTypes, m_sClampMode));
		ar.Read("TClampmode", EnumFilter<GLenum>(ClampTypes, m_tClampMode));
		ar.Read("RClampmode", EnumFilter<GLenum>(ClampTypes, m_rClampMode));
		ar.Read("MinMode", EnumFilter<GLenum>(MinTypes, m_MinFilter));
		ar.Read("MagMode", EnumFilter<GLenum>(MagTypes, m_MagFilter));
	}
	else
	{
//		ar.Write("Name", m_Name);
	}

	LOG_INFO("Stage: %d, Auxiliary: % d, Type: %x, SClamp: %x, TClamp: %x, RClamp: %x, MinMode: %x, MagMode: %x",
			m_TextureUnit, m_Auxiliary, m_GLTextureType, m_sClampMode, m_tClampMode, m_rClampMode, m_MinFilter, m_MagFilter);
}

void Texture::SetActive()
{
	if (m_TexID != 0)
	{
		IEffectManager::GetInstance()->LockContext(true);
		glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
		CHECKGLERRORONCE("Texture::SetActive");
		IEffectManager::GetInstance()->LockContext(false);
	}

}

void Texture::Bind()
{
	if (m_TexID != 0)
	{
		IEffectManager::GetInstance()->LockContext(true);

		glBindTexture(m_GLTextureType, m_TexID);
		CHECKGLERRORONCE("Texture::Bind");

		glTexParameteri(m_GLTextureType, GL_TEXTURE_WRAP_S, m_sClampMode);
		CHECKGLERRORONCE("Texture::Bind::TextureWrapS");
		if (m_GLTextureType != GL_TEXTURE_1D)
		{
			glTexParameteri(m_GLTextureType, GL_TEXTURE_WRAP_T, m_tClampMode);
			CHECKGLERRORONCE("Texture::Bind::TextureWrapT");
		}

		if (m_GLTextureType == GL_TEXTURE_3D)
		{
			glTexParameteri(m_GLTextureType, GL_TEXTURE_WRAP_R, m_rClampMode);
			CHECKGLERRORONCE("Texture::Bind::TextureWrapR");
		}

		glTexParameteri(m_GLTextureType, GL_TEXTURE_MIN_FILTER, m_MinFilter);
		CHECKGLERRORONCE("Texture::Bind::TextureMinFilter");
		glTexParameteri(m_GLTextureType, GL_TEXTURE_MAG_FILTER, m_MagFilter);
		CHECKGLERRORONCE("Texture::Bind::TextureMagFilter");
		IEffectManager::GetInstance()->LockContext(false);
	}

}

void Texture::Unbind()
{
	if (m_TexID != 0)
	{
		IEffectManager::GetInstance()->LockContext(true);
		glBindTexture(m_GLTextureType, 0);
		CHECKGLERRORONCE("Texture::Unbind::");
		IEffectManager::GetInstance()->LockContext(false);
	}
}


void Texture::Load(GLuint xDim, GLuint nLods, void **data, GLuint internalFormat, GLuint format, GLuint type)
{
	if (m_TexID == 0)
	{
		LOG_ERROR("Texture::Load 1D: Can't load texture without it being created first");
		return;
	}

	if (m_GLTextureType != GL_TEXTURE_2D)
	{
		LOG_ERROR("Texture::Load 1D: Can't load a non 1D (2D, 3D) texture into a 1D texture");
		return;
	}

	IEffectManager::GetInstance()->LockContext(true);

	assert(m_GLTextureType == GL_TEXTURE_1D);

	SetActive();
	Bind();

	for(GLuint i=0 ; i < nLods ; i++)
	{
		if (!m_bIsInitialized)
		{
			m_TexWidth = xDim;
			glTexImage1D( m_GLTextureType, i, internalFormat, xDim, 0, format, type, data[i]);
		}
		else
		{
			if (m_TexWidth != xDim)
			{
				LOG_INFO("Texture::Load1d:: Dimensions %d doesn't match creation dimension of %d", xDim, m_TexWidth);
			}
			glTexSubImage1D( m_GLTextureType, i, 0, xDim, format, type, data[i]);
		}
		CheckGLError("Texture1D::load");
	}

	m_bIsInitialized = true;
	Unbind();
	IEffectManager::GetInstance()->LockContext(false);
}

void Texture::Activate()
{
	SetActive();
	Bind();
}

void Texture::Deactivate()
{
	SetActive();
	Unbind();
}

void Texture::Load(GLuint xDim, GLuint yDim, GLuint nLods, void **data, GLuint internalFormat, GLuint format, GLuint type)
{
	if (m_TexID == 0)
	{
		LOG_ERROR("Texture::Load 2D: Can't load texture without it being created first");
		return;
	}
	if (m_GLTextureType != GL_TEXTURE_2D)
	{
		LOG_ERROR("Texture::Load 2D: Can't load a non 2D (1D, 3D) texture into a 2D texture");
		return;
	}

	static int count = 249;
	if (++count == 250)
	{
		LOG_INFO("Loading 2D texture auxID(%d), textID(%d)", m_Auxiliary, m_TexID);
		count = 0;
	}
	IEffectManager::GetInstance()->LockContext(true);

	SetActive();
	Bind();

	for(GLuint i=0 ; i < nLods ; i++)
	{
		if (!m_bIsInitialized)
		{
			glTexImage2D( m_GLTextureType, i, internalFormat, xDim, yDim, 0, format, type, data[i]);
			m_TexWidth = xDim;
			m_TexHeight = yDim;
		}
		else
		{
			if ((m_TexWidth != xDim) || (m_TexHeight != yDim))
			{
				LOG_INFO("Texture::Loaddd:: Dimensions %d,%d doesn't match creation dimension of %d,%d",
						xDim, yDim, m_TexWidth, m_TexHeight);
			}

			glTexSubImage2D( m_GLTextureType, i, 0, 0, xDim, yDim, format, type, data[i]);

		}
		//CheckGLError("Texture2D::load");
		if (glGetError())
		{
			LOG_ERROR("Error load texture ID(auxID): %d(%d), format: 0x%x, width: %d, height: %d", m_TexID, m_Auxiliary, format, xDim, yDim);
		}
	}

	m_bIsInitialized = true;
	Unbind();
	IEffectManager::GetInstance()->LockContext(false);
}

void Texture::Load(GLuint xDim, GLuint yDim, GLuint zDim, GLuint nLods, void **data, GLuint internalFormat, GLuint format, GLuint type)
{

	if (m_TexID == 0)
	{
		LOG_ERROR("Texture::Load 3D: Can't load texture without it being created first");
		return;
	}
	if (m_GLTextureType != GL_TEXTURE_2D)
	{
		LOG_ERROR("Texture::Load 3D: Can't load a non 3D (1D, 2D) texture into a 3D texture");
		return;
	}

	IEffectManager::GetInstance()->LockContext(true);

	SetActive();
	Bind();

	for(GLuint i=0 ; i < nLods ; i++)
	{
		if (!m_bIsInitialized)
		{
			glTexImage3D( m_GLTextureType, i, internalFormat, xDim, yDim, zDim, 0, format, type, data[i]);
			m_TexWidth = xDim;
			m_TexHeight = yDim;
			m_TexDepth = zDim;
		}
		else
		{
			if ((m_TexWidth != xDim) || (m_TexHeight != yDim) || m_TexDepth != zDim)
			{
				LOG_INFO("Texture::Load3d:: Dimensions %d,%d doesn't match creation dimension of %d,%d",
					xDim, yDim, zDim, m_TexWidth, m_TexHeight, m_TexDepth);
			}
			glTexSubImage3D( m_GLTextureType, i, 0, 0, 0, xDim, yDim, zDim, format, type, data[i]);
		}
		CheckGLError("Texture3D::load");
	}
	Unbind();
	IEffectManager::GetInstance()->LockContext(false);
}
}
