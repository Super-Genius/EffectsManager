//
// IEffectManager:  Interface for Effect Manager
//

#ifndef _IFILEMANAGER_H_
#define _IFILEMANAGER_H_

#include "IClassManager.h"
#include <string>

namespace GLSLEFX
{

class IOBase;

class IFileManager
{
private:
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
	IFileManager(IFileManager const&);  // Don't Implement
    void operator=(IFileManager const&); 	// Don't implement

protected:
	IFileManager() {}

public:
	// access to the singleton
	static IFileManager *GetInstance();
	~IFileManager() { }

	virtual const char *GetLastError() = 0;
	virtual bool Load(const char *sourceName, unsigned int &id) = 0;
	virtual bool Load(const char *sourceName, std::string& out) = 0;
	virtual bool Load(const char *sourceName, void **out, unsigned long *outLen = NULL) = 0;
	virtual bool Save(const char *sourceName, unsigned int &id) = 0;
	virtual bool Save(const char *destName, std::string& in) = 0;
	virtual bool Save(const char *destName, void *data, unsigned long length) = 0;

	virtual IOBase *GetFileSystem(std::string &fName) = 0;

};

}

#endif	//_IFILEMANAGER_H_
