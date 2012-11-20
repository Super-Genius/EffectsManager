#ifndef _SHADER_MANAGER_SOURCE_CODE_
#define _SHADER_MANAGER_SOURCE_CODE_

#include "IClassManager.h"
#include <vector>
#include <string>

namespace GLSLEFX
{

class SourceCode
{
public:
	SourceCode(const std::string& source);
	~SourceCode();

	char * GetCode();

	std::string &LoadSource(const std::string &source, int &linen);
	std::string &LoadSource(std::string &filename_path, const std::string &sourceName, std::istream& sourceStream, int &linen);

	void displayOpenglLog(const std::string &openGLlog);

private:
	std::string m_SourceCode;
	std::vector<int> lines;
	std::vector<int> locallines;
	std::vector<std::string> m_FileNames;
};

}

#endif
