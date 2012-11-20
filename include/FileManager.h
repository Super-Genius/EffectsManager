//
// FileManager:  Implementation for File Manager
//

#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include "IFileManager.h"
#include <string>

namespace GLSLEFX
{

class FileManager : public IFileManager
{
private:
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singelton appearing.
	FileManager(FileManager const&);  // Don't Implement
    void operator=(FileManager const&); 	// Don't implement
    FileManager() { }
public:
    ~FileManager() { }
	virtual const char *GetLastError();
	virtual bool Load(const char *sourceName, unsigned int &id);
	virtual bool Load(const char *sourceName, std::string& out);
	virtual bool Load(const char *sourceName, void **out, unsigned long *length = NULL);
	virtual bool Save(const char *sourceName, unsigned int &id);
	virtual bool Save(const char *destName, std::string& in);
	virtual bool Save(const char *destName, void *data, unsigned long length);

	virtual IOBase *GetFileSystem(std::string &fName);

private:
	friend class IFileManager;
    static FileManager *_instance;
	static IFileManager *GetInstance();
	std::string m_LastError;

};

}

#endif	//_FILEMANAGER_H_
