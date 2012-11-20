//
// IClassManager:  Interface for Effect Manager
//

#ifndef _ICLASSMANAGER_H_
#define _ICLASSMANAGER_H_

#include "EFObject.h"
#include <string.h>

#define DECLARE_CLASS(className)																\
private:																						\
	className();																				\
public:																							\
	static EFObject *CreateObject();

#define DECLARE_CLASS_SINGLETON(className)														\
private:																						\
	className();																				\
	static className _instance;																	\
public:																							\
	static className *GetInstance();

#define REGISTER_CLASS(className)																\
	bool registered_##className = 																\
		IClassManager::GetInstance()->RegisterClass(#className, &className::CreateObject);		\
		EFObject *className::CreateObject()														\
		{																						\
			EFObject *newClass = new className();												\
			newClass->SetType(#className);														\
			return newClass;																	\
		}

#define REGISTER_CLASS_AS(className, asClassName, priority)										\
	bool registered_##className = 																\
		IClassManager::GetInstance()->RegisterClass(#asClassName, &className::CreateObject, (priority));		\
		EFObject *className::CreateObject()														\
		{																						\
			EFObject *newClass = new className();												\
			newClass->SetType(#className);														\
			return newClass;																	\
		}

#define REGISTER_CLASS_SINGLETON(className)														\
	bool registered_##className = 																\
		IClassManager::GetInstance()->RegisterClass(#className, (CREATEOBJECTFUNC)&className::GetInstance);		\
		className className::_instance;															\
		className *className::GetInstance()														\
		{																						\
			return &_instance;																	\
		}

#define REGISTER_CLASS_SINGLETON_AS(className, asClassName, priority)							\
	bool registered_##className = 																\
		IClassManager::GetInstance()->RegisterClass(#asClassName, (CREATEOBJECTFUNC)&className::GetInstance, (priority));		\
		className className::_instance;															\
		className *className::GetInstance()														\
		{																						\
			return &_instance;																	\
		}

#define REGISTER_PRIORITY_NORMAL	0x80000000

namespace GLSLEFX
{

class IClassManager
{
private:
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
	IClassManager(IClassManager const&);  // Don't Implement
    void operator=(IClassManager const&); 	// Don't implement

protected:
	IClassManager() {}

public:
	// access to the singleton
	static IClassManager *GetInstance();
	~IClassManager() { }

	// Registers a class in the EffectManager
	// @param className = the name of the class to register
	// @param coFunc = pointer to create function for class
	// @param priority = priority to override class creation, 0x80000000 is normal > means more priority
	// @returns bool = true always
	virtual bool RegisterClass(const char *className, CREATEOBJECTFUNC coFunc, unsigned int priority = REGISTER_PRIORITY_NORMAL) = 0;

	// Creates an object of from name
	// @param objName = name of object to create
	// @returns EFObject * = pointer to object
	virtual EFObject *CreateObject(const char *className) = 0;

};

}
#endif	//_ICLASSMANAGER_H_
