
#include "IEffectManager.h"
#include "Pass.h"
#include "Shader.h"
#include "Archive.h"
#include "Uniform.h"
#include <iostream>

namespace GLSLEFX
{

REGISTER_CLASS(Uniform);

const Uniform::UNIFORMFUNCINFO Uniform::m_UniformFuncInfos[] = { { false, false, 1, (void *)&glUniform1iv }, { false, false, 2, (void *)&glUniform2iv }, { false, false, 3, (void *)&glUniform3iv }, { false, false, 4, (void *)&glUniform4iv },
														{ true, false, 1, (void *)&glUniform1fv }, { true, false, 2, (void *)&glUniform2fv }, { true, false, 3, (void *)&glUniform3fv }, { true, false, 4, (void *)&glUniform4fv },
														{ true, true, 4, (void *)&glUniformMatrix2fv }, { true, true, 6, (void *)&glUniformMatrix2x3fv }, { true, true, 8, (void *)&glUniformMatrix2x4fv },
														{ true, true, 6, (void *)&glUniformMatrix3x2fv }, { true, true, 9, (void *)&glUniformMatrix3fv }, { true, true, 12, (void *)&glUniformMatrix3x4fv },
														{ true, true, 8, (void *)&glUniformMatrix4x2fv }, { true, true, 12, (void *)&glUniformMatrix4x3fv }, { true, true, 16, (void *)&glUniformMatrix4fv  } };

const Uniform::UniformFuncMap Uniform::m_UniformFuncMap = Uniform::UniformFuncMap
	( Uniform::INT1, &m_UniformFuncInfos[0])
	( Uniform::INT2, &m_UniformFuncInfos[1])
	( Uniform::INT3, &m_UniformFuncInfos[2])
	( Uniform::INT4, &m_UniformFuncInfos[3])
	( Uniform::FLOAT1, &m_UniformFuncInfos[4])
	( Uniform::FLOAT2, &m_UniformFuncInfos[5])
	( Uniform::FLOAT3, &m_UniformFuncInfos[6])
	( Uniform::FLOAT4, &m_UniformFuncInfos[7])
	( Uniform::MATRIX2X2, &m_UniformFuncInfos[8])
	( Uniform::MATRIX2X3, &m_UniformFuncInfos[9])
	( Uniform::MATRIX2X4, &m_UniformFuncInfos[10])
	( Uniform::MATRIX3X2, &m_UniformFuncInfos[11])
	( Uniform::MATRIX3X3, &m_UniformFuncInfos[12])
	( Uniform::MATRIX3X4, &m_UniformFuncInfos[13])
	( Uniform::MATRIX4X2, &m_UniformFuncInfos[14])
	( Uniform::MATRIX4X3, &m_UniformFuncInfos[15])
	( Uniform::MATRIX4X4, &m_UniformFuncInfos[16]) ;

const EnumMap<Uniform::UNIFORM_TYPE> Uniform::uniformTypes = EnumMap<Uniform::UNIFORM_TYPE>
	( "int1", Uniform::INT1 )
	( "int2", Uniform::INT2 )
	( "int3", Uniform::INT3 )
	( "int4", Uniform::INT4 )
	( "float1", Uniform::FLOAT1 )
	( "float2", Uniform::FLOAT2 )
	( "float3", Uniform::FLOAT3 )
	( "float4", Uniform::FLOAT4 )
	( "matrix2x2", Uniform::MATRIX2X2 )
	( "matrix2x3", Uniform::MATRIX2X3 )
	( "matrix2x4", Uniform::MATRIX2X4 )
	( "matrix3x2", Uniform::MATRIX3X2 )
	( "matrix3x3", Uniform::MATRIX3X3 )
	( "matrix3x4", Uniform::MATRIX3X4 )
	( "matrix4x2", Uniform::MATRIX4X2 )
	( "matrix4x3", Uniform::MATRIX4X3 )
	( "matrix4x4", Uniform::MATRIX4X4 ) ;

Uniform::Uniform()
{
	m_IsLoaded = false;
	m_UniformFuncInfo = NULL;
}

void Uniform::Init(bool reInit)
{
	if (!reInit)
	{
		((Shader *)m_Parent)->AddUniform(this);
	}
}

void Uniform::DoArchive(Archive &ar, bool reading)
{
	if (reading)
	{
		ar.Read("Name", m_Name);
		ar.Read("Type", EnumFilter<UNIFORM_TYPE>(uniformTypes, m_Type));
		const UniformFuncMap::const_iterator ufmIter = m_UniformFuncMap.find(m_Type);
		if (ufmIter != m_UniformFuncMap.end())
		{
			m_UniformFuncInfo = ufmIter->second;
			if (m_UniformFuncInfo->m_AreFloatValues)
			{
				ar.Read("Values", m_Values.m_FloatValues, m_UniformFuncInfo->m_NumValues);
			}
			else
			{
				ar.Read("Values", m_Values.m_IntValues, m_UniformFuncInfo->m_NumValues);
			}
		}
		ar.Read( "Transpose", m_MatTranspose );
	}
	else
	{
//		ar.Write("Name", m_Name);
	}

	m_LogNotFound = false;
}


Uniform::~Uniform()
{
}

void Uniform::SyncID(GLuint programID)
{
	IEffectManager::GetInstance()->LockContext(true);

	m_UniformID = glGetUniformLocation(programID, m_Name.c_str());

	if (((int)m_UniformID < 0) && (!m_LogNotFound))
	{
		m_LogNotFound = true;
		LOG_INFO("shader uniform %s not found!", m_Name.c_str());
	}
	IEffectManager::GetInstance()->LockContext(false);
}

void Uniform::Load()
{
	IEffectManager::GetInstance()->LockContext(true);

	if (m_UniformFuncInfo != NULL)
	{
		if (!m_UniformFuncInfo->m_IsMatrixFunc)
		{
			GLUNIFORMFUNC funcPtr = (GLUNIFORMFUNC)m_UniformFuncInfo->m_UniformFunc;
			(*funcPtr)(m_UniformID, m_UniformFuncInfo->m_NumValues, (GLvoid *)m_Values.m_IntValues);
		}
		else
		{
			GLUNIFORMMATFUNC funcPtr = (GLUNIFORMMATFUNC)m_UniformFuncInfo->m_UniformFunc;
			(*funcPtr)(m_UniformID, m_UniformFuncInfo->m_NumValues, m_MatTranspose, m_Values.m_FloatValues);

		}
	}
	m_IsLoaded = true;
	IEffectManager::GetInstance()->LockContext(false);
}

void Uniform::SetValues(unsigned int count, float *values)
{
	if (count > 16)
	{
		count = 16;
		LOG_ERROR("Count of values for SetValues of type float is %d, should be < 16", count);
	}

	for (unsigned int i=0; i<count; i++)
	{
		m_Values.m_FloatValues[i] = values[i];
	}
}

void Uniform::SetValues(unsigned int count, int *values)
{
	if (count > 4)
	{
		count = 4;
		LOG_ERROR("Count of values for SetValues of type int is %d, should be < 4", count);
	}

	for (unsigned int i=0; i<count; i++)
	{
		m_Values.m_IntValues[i] = values[i];
	}
}

}
