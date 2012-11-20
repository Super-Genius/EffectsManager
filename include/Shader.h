#ifndef _SHADER_H_
#define _SHADER_H_

#include "IEffectManager.h"
#include <vector>
#include <string>

#include "Platform.h"

namespace GLSLEFX
{

class Uniform;
class Texture;
class Pass;

class Shader : public EFObject
{
private:
	DECLARE_CLASS(Shader);
	typedef std::vector<Texture *> VECTEXTURES;
	typedef std::map<std::string, Uniform *> UNIFORMMAP;

	void LoadShader();

public:
	virtual ~Shader();

	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false);

	virtual void DoArchive(Archive &ar, bool reading = true);

	void Activate();
	void Deactivate();

	// shader texture
	void AddTexture(Texture* Texture);

	// auxiliary textures
	void AddAuxTexture(unsigned int auxID, Texture *texture);

	GLuint GetShaderID() { return m_ShaderID; }

	// shader uniform variables
	void AddUniform(Uniform *uniform);
	Uniform *GetUniform(const char *name);

	// set uniforms outside the effects manager
	bool SetUniform(const char *name, unsigned int count, float *values);
	bool SetUniform(const char *name, unsigned int count, int *values);

	void LoadUniforms();
	void SyncUniforms(GLuint programID);

	GLuint GetShaderType() { return m_ShaderType; }

	const char *GetShaderTypeString() { return m_ShaderTypeString.c_str(); }


private:
	UNIFORMMAP m_Uniforms;

	VECTEXTURES m_Textures;

	GLenum	m_ShaderType;
	std::string m_SourceName;
	std::string m_Source;
	GLuint	m_ShaderID;
	std::string m_ShaderTypeString;

};

}

#endif
