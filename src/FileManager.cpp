
#include "FileManager.h"
#include "IOBase.h"
#include <cctype>

namespace GLSLEFX
{

FileManager *FileManager::_instance = NULL;

IFileManager *IFileManager::GetInstance()
{
	return FileManager::GetInstance();
}

IFileManager *FileManager::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new FileManager();
	}

	return _instance;
}

const char *FileManager::GetLastError()
{
	return m_LastError.c_str();
}

IOBase *FileManager::GetFileSystem(std::string &fName)
{
	std::string fileSysName;
	size_t fsEnd = fName.find("://");
	// if not specified, use standard file system
	if (fsEnd == std::string::npos)
	{
		fileSysName = "File";
	}
	else
	{
		fileSysName = fName.substr(0, fsEnd);
		fName = fName.substr(fsEnd + 3);
	}

	fileSysName[0] = toupper(fileSysName[0]);
	fileSysName += "IO";

	IOBase *iob = (IOBase *)IClassManager::GetInstance()->CreateObject(fileSysName.c_str());
	if (iob == NULL)
	{
		m_LastError = "Unknown file system ";
		m_LastError += fileSysName;
		m_LastError += " for file ";
		m_LastError += fName;
	}

	return iob;
}

bool FileManager::Load(const char *sourceName, unsigned int &id)
{
	std::string pathName(sourceName);
	m_LastError = "";
	IOBase *fs = GetFileSystem(pathName);
	bool retVal = false;
	if (fs != NULL)
	{
		retVal = fs->Load(pathName.c_str(), id);
		if (!retVal)
		{
			m_LastError = fs->GetLastError();
		}
	}

	return retVal;
}

bool FileManager::Load(const char *sourceName, std::string& out)
{
	std::string pathName(sourceName);
	m_LastError = "";
	bool retVal = false;
	IOBase *fs = GetFileSystem(pathName);
	if (fs != NULL)
	{
		retVal = fs->Load(pathName.c_str(), out);
		if (!retVal)
		{
			m_LastError = fs->GetLastError();
		}
	}

	return retVal;
}

bool FileManager::Load(const char *sourceName, void **out, unsigned long *length)
{
	std::string pathName(sourceName);
	m_LastError = "";
	bool retVal = false;
	IOBase *fs = GetFileSystem(pathName);
	if (fs != NULL)
	{
		retVal = fs->Load(pathName.c_str(), out, length);
		if (!retVal)
		{
			m_LastError = fs->GetLastError();
		}
	}

	return retVal;
}

bool FileManager::Save(const char *destName, unsigned int &id)
{
	std::string pathName(destName);
	m_LastError = "";
	bool retVal = false;
	IOBase *fs = GetFileSystem(pathName);
	if (fs != NULL)
	{
		retVal = fs->Save(pathName.c_str(), id);
		if (!retVal)
		{
			m_LastError = fs->GetLastError();
		}
	}

	return retVal;
}

bool FileManager::Save(const char *destName, std::string& in)
{
	std::string pathName(destName);
	m_LastError = "";
	bool retVal = false;
	IOBase *fs = GetFileSystem(pathName);
	if (fs != NULL)
	{
		retVal =  fs->Save(pathName.c_str(), in);
		if (!retVal)
		{
			m_LastError = fs->GetLastError();
		}
	}

	return retVal;
}

bool FileManager::Save(const char *destName, void *data, unsigned long length)
{
	std::string pathName(destName);
	m_LastError = "";
	bool retVal = false;
	IOBase *fs = GetFileSystem(pathName);
	if (fs != NULL)
	{
		retVal = fs->Save(pathName.c_str(), data, length);
		if (!retVal)
		{
			m_LastError = fs->GetLastError();
		}
	}

	return retVal;
}

}
