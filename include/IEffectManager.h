//
// IEffectManager:  Interface for Effect Manager
//

#ifndef _IEFFECTMANAGER_H_
#define _IEFFECTMANAGER_H_ 1

#include "IClassManager.h"

namespace GLSLEFX
{

typedef void (*LOCKUNLOCKFUNC)(bool);

class Effect;

class IEffectManager
{
private:
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
	IEffectManager(IEffectManager const&);  // Don't Implement
    void operator=(IEffectManager const&); 	// Don't implement

protected:
	IEffectManager() {}

public:
	// access to the singleton
	static IEffectManager *GetInstance();

	// Load and Effect file from source (memory, filename, URL, zip file, etc
	// @param source = "file://filename", "http://url", "zip://zipFileName"
	virtual bool LoadEffectFile(const char *source) = 0;
	// select the technique to use in effect file
	// @param effectName = name of Effect that is insided the Effect File
	// @param TechniqueName = technique name inside effect file
	// @return bool = true if technique found, else false
	virtual bool SelectTechnique(const char *Effect, const char *TechniqueName) = 0;

	// set the global auxiliary texture ID
	virtual bool SetAuxiliaryTextureID(unsigned int auxID, unsigned int textureID) = 0;
	// get the global auxiliary texture ID
	virtual unsigned int GetAuxiliaryTextureID(unsigned int auxID) = 0;

    // update an auxiliary texture for the renderer
    virtual void UpdateAuxiliaryTexture(unsigned int auxID, unsigned int format, unsigned int width, unsigned int height, unsigned char *data, unsigned long size) = 0;


	// Add and effect (usually from load effect) Creation of Effect object automatically
	// calls this, so you don't have to do it manually
	// @param eff = Effect class that was created from load.

	virtual void AddEffect(Effect *eff) = 0;

	// return the currently selected Effect
	virtual Effect *GetCurrentEffect() = 0;

	// set lock/unlock function
	virtual void SetLockFunction(LOCKUNLOCKFUNC) = 0;

	// lock the rendering context so we can do things.
	virtual void LockContext(bool lock = true) = 0;

	// reinitialize the effects files that are loaded
	virtual void ReInitEffects() = 0;

};

}

#endif	//_IEFFECTMANAGER_H_
