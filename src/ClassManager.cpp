

// Implementation file for ClassManager

#include "IClassManager.h"

#include "ClassManager.h"
#include "CIString.h"
#include "Platform.h"

namespace GLSLEFX
{

ClassManager *ClassManager::_instance = NULL;

IClassManager *IClassManager::GetInstance()
{
	return ClassManager::GetInstance();
}

IClassManager *ClassManager::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new ClassManager();
	}

	return _instance;
}

// Creates an object of from name
// @param objName = name of object to create
// @returns EFObject * = pointer to object
EFObject *ClassManager::CreateObject(const char *className)
{
	NAMETOOBJCREATEMAP::iterator ntocmIter;
	ntocmIter = m_NameToObjCreateMap.find(className);
	if (ntocmIter != m_NameToObjCreateMap.end())
	{
		CREATEOBJECTFUNC createFunc = ntocmIter->second.m_Func;
		return (*createFunc)();
	}

	return NULL;
}

bool ClassManager::RegisterClass(const char *className, CREATEOBJECTFUNC coFunc, unsigned int priority)
{
	CLASSCREATEINFO ccInfo;
	NAMETOOBJCREATEMAP::iterator ntoIter = m_NameToObjCreateMap.find(className);
	if ((ntoIter == m_NameToObjCreateMap.end()) || (ntoIter->second.m_Priority <= priority))
	{
		ccInfo.m_Priority = priority;
		ccInfo.m_Func = coFunc;
		// replace if there or create new map entry
		m_NameToObjCreateMap[className] = ccInfo;
		LOG_INFO("Registering class %s with priority 0x%.8x", className, priority);

	}
	return true;
}

};	// namespace GLSLEFX
