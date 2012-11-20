
#include "JsonArchive.h"
#include "json/json.h"
#include <stdio.h>

namespace GLSLEFX
{

JsonArchive::JsonArchive()
{
	m_CurNodeInfo.node = NULL;

}

JsonArchive::JsonArchive(const char *inputString, unsigned long size)
{
	Json::Reader reader;
	reader.parse(inputString, inputString + size, m_Root);
	m_CurNodeInfo.node = NULL;
}

JsonArchive::JsonArchive(std::istream& inputStream)
{
	Json::Reader reader;
	reader.parse(inputStream, m_Root);
	m_CurNodeInfo.node = NULL;
}

JsonArchive::JsonArchive(std::string& inputString)
{
	Json::Reader reader;
	reader.parse(inputString, m_Root);
	m_CurNodeInfo.node = NULL;
}

Json::Value * JsonArchive::GetNextSiblingObject()
{
	Json::Value* curNode = m_CurNodeInfo.node;

	if (curNode == NULL)
	{
		return NULL;
	}

	Json::Value* nextChildNode = NULL;

	for  (;(nextChildNode == NULL) && (m_CurNodeInfo.curNodeIter != m_CurNodeInfo.node->end()); m_CurNodeInfo.curNodeIter++)
	{
		Json::Value *curIterNode = &(*(m_CurNodeInfo.curNodeIter));
		Json::ValueType nodeType = curIterNode->type();
		if (nodeType == Json::objectValue)
		{
			if (!m_CurNodeInfo.bIsParsingArray)
			{
				m_CurrentMemberName = m_CurNodeInfo.curNodeIter.memberName();
			}

			nextChildNode = curIterNode;
			// and let iterator bump to next object
		}
		else if (nodeType == Json::arrayValue)
		{
			const char *curName = m_CurNodeInfo.curNodeIter.memberName();
			// bump iterator past this object since we break out.
			m_CurNodeInfo.curNodeIter++;
			// push parent state, to traverse children of array
			m_NodeTree.push(m_CurNodeInfo);
			m_CurNodeInfo.name = curName;
			// dump into array could be objects in there
			m_CurNodeInfo.bIsParsingArray = true;
			m_CurNodeInfo.node = curIterNode;
			nextChildNode = GetFirstChildObject();
			if (nextChildNode == NULL)
			{
				m_CurNodeInfo = m_NodeTree.top();
				m_NodeTree.pop();
				// put back iterator to us, as for loop will update
				m_CurNodeInfo.curNodeIter--;
			}
			else
			{
				m_CurrentMemberName = m_CurNodeInfo.name;
				// iterator is already at next object, so break so it doesn't ++ again.
				break;
			}
		}
#ifdef DEBUG_ITERATE_HIERARCHY
		else
		{
			for (int i=0; i<m_Depth+1; i++)
			{
				printf(" ");
			}

			if (m_CurNodeInfo.bIsParsingArray)
			{
				printf("member = %s ", m_CurNodeInfo.name);

			}
			else
			{
				printf("member = %s ", m_CurNodeInfo.curNodeIter.memberName());
			}
			Json::Value& curVal = *(m_CurNodeInfo.curNodeIter);
			if (curVal.isConvertibleTo(Json::stringValue))
			{
				printf("value = %s\n", curVal.asString().c_str());
			}
			else
			{
				printf("value = unknown\n");
			}
		}
#endif
	}

	// auto-pop arrays
	if ((nextChildNode == NULL) && m_CurNodeInfo.bIsParsingArray)
	{
		m_CurNodeInfo = m_NodeTree.top();
		m_NodeTree.pop();
		m_CurrentMemberName = m_CurNodeInfo.name;
		nextChildNode = GetNextSiblingObject();
	}

	return nextChildNode;
}

Json::Value * JsonArchive::GetFirstChildObject()
{
	if (m_CurNodeInfo.node->size() > 0)
	{
		m_CurNodeInfo.curNodeIter = m_CurNodeInfo.node->begin();
		return GetNextSiblingObject();
	}

	return NULL;
}

void JsonArchive::PrintJsonValue(Json::Value *val)
{
    if( val->isString() ) {
        printf( "string(%s)", val->asCString() );
    } else if( val->isBool() ) {
        printf( "bool(%d)", val->asBool() );
    } else if( val->isInt() ) {
        printf( "int(%d)", val->asInt() );
    } else if( val->isUInt() ) {
        printf( "uint(%u)", val->asUInt() );
    } else if( val->isDouble() ) {
        printf( "double(%f)", val->asDouble() );
    }
    else
    {
        printf( "unknown type=[%d]", val->type() );
    }
}

void JsonArchive::DumpJsonTree(Json::Value* root, int depth)
{
	if (root == NULL)
	{
		root = &m_Root;
	}

	depth += 1;
	printf( " {type=[%d], size=%d}", root->type(), root->size() );

	if( root->size() > 0 )
	{
		printf("\n");
		for( Json::ValueIterator itr = root->begin() ; itr != root->end() ; itr++ )
		{
			// Print depth.
			for( int tab = 0 ; tab < depth; tab++)
			{
			   printf("-");
			}
			printf(" subvalue(%s) -", itr.memberName());
			DumpJsonTree( &(*itr), depth);
		}
	}
	else
	{
		printf(" ");
		PrintJsonValue(root);
		printf( "\n" );
	}
}

const char *JsonArchive::GetNextSibling()
{
	Json::Value *newJsonNode = GetNextSiblingObject();
	// no more, pop back to parent
	if (newJsonNode == NULL)
	{
		if (!m_NodeTree.empty())
		{
			m_CurNodeInfo = m_NodeTree.top();
			m_NodeTree.pop();
			if (m_CurNodeInfo.bIsParsingArray)
			{
				m_CurrentMemberName = m_CurNodeInfo.name;
			}
			m_Depth--;
		}

		return NULL;

	}

	// start traversing children
	m_NodeTree.push(m_CurNodeInfo);

	m_Depth++;

	// set the node that we are parsing
	m_CurNodeInfo.node = newJsonNode;
	m_CurNodeInfo.name = m_CurrentMemberName;

#ifdef DEBUG_ITERATE_HIERARCHY
	for (int i=0; i< m_Depth; i++)
	{
		printf("-");
	}

	printf("%s\n",  m_CurNodeInfo.name);
#endif

	return m_CurNodeInfo.name;
}

const char *JsonArchive::GetFirstChild(bool resetToRoot)
{
	static std::string name;

	if (resetToRoot || (m_CurNodeInfo.node == NULL))
	{
		m_CurNodeInfo.node = &m_Root;
		m_CurNodeInfo.name = "";
		m_Depth = 0;
		while (!m_NodeTree.empty())
		{
			m_NodeTree.pop();
		}
	}

	m_CurNodeInfo.bIsParsingArray = false;
	Json::Value *newJsonNode = GetFirstChildObject();
	// See if any more children, if not, move to next sibling
	if (newJsonNode == NULL)
	{
		// pop to parent for next sibling
		if (!m_NodeTree.empty())
		{
			m_CurNodeInfo = m_NodeTree.top();
			m_NodeTree.pop();
			if (m_CurNodeInfo.bIsParsingArray)
			{
				m_CurrentMemberName = m_CurNodeInfo.name;
			}
			m_Depth--;
		}

		return NULL;
	}

	// start traversing children
	m_NodeTree.push(m_CurNodeInfo);
	// set the node that we are parsing

	m_Depth++;
	m_CurNodeInfo.node = newJsonNode;
	m_CurNodeInfo.name = m_CurrentMemberName;

#ifdef DEBUG_ITERATE_HIERARCHY
	for (int i=0; i< m_Depth; i++)
	{
		printf("-");
	}

	printf("%s\n", m_CurrentMemberName);
#endif

	return m_CurNodeInfo.name;

}

const char *JsonArchive::FindTagValue(const char *tag)
{
	static std::string tempStr;

	if (m_CurNodeInfo.node->isMember(tag))
	{
		const Json::Value& value = (*m_CurNodeInfo.node)[tag];
		if (value != Json::nullValue)
		{
			if (value.type() == Json::arrayValue)
			{
				tempStr = "";
				for (Json::ValueIterator vIter = value.begin(); vIter != value.end(); ++vIter)
				{
					Json::Value& curVal = *vIter;
					if (curVal.isConvertibleTo(Json::stringValue))
					{
						tempStr.append(curVal.asString());
						tempStr.append(" ");
					}
					else	// break out on error, as something is wrong with the array
					{
						break;
					}
				}

				return tempStr.c_str();
			}
			else if (value.isConvertibleTo(Json::stringValue))
			{
				tempStr = value.asString().c_str();
				return tempStr.c_str();
			}
		}
	}

	return NULL;
}

}
