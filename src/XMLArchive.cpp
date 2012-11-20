
#include "XMLArchive.h"
#include <stdio.h>
namespace GLSLEFX
{

XMLArchive::XMLArchive(const char *xmlContent, unsigned long contentLen)
{
	curXmlNode = NULL;
	xmlDoc.parse<0>((char *)xmlContent);
}

XMLArchive::XMLArchive(std::string& xmlContent)
{
	curXmlNode = NULL;
	xmlDoc.parse<0>((char *)xmlContent.c_str());
}

XMLArchive::XMLArchive(std::istream& inputStream)
{
	curXmlNode = NULL;

	// get length of file:
	inputStream.seekg (0, std::ios::end);
	unsigned long length = inputStream.tellg();
	inputStream.seekg (0, std::ios::beg);

	char *xmlContent = new char[length+1];

	inputStream.read(xmlContent, length);
	xmlContent[length] = '\0';

	xmlDoc.parse<0>(xmlContent);
}

const char *XMLArchive::GetNextSibling()
{
	static std::string name;
	curXmlNode = curXmlNode->next_sibling();
	if (curXmlNode == NULL)
	{
		if (!m_NodeTree.empty())
		{
			curXmlNode = m_NodeTree.top();
			m_NodeTree.pop();
			m_Depth--;
		}

		return NULL;

	}

	name.clear();
	name.append(curXmlNode->name(), curXmlNode->name_size());

#ifdef DEBUG_ITERATE_HIERARCHY
	for(int i=0; i<m_Depth; i++)
	{
		printf("-");
	}
	printf(" %s\n", name.c_str());
#endif

	return name.c_str();

}

// returns the l
const char *XMLArchive::GetFirstChild(bool resetToRoot)
{
	static std::string name;
	if (xmlDoc.first_node() == NULL)
	{
		return NULL;
	}

	xml_node<> *child;

	if (resetToRoot || (curXmlNode == NULL))
	{
		child = xmlDoc.first_node();
		m_Depth = 0;
		while (!m_NodeTree.empty())
		{
			m_NodeTree.pop();
		}
	}
	else
	{
		// see if any children
		child = curXmlNode->first_node();
	}

	if (child == NULL)
	{
		return NULL;
	}

	m_NodeTree.push(curXmlNode);
	m_Depth++;

	curXmlNode = child;

	name.clear();
	name.append(child->name(), child->name_size());

	for(int i=0; i<m_Depth; i++)
	{
		printf("-");
	}
	printf(" %s\n", name.c_str());
	return name.c_str();

}

const char *XMLArchive::FindTagValue(const char *tag)
{
	xml_attribute<> *attr = curXmlNode->first_attribute(tag);
	if (attr != NULL)
	{
		return attr->value();
	}

	return NULL;
}

}

