#ifndef _FILEIO_H_
#define _FILEIO_H_

#include "IClassManager.h"
#include "IOBase.h"
#include <string>

namespace GLSLEFX
{

class FileIO : public IOBase
{
private:
	DECLARE_CLASS_SINGLETON(FileIO);

public:
	virtual const char *GetLastError();
	virtual bool Load(const char *sourceName, std::string& out);
	virtual bool Load(const char *sourceName, void **out, unsigned long *outLen = NULL);
	virtual bool Save(const char *destName, std::string& in);
	virtual bool Save(const char *destName, void *data, unsigned long length);
};

}

#endif
