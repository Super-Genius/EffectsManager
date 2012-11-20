

#include "Effect.h"
#include "Pass.h"
#include "Technique.h"
#include "Shader.h"
#include "Uniform.h"
#include "Texture.h"
#include "Archive.h"
#include "EFError.h"

namespace GLSLEFX
{

REGISTER_CLASS(Pass);

Pass::Pass()
{
	m_bDidSpecifyPassNum = false;
	m_ProgramID = 0;
}

Pass::~Pass()
{
	//parent->RemovePass(this);
	SHADERMAP::iterator it;
	for (it = m_Shaders.begin(); it != m_Shaders.end(); ++it)
	{
		delete it->second;
	}
}

void Pass::Init(bool reInit)
{
	if (reInit)
	{
		m_ProgramID = 0;
		// attach program ID to the uniforms
		for (SHADERMAP::iterator slIter = m_Shaders.begin(); slIter != m_Shaders.end(); ++slIter)
		{
			Shader *curShader = slIter->second;
			if (reInit)
			{
				curShader->Init(true);

			}
		}
	}

	CreateProgram();

	// attach program ID to the uniforms
	for (SHADERMAP::iterator slIter = m_Shaders.begin(); slIter != m_Shaders.end(); ++slIter)
	{
		Shader *curShader = slIter->second;
		curShader->SyncUniforms(m_ProgramID);
	}

	if (!reInit)
	{
		((Technique *)m_Parent)->AddPass(this, GetPassNum());
	}
}

void Pass::DoArchive(Archive &ar, bool reading)
{
	if (reading)
	{
		if (ar.Read("Number", m_PassNum))
		{
			m_bDidSpecifyPassNum = true;
		}
	}
	else
	{
		if (m_bDidSpecifyPassNum)
		{
			ar.Write("Number", m_PassNum);
		}
	}
}

unsigned int Pass::GetPassNum()
{
	if (m_bDidSpecifyPassNum)
	{
		return m_PassNum;
	}
	else
	{
		return ((Technique *)m_Parent)->GetNumPasses();
	}
}

void Pass::AddShader(Shader *shader)
{
	m_Shaders[shader->GetShaderTypeString()] = shader;
}

void Pass::CreateProgram()
{
	IEffectManager::GetInstance()->LockContext(true);
	if (m_ProgramID != 0)
	{
		glDeleteProgram(m_ProgramID);
	    CheckGLError("Pass:DeleteProgram");
	}


	LOG_INFO("Creating Program from %d shaders", m_Shaders.size());

    // create a program
    m_ProgramID = glCreateProgram();
    CheckGLError("Pass:CreateProgram");

    if (m_ProgramID == 0)
    {
		LOG_ERROR("Unable to create a shader program?");
    }
    else
    {
		// attach the vertex and fragment shader codes, and the geometric if available
		for (SHADERMAP::iterator slIter = m_Shaders.begin(); slIter != m_Shaders.end(); ++slIter)

		{
			Shader *curShader = slIter->second;
			glAttachShader(m_ProgramID, curShader->GetShaderID());
		}

		// link
		glLinkProgram(m_ProgramID);

		// check link status
		GLint status = GL_FALSE;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			// The link has failed, check log info
			int logLength = 1;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logLength);

			char* infoLog = (char*)malloc(logLength+1);
			glGetProgramInfoLog(m_ProgramID, logLength, &logLength, infoLog);
			LOG_ERROR("Failed to link the shader: %s", infoLog);
			free(infoLog);
		}
		else
		{
			// check if the shader will run in the current OpenGL state
			glValidateProgram(m_ProgramID);
			glGetProgramiv(m_ProgramID, GL_VALIDATE_STATUS, &status);
			if (status != GL_TRUE)
			{
				// log error somewhere
				LOG_ERROR("Shader program will not run in this OpenGL environment!\n");
			}
		}
    }

	IEffectManager::GetInstance()->LockContext(false);
}

void Pass::Activate()
{
	IEffectManager::GetInstance()->LockContext(true);

	// activate the shader program
	glUseProgram(m_ProgramID);
    for (SHADERMAP::iterator slIter = m_Shaders.begin(); slIter != m_Shaders.end(); ++slIter)
    {
    	Shader *curShader;

    	curShader = slIter->second;

    	curShader->Activate();
    }

	IEffectManager::GetInstance()->LockContext(false);
}

void Pass::Deactivate()
{
    for (SHADERMAP::iterator slIter = m_Shaders.begin(); slIter != m_Shaders.end(); ++slIter)
    {
    	Shader *curShader;

    	curShader = slIter->second;

    	curShader->Deactivate();
    }
	IEffectManager::GetInstance()->LockContext(true);
	glUseProgram(0);
	IEffectManager::GetInstance()->LockContext(false);

}

// grab a map of shaders for this pass
const Pass::SHADERMAP *Pass::GetShaders()
{
	return &m_Shaders;
}

// Tests if the shader is valid (it is considered valid if the shader compiles)
bool Pass::IsValid() const
{
	return true;
}

bool Pass::SetAuxiliaryTextureID(unsigned int auxID, unsigned int texID)
{
	std::pair<AUXTEXTUREMAP::const_iterator, AUXTEXTUREMAP::const_iterator> atmRange;
	atmRange = m_AuxTextures.equal_range(auxID);
	if (atmRange.first != atmRange.second)
	{
		for (AUXTEXTUREMAP::const_iterator atmIter = atmRange.first; atmIter != atmRange.second; ++atmIter)
		{
			Texture *tex = atmIter->second;
			tex->SetAuxiliaryTexID(texID);
		}
		return true;
	}

	return false;
}

void Pass::AddAuxTexture(unsigned int auxID, Texture *auxTexture)
{
	// multimap, so use insert
	m_AuxTextures.insert(std::pair<unsigned int, Texture *>(auxID, auxTexture));
	LOG_INFO("Pass added auxiliary texture ID: %d", auxID);
}


void Pass::UpdateAuxiliaryTextures(unsigned int auxID, unsigned int format, unsigned int width, unsigned int height, unsigned char *data, unsigned long size)
{
	// only need one texture object to load this data
	AUXTEXTUREMAP::const_iterator atmIter = m_AuxTextures.find(auxID);
	if (atmIter != m_AuxTextures.end())
	{
		Texture *tex = atmIter->second;
		tex->Load(width, height, 1, (void **)&data, format, format, GL_UNSIGNED_BYTE);
	}
	else
	{
		LOG_INFO("Could not find auxid(%d) with %d, height %d", auxID, width, height);
	}
}

// set uniforms outside the effects manager
bool Pass::SetUniform(const char *type, const char *name, unsigned int count, float *values)
{
	SHADERMAP::iterator smIter = m_Shaders.find(type);
	if (smIter != m_Shaders.end())
	{
		Shader *curShader = smIter->second;
		return curShader->SetUniform(name, count, values);
	}

	return false;
}

bool Pass::SetUniform(const char *type, const char *name, unsigned int count, int *values)
{
	SHADERMAP::iterator smIter = m_Shaders.find(type);
	if (smIter != m_Shaders.end())
	{
		Shader *curShader = smIter->second;
		return curShader->SetUniform(name, count, values);
	}

	return false;
}


}
