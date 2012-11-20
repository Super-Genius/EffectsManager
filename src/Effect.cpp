

#include "EffectManager.h"
#include "Effect.h"
#include "Technique.h"
#include "Archive.h"

namespace GLSLEFX
{

REGISTER_CLASS(Effect);

Effect::Effect()
{
	m_CurrentTechnique = NULL;
}

Effect::~Effect()
{
}

void Effect::DoArchive(Archive &ar, bool reading)
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

void Effect::Init(bool reInit)
{
	// parent should be null
	if (!reInit)
	{
		IEffectManager::GetInstance()->AddEffect(this);
	}
	else
	{
		for (NAMETOTECHNIQUEMAP::iterator it = m_TechniquesByName.begin(); it != m_TechniquesByName.end(); ++it )
		{
			Technique* tech = it->second;
			tech->Init(true);
		}
	}
}

Technique* Effect::GetTechnique(const std::string& name)
{
	NAMETOTECHNIQUEMAP::iterator it = m_TechniquesByName.find(name);
	if ( it == m_TechniquesByName.end() )
	{
		return NULL;
	}

	return it->second;
}

void Effect::AddTechnique(const std::string& name, Technique *newTechnique)
{
	m_TechniquesByName[name] = newTechnique;
}

bool Effect::IsValid()
{
	for (NAMETOTECHNIQUEMAP::iterator it = m_TechniquesByName.begin(); it != m_TechniquesByName.end(); ++it )
	{
		Technique* tech = it->second;
		if ( !tech->IsValid() )
		{
			return false;
		}
	}

	return true;
}

/// Activate/Select a technique
bool Effect::ActivateTechnique(const char *name)
{
	std::string techName(name);
	m_CurrentTechnique = GetTechnique(techName);

	return (m_CurrentTechnique != NULL);
}

int Effect::SetLayout(LAYOUTDESC* layout, size_t stride, size_t n)
{
	int ret;

	for ( NAMETOTECHNIQUEMAP::iterator it = m_TechniquesByName.begin(); it != m_TechniquesByName.end(); ++it )
	{
		Technique* tech = it->second;
		if ( ( ret = tech->SetLayout(layout, stride, n) ) != 0 )
		{
			return ret;
		}
	}

	return 0;
}

}
