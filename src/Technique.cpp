

#include "IEffectManager.h"
#include "Effect.h"
#include "Technique.h"
#include "Pass.h"
#include "Archive.h"

namespace GLSLEFX
{

REGISTER_CLASS(Technique);

Technique::Technique()
{
}

Technique::~Technique()
{

}

void Technique::Init(bool reInit)
{
	if (!reInit)
	{
		((Effect *)m_Parent)->AddTechnique(m_Name, this);
	}
	else
	{
		MAPPASSES::iterator mpIter;
		for (mpIter = m_Passes.begin(); mpIter != m_Passes.end(); ++mpIter)
		{
			Pass *pass = mpIter->second;
			pass->Init(true);
		}
	}
}

void Technique::DoArchive(Archive &ar, bool reading)
{
	if (reading)
	{
		ar.Read("Name", m_Name);
	}
	else
	{
		ar.Write("Name", m_Name);
	}
}

void Technique::AddPass(Pass *newPass, unsigned int Number)
{
	m_Passes[Number] = newPass;
}

Pass* Technique::GetPass(unsigned int passNum)
{
	MAPPASSES::iterator mpIter = m_Passes.find(passNum);
	if (mpIter != m_Passes.end())
	{
		return mpIter->second;
	}

	return NULL;
}

const Technique::MAPPASSES *Technique::GetPasses()
{
	return &m_Passes;
}

int Technique::SetLayout(LAYOUTDESC* layout, size_t stride, size_t n)
{
	return 0;
}

bool Technique::IsValid()
{
	return true;
}

// sets the pass X auxiliary texture (auxID) to use texID (opengl texture ID)
bool Technique::SetAuxiliaryTextureID(unsigned int passNum, unsigned int auxID, unsigned int texID)
{
	Pass *pass = GetPass(passNum);
	if (pass == NULL)
	{
		return false;
	}

	return pass->SetAuxiliaryTextureID(auxID, texID);
}

}
