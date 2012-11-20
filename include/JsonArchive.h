
#ifndef _JSON_ARCHIVE_H
#define _JSON_ARCHIVE_H

#include "Archive.h"
#include <iostream>
#include <stack>
#include <string>
#include <istream>
#include "json/json.h"

using namespace Json;

namespace GLSLEFX
{

class JsonArchive : public Archive
{
public:
	JsonArchive();
	JsonArchive(std::string& source);
	JsonArchive(std::istream& source);
	JsonArchive(const char *source, unsigned long len);

	virtual const char *FindTagValue(const char *tag);
	virtual const char *GetNextSibling();
	virtual const char *GetFirstChild(bool resetToRoot = false);


	void DumpJsonTree(Json::Value *root = NULL, int depth = 0);

private:
	Json::Value * GetFirstChildObject();
	Json::Value * GetNextSiblingObject();

	typedef struct JsonNodeInfo
	{
		Json::ValueIterator curNodeIter;
		Json::Value *node;
		const char *name;
		bool bIsParsingArray;
	} JsonNodeInfo;

	JsonNodeInfo m_CurNodeInfo;
	const char *m_CurrentMemberName;

	std::stack<JsonNodeInfo> m_NodeTree;
	Json::Value m_Root;
	void PrintJsonValue(Json::Value *val);

	int m_Depth;

};

}

#endif	// #ifndef _JSON_ARCHIVE_H
