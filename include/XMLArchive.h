

#ifndef _XML_ARCHIVE_H
#define _XML_ARCHIVE_H

#include "Archive.h"
#include <iostream>
#include <stack>
#include <string>
#include <istream>
#include "rapidxml.hpp"

using namespace rapidxml;

namespace GLSLEFX
{

class XMLArchive : public Archive
{
public:
	XMLArchive() { };
	XMLArchive(std::string& source);
	XMLArchive(std::istream& source);
	XMLArchive(const char *source, unsigned long len);

	virtual const char *FindTagValue(const char *tag);
	virtual const char *GetNextSibling();
	virtual const char *GetFirstChild(bool resetToRoot = false);

private:
	xml_document<> xmlDoc;
	xml_node<> *curXmlNode;
	std::stack<xml_node<> *>m_NodeTree;
	int m_Depth;
};

}

#endif	// #ifndef _XML_ARCHIVE_H
