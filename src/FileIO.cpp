
#include "IClassManager.h"

#include "FileIO.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <errno.h>
#include <string.h>

namespace GLSLEFX
{

REGISTER_CLASS_SINGLETON(FileIO);

FileIO::FileIO()
{
}

const char *FileIO::GetLastError()
{
	// return file loader last error message
	return strerror(errno);
}

bool FileIO::Load(const char *sourceName, std::string& out)
{
	std::ifstream file(sourceName);
	if (!file.is_open())
	{
		return false;
	}

	std::getline(file, out, '\0');

	if (file.bad())
	{
		return false;
	}

	file.close();

	return true;
}

bool FileIO::Load(const char *sourceName, void **out, unsigned long *outLen)
{
	std::ifstream file(sourceName);
	if (!file.is_open())
	{
		return false;
	}

	// get length of file:
	file.seekg (0, std::ios::end);
	unsigned long length = file.tellg();
	file.seekg (0, std::ios::beg);

	if (outLen != NULL)
	{
		*outLen = length;
	}

	char *content = new char[length+1];

	file.read(content, length);
	content[length] = '\0';

	if (file.bad())
	{
		delete [] content;
		return false;
	}

	if (out != NULL)
	{
		*out = content;
	}
	file.close();

	return true;
}

bool FileIO::Save(const char *destName, std::string& out)
{
	std::ofstream file(destName);
	if (!file.is_open())
	{
		return false;
	}

	file << out;

	if (file.bad())
	{
		return false;
	}

	file.close();

	return true;
}

bool FileIO::Save(const char * destName, void *data, unsigned long length)
{
	std::ofstream file(destName);
	if (!file.is_open())
	{
		return false;
	}

	file.write((const char *)data, length);

	if (file.bad())
	{
		return false;
	}

	file.close();

	return true;

}

}
