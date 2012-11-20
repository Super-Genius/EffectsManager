
#ifndef _CLASSMANAGER_H_
#define _CLASSMANAGER_H_

#include "IClassManager.h"
#include <map>
#include <string>

namespace GLSLEFX
{

/// class: ClassManager
/// description: Manager for factory Implementation Singleton
///
class ClassManager : public IClassManager
{
private:
	ClassManager() { };

    typedef struct CLASSCREATEINFO
    {
    	unsigned int m_Priority;
    	CREATEOBJECTFUNC m_Func;
    } CLASSCREATEINFO;

	typedef std::map<std::string, CLASSCREATEINFO> NAMETOOBJCREATEMAP;

public:
	virtual ~ClassManager() {};

	virtual bool RegisterClass(const char *className, CREATEOBJECTFUNC coFunc, unsigned int priority = REGISTER_PRIORITY_NORMAL);
	virtual EFObject *CreateObject(const char *objName);

private:
	friend class IClassManager;
	static ClassManager *_instance;
	static IClassManager *GetInstance();
	NAMETOOBJCREATEMAP m_NameToObjCreateMap;
};


}

#endif	// _CLASSMANAGER_H_
