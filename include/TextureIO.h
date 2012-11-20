#ifndef _TEXTUREIO_H_
#define _TEXTUREIO_H_

#include "IClassManager.h"
#include "IOBase.h"
#include <string>

namespace GLSLEFX
{

class TextureIO : public IOBase
{
private:
	DECLARE_CLASS_SINGLETON(TextureIO);

public:
	virtual const char *GetLastError();
	virtual bool Load(const char *sourceName, unsigned int& id);
	//virtual bool Load(std::string &sourceName, void **out, unsigned long *outLen = NULL);
	//virtual bool Save(std::string &sourceName, unsigned int &id);
	//virtual bool Save(std::string &destName, void *data, unsigned long length);
private:
	std::string m_LastError;
};

}

#endif
