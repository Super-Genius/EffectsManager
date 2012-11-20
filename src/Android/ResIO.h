#ifndef _RESIO_H_
#define _RESIO_H_

#include "IClassManager.h"
#include "IOBase.h"
#include <string>

// forward reference
#ifdef __cplusplus
extern "C" {
#endif
struct AAssetManager;
typedef struct AAssetManager AAssetManager;
#ifdef __cplusplus
}
#endif

namespace GLSLEFX
{

class ResIO : public IOBase
{
private:
	DECLARE_CLASS_SINGLETON(ResIO);

public:
	virtual const char *GetLastError();
	virtual bool Load(const char *sourceName, std::string& out);
	virtual bool Load(const char *sourceName, void **out, unsigned long *outLen = NULL);
	void SetAppResourceInfo(const char *appDirectory, AAssetManager *assetManager);

private:
	AAssetManager *m_AssetManager;
	std::string	m_AppDirectory;
	std::string m_Error;
};

}

#endif
