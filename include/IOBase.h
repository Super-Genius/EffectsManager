
#ifndef _IOBASE_H_
#define _IOBASE_H_

#include "EFObject.h"
#include <string>

namespace GLSLEFX
{

class IOBase : public EFObject
{
public:
	virtual const char *GetLastError() { return "Load/Save type not supported"; }
	virtual bool Load(const char *sourceName, unsigned int &id) { return false; }
	virtual bool Load(const char *sourceName, std::string& out) { return false; }
	virtual bool Load(const char *sourceName, void **out, unsigned long *outLen = NULL) { return false; }
	virtual bool Save(const char *sourceName, unsigned int &id)  { return false; }
	virtual bool Save(const char *destName, std::string& in)  { return false; }
	virtual bool Save(const char *destName, void *data, unsigned int length)  { return false; }
};

}

#endif	// #ifndef _IOBASE_H_
