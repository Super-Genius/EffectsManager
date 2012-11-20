
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include "IEffectManager.h"
#include <map>
#include <string>

namespace GLSLEFX
{

class Effect;
class IFileManager;

/// class: EffectManager
/// description: Manager for Effects loaded from source, Implementation Singleton
///
class EffectManager : public IEffectManager
{
private:
	EffectManager();

	typedef std::map<std::string, Effect *> EFXTOOBJMAP;
	typedef std::map<unsigned int, unsigned int> AUXIDTOTEXIDMAP;
public:
	virtual ~EffectManager() {};

	virtual bool LoadEffectFile(const char *source);
	virtual bool SelectTechnique(const char *effectName, const char *TechniqueName);
	virtual bool SetAuxiliaryTextureID(unsigned int auxID, unsigned int textureID);
	virtual unsigned int GetAuxiliaryTextureID(unsigned int auxID);
    // update an auxiliary texture for the renderer
    virtual void UpdateAuxiliaryTexture(unsigned int auxID, unsigned int format, unsigned int width, unsigned int height, unsigned char *data, unsigned long size);

	virtual void AddEffect(Effect *eff);

	virtual Effect *GetCurrentEffect() { return m_CurrentEffect; }

	// set lock/unlock function
	virtual void SetLockFunction(LOCKUNLOCKFUNC);

	virtual void LockContext(bool lock = true);

	// reinitialize the effects files that are loaded
	virtual void ReInitEffects();


private:
	friend class IEffectManager;
	static EffectManager *_instance;
	static IEffectManager *GetInstance();
	unsigned int m_CurEffectID;
	EFXTOOBJMAP m_EfxToObjMap;
	IFileManager *m_FileManager;
	Effect *m_CurrentEffect;
	AUXIDTOTEXIDMAP m_AuxTexIDs;
	LOCKUNLOCKFUNC m_LockFunction;

};


}

#endif	// _EFFECTMANAGER_H_
