

#include "IClassManager.h"
#include "Archive.h"

namespace GLSLEFX
{

Archive::Archive()
{
	m_ClassManager = IClassManager::GetInstance();
}

const char *Archive::FindTagValue(const char *tag)
{
	return NULL;
}

const char *Archive::GetNextSibling()
{
	return NULL;
}

const char *Archive::GetFirstChild(bool resetToRoot)
{
	return NULL;
}

bool Archive::LoadHierarchy(EFObject *parent)
{
	EFObject *curObject;

	const char *nodeName = GetFirstChild(parent == NULL);
	while (nodeName != NULL)
	{
		curObject = m_ClassManager->CreateObject(nodeName);
		if (curObject != NULL)
		{
			curObject->SetParent(parent);
			// stream in the object
			curObject->DoArchive(*this);
			// load all children before Init call
			if (!LoadHierarchy(curObject))
			{
				return false;
			}
			// now Initialize the object
			curObject->Init();
		}
		nodeName = GetNextSibling();
	}

	return true;


}

}
