
#include "Pass.h"
#include "Shader.h"
#include "Uniform.h"
#include "Texture.h"
#include "EnumMap.h"
#include "IFileManager.h"
#include "Archive.h"
#include "EFError.h"

namespace GLSLEFX
{

REGISTER_CLASS(Shader);

static const EnumMap<GLenum> SHADERTYPE = EnumMap<GLenum>
	( "fragment" , GL_FRAGMENT_SHADER )
	( "vertex", GL_VERTEX_SHADER )
	( "geometry", GL_GEOMETRY_SHADER )
	( "control", GL_TESS_CONTROL_SHADER )
	( "evaluation", GL_TESS_EVALUATION_SHADER );

Shader::Shader()
{
	m_ShaderType = (GLenum)-1;
	m_ShaderID = 0;
}

void Shader::Init(bool reInit)
{
	if (reInit)
	{
		// activate the m_Texturess for this shader
		for (unsigned int i=0 ; i<m_Textures.size() ; ++i)
		{
			m_Textures[i]->Init(true);
		}

		for (UNIFORMMAP::iterator uIter = m_Uniforms.begin(); uIter != m_Uniforms.end(); ++uIter)
		{
			Uniform *curUniform = uIter->second;

			curUniform->Init(true);
		}
	}
	else
	{
		((Pass *)m_Parent)->AddShader(this);

		IFileManager * fm = IFileManager::GetInstance();
		if (!fm->Load(m_SourceName.c_str(), m_Source))
		{
			LOG_ERROR("Unable to load file %s, Reason: %s", m_SourceName.c_str(), fm->GetLastError());
		}
	}

	if ((m_ShaderType == (GLenum)-1))
	{
		LOG_ERROR("The Shader Type has not been set");
		return;
	}

	if (m_SourceName.empty() )
	{
		LOG_ERROR("No file to load?\n");
		// log error here
		return;
	}

    // will load a blank shader if error above.
	LoadShader();

}

void Shader::DoArchive(Archive &ar, bool reading)
{
	if (reading)
	{
		ar.Read("Source", m_SourceName);
		ar.Read("Type", EnumFilter<GLenum>(SHADERTYPE, m_ShaderType));
		EnumMap<GLenum>::const_iterator emIter;
		m_ShaderTypeString = "UNKNOWN";
		for (emIter = SHADERTYPE.begin(); emIter != SHADERTYPE.end(); ++emIter)
		{
			if (m_ShaderType == emIter->second)
			{
				m_ShaderTypeString = emIter->first;
			}
		}
	}
	else
	{
		ar.Write("Source", m_SourceName);
		ar.Read("Type", EnumFilter<GLenum>(SHADERTYPE, m_ShaderType));
	}
}

Shader::~Shader()
{

}

void Shader::Activate()
{
	// activate the m_Texturess for this shader
	for (unsigned int i=0 ; i<m_Textures.size() ; ++i)
	{
		m_Textures[i]->Activate();
	}
	
	LoadUniforms();

}
void Shader::Deactivate()
{
	// deactivate m_Textures
	for(unsigned int i=0 ; i<m_Textures.size() ; ++i)
	{
		m_Textures[i]->Deactivate();
	}
}

void Shader::AddAuxTexture(unsigned int auxID, Texture *texture)
{
	((Pass *)m_Parent)->AddAuxTexture(auxID, texture);
}

void Shader::SyncUniforms(GLuint programID)
{
	for (UNIFORMMAP::iterator uIter = m_Uniforms.begin(); uIter != m_Uniforms.end(); ++uIter)
	{
		Uniform *curUniform = uIter->second;

		curUniform->SyncID(programID);
	}
}

void Shader::LoadUniforms()
{
	for (UNIFORMMAP::iterator uIter = m_Uniforms.begin(); uIter != m_Uniforms.end(); ++uIter)
	{
		Uniform *curUniform = uIter->second;

		curUniform->Load();
	}
}

void Shader::AddTexture(Texture *texture)
{
	m_Textures.push_back(texture);
}

void Shader::LoadShader()
{
    GLsizei logsize = 0;
    GLint compile_status = GL_TRUE;
    char *log = NULL;

	IEffectManager::GetInstance()->LockContext(true);

	LOG_INFO("Loading Shader %s", m_SourceName.c_str());

    // Create a shader of type specified
    m_ShaderID = glCreateShader(m_ShaderType);
    if (m_ShaderID == 0)
    {
        LOG_ERROR("Could not create the shader of type %d\n", m_ShaderType);
    	IEffectManager::GetInstance()->LockContext(false);
        return;
    }
    
    // give the shader source code to OpenGL
    const GLchar *source = m_Source.c_str();
    glShaderSource(m_ShaderID, 1, &source, NULL);
    CheckGLError("Shader::LoadShader::ShaderSource");
    // and compile it for OpenGL
    glCompileShader(m_ShaderID);
    CheckGLError("Shader::LoadShader::CompileShader");
    
    // verify that the shaders compiled
    glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        // There was an error on compile
        
        // grab the information of the error, if any
        glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &logsize);

        // allocate the log if available
        log = (char*)malloc(logsize + 1);
        if (log == NULL)
        {
            LOG_ERROR("Out of Memory allocating the log file!\n");
        	IEffectManager::GetInstance()->LockContext(false);
            return;
        }
        
        glGetShaderInfoLog(m_ShaderID, logsize, &logsize, log);

        LOG_ERROR("Shader compile Error: %s", log);
        // Need easy cross platform logging system
        free(log);
        
        glDeleteShader(m_ShaderID);
        
        m_ShaderID = 0;
    }
	IEffectManager::GetInstance()->LockContext(false);
}

void Shader::AddUniform(Uniform *uniform)
{
	m_Uniforms[uniform->GetName()] = uniform;
}

Uniform *Shader::GetUniform(const char *name)
{
	Uniform *retUniform = NULL;
	UNIFORMMAP::iterator uIter = m_Uniforms.find(name);

	if (uIter != m_Uniforms.end())
	{
		retUniform = uIter->second;
	}

	return retUniform;
}

bool Shader::SetUniform(const char *name, unsigned int count, float *values)
{
	UNIFORMMAP::iterator uIter = m_Uniforms.find(name);
	if (uIter == m_Uniforms.end())
	{
		//LOG_ERROR("Uniform name not found %s, in SetUniform", name);
		return false;
	}
	else
	{
		Uniform *curUniform = uIter->second;
		curUniform->SetValues(count, values);
	}

	return true;
}

bool Shader::SetUniform(const char *name, unsigned int count, int *values)
{
	UNIFORMMAP::iterator uIter = m_Uniforms.find(name);
	if (uIter == m_Uniforms.end())
	{
		//LOG_ERROR("Uniform name not found %s, in SetUniform", name);
		return false;
	}
	else
	{
		Uniform *curUniform = uIter->second;
		curUniform->SetValues(count, values);
	}

	return true;
}


}
