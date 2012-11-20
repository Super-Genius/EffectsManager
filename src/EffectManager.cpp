

// Implementation file for EffectManager

#include "IEffectManager.h"
#include "EffectManager.h"
#include "IFileManager.h"
#include "Effect.h"
#include "Technique.h"
#include "Pass.h"
#include "XMLArchive.h"
#include "JsonArchive.h"
#include <fstream>
#include "CIString.h"



namespace GLSLEFX
{

EffectManager *EffectManager::_instance = NULL;

EffectManager::EffectManager()
{
	m_CurEffectID = 1;
	m_FileManager = IFileManager::GetInstance();
	m_CurrentEffect = NULL;
	m_LockFunction = NULL;
}

IEffectManager *IEffectManager::GetInstance()
{
	return EffectManager::GetInstance();
}

IEffectManager *EffectManager::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new EffectManager();
	}

	return _instance;
}

bool EffectManager::LoadEffectFile(const char *source)
{
	char *data;
	unsigned long sourceLen;

	if (!m_FileManager->Load(source, (void **)&data, &sourceLen))
	{
		LOG_ERROR("Unable to open %s, reason %s\n", source, m_FileManager->GetLastError());
		return false;
	}

	CIString sourceStr(source);
	//std::fstream fileStream(source, std::fstream::in);
	Archive *curArchive;

	if (sourceStr.find(".json") != std::string::npos)
	{
		curArchive = new JsonArchive((const char *)data, sourceLen);
		//((JsonArchive *)curArchive)->DumpJsonTree();
	}
	else
	{
		curArchive = new XMLArchive((const char *)data, sourceLen);
	}

	bool returnVal = curArchive->LoadHierarchy(NULL);

	//fileStream.close();

	delete curArchive;

	// Initialized all the classes
	return returnVal;
}

bool EffectManager::SelectTechnique(const char *EffectName, const char *TechniqueName)
{
	if ((EffectName == NULL) || (TechniqueName == NULL))
	{
		m_CurrentEffect = NULL;
		return true;
	}

	EFXTOOBJMAP::iterator efxIter;
	efxIter = m_EfxToObjMap.find(std::string(EffectName));
	if (efxIter != m_EfxToObjMap.end())
	{
		m_CurrentEffect = efxIter->second;
		return m_CurrentEffect->ActivateTechnique(TechniqueName);
	}

	return false;
}

void EffectManager::ReInitEffects()
{
	EFXTOOBJMAP::iterator efxIter;
	for (efxIter = m_EfxToObjMap.begin(); efxIter != m_EfxToObjMap.end(); ++efxIter)
	{
		Effect *effect = efxIter->second;
		effect->Init(true);
	}
}

// update an auxiliary texture for the renderer
void EffectManager::UpdateAuxiliaryTexture(unsigned int auxID, unsigned int format, unsigned int width, unsigned int height, unsigned char *data, unsigned long size)
{
	Effect *curEffect = GetCurrentEffect();
	if (curEffect != NULL)
	{
		Technique *curTechnique = curEffect->GetCurrentTechnique();
		if (curTechnique != NULL)
		{
			const Technique::MAPPASSES *mPasses = curTechnique->GetPasses();
			Technique::MAPPASSES::const_iterator passIter;

			for (passIter = mPasses->begin(); passIter != mPasses->end(); ++passIter)
			{
				Pass *curPass = passIter->second;
				if (curPass != NULL)
				{
					curPass->UpdateAuxiliaryTextures(auxID, format, width, height, data, size);
				}
			}
		}
	}

}

bool EffectManager::SetAuxiliaryTextureID(unsigned int auxID, unsigned int textureID)
{
	m_AuxTexIDs[auxID] = textureID;
	return true;
}

unsigned int EffectManager::GetAuxiliaryTextureID(unsigned int auxID)
{
	AUXIDTOTEXIDMAP::iterator auxIter = m_AuxTexIDs.find(auxID);
	if (auxIter != m_AuxTexIDs.end())
	{
		return m_AuxTexIDs[auxID];
	}

	return 0;
}

void EffectManager::AddEffect(Effect *efx)
{
	m_EfxToObjMap[efx->GetName()] = efx;
	//m_CurEffectID++;
}

void EffectManager::SetLockFunction(LOCKUNLOCKFUNC func)
{
	m_LockFunction = func;
}

void EffectManager::LockContext(bool lock)
{
	if (m_LockFunction != NULL)
	{
		(*m_LockFunction)(lock);
	}
}

};	// namespace GLSLEFX
