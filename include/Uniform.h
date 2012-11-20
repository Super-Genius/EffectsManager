#ifndef _UNIFORM_H_
#define _UNIFORM_H_

#include "IEffectManager.h"
#include "Platform.h"
#include "EnumMap.h"
#include <string>
#include <map>

namespace GLSLEFX
{
class Pass;

class Uniform : public EFObject
{
private:
	DECLARE_CLASS(Uniform);

	typedef enum UNIFORM_TYPE
	{
		INT1,
		INT2,
		INT3,
		INT4,
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MATRIX2X2,
		MATRIX2X3,
		MATRIX2X4,
		MATRIX3X2,
		MATRIX3X3,
		MATRIX3X4,
		MATRIX4X2,
		MATRIX4X3,
		MATRIX4X4,
		COUNT
	} UNIFORM_TYPE;

	typedef void (*GLUNIFORMMATFUNC)(GLint  location,  GLsizei  count,  GLboolean  transpose,  const GLfloat *values);
	typedef void (*GLUNIFORMFUNC)(GLint locataion, GLsizei count,  const void *values);

	typedef struct UNIFORMFUNCINFO
	{
		bool	m_AreFloatValues;
		bool	m_IsMatrixFunc;
		GLuint	m_NumValues;
		void	*m_UniformFunc;
	} UNIFORMFUNCINFO;

	class UniformFuncMap : public std::map<UNIFORM_TYPE, const UNIFORMFUNCINFO *>
	{
	public:
		UniformFuncMap(UNIFORM_TYPE key, const UNIFORMFUNCINFO *val)
	    {
	        (*this)[key] = val;
	    }

		UniformFuncMap& operator()(UNIFORM_TYPE key,const UNIFORMFUNCINFO *val)
	    {
	        (*this)[key] = val;
	        return *this;
	    }
	};

public:

	virtual ~Uniform();

	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false);

	// archive the object
	virtual void DoArchive(Archive &ar, bool reading = true);

	/// Sync the ID with the uniform by grabbing the ID and location from compiled shader
	void SyncID(GLuint programID);
	/// load the uniform into the memory for shader access
	void Load();

	void SetValues(unsigned int count, float *values);
	void SetValues(unsigned int count, int *values);

	std::string &GetName() { return m_Name; }

private:
	GLuint m_UniformID;						// the ID returned from OpenGL
	bool m_IsLoaded;						// is it loaded into the shader memory?

	union {
		GLint	m_IntValues[4];				// uniform with integer values
		GLfloat	m_FloatValues[16];			// array of floats max 4x4 matrix
	} m_Values;

	std::string m_Name;
	UNIFORM_TYPE m_Type;					// the type of uniform
	bool m_MatTranspose;					// if the matrix should be transposed

	static const UNIFORMFUNCINFO m_UniformFuncInfos[];
	static const UniformFuncMap m_UniformFuncMap;
	const UNIFORMFUNCINFO *m_UniformFuncInfo;

	static const EnumMap<UNIFORM_TYPE> uniformTypes;

	bool m_LogNotFound;

};

}

#endif	// #ifndef _UNIFORM_H_

