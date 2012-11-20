

#ifndef _PASS_H_
#define _PASS_H_ 1

#include "IEffectManager.h"
#include <list>
#include <string>
#include <map>

#include "Platform.h"
#include <map>
namespace GLSLEFX
{

// forward reference
class Shader;
class Technique;
class Uniform;
class Texture;

class Pass : public EFObject
{
private:
	DECLARE_CLASS(Pass);
	void CreateProgram();
	typedef std::multimap<unsigned int, Texture *> AUXTEXTUREMAP;

public:
	virtual ~Pass();

	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false);

	// archive the object
	virtual void DoArchive(Archive &ar, bool reading = true);

	typedef std::map<std::string, Shader *> SHADERMAP;

	void Activate();
	void Deactivate();

	// add a new shader to the pass
	void AddShader(Shader *shader);

	// add auxiliary textures to pass
	void AddAuxTexture(unsigned int auxID, Texture *auxTexture);
	bool SetAuxTextureID(unsigned int auxID, unsigned int texID);

	void UpdateAuxiliaryTextures(unsigned int auxID, unsigned int format, unsigned int width, unsigned int height, unsigned char *data, unsigned long size);

	// grab a list of shaders for this pass
	const SHADERMAP *GetShaders();

	// get our pass number
	unsigned int GetPassNum();

	GLuint GetProgramID() { return m_ProgramID; }

	bool SetAuxiliaryTextureID(unsigned int auxID, unsigned int texID);

	// Tests if the shaders are valid (it is considered valid if the shaders all compile)
	bool IsValid() const;

	// set uniforms outside the effects manager
	bool SetUniform(const char *type, const char *name, unsigned int count, float *values);
	bool SetUniform(const char *type, const char *name, unsigned int count, int *values);


private:
	AUXTEXTUREMAP m_AuxTextures;

	/// list of shaders for this pass
	SHADERMAP m_Shaders;
	/// the program ID from OpenGL
	GLuint m_ProgramID;

	// if we specified the pass number of this pass
	bool m_bDidSpecifyPassNum;

	// pass number assigned to us
	unsigned int m_PassNum;

};

}

#endif
