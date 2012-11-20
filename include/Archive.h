


#ifndef _ARCHIVE_H_
#define _ARCHIVE_H_

#include <sstream>

namespace GLSLEFX
{

class EFObject;
class IClassManager;

typedef EFObject * (*CREATEOBJFUNC)(const char *className);

class Archive
{
protected:
	Archive();
public:
	virtual ~Archive() { }

	virtual const char *FindTagValue(const char *tag);
	virtual const char *GetNextSibling();
	virtual const char *GetFirstChild(bool resetToRoot = false);


	virtual bool LoadHierarchy(EFObject *parent);

	template <typename T>
	bool Read(const char *tag, T &out)
	{
		const char *valueStr = FindTagValue(tag);
		if (valueStr == NULL)
		{
			return false;
		}

		std::stringstream valueSS(valueStr);

		valueSS >> out;

		return true;
	}

	template <typename T>
	bool Read(const char *tag, T &out, unsigned int count)
	{
		const char *valueStr = FindTagValue(tag);
		if (valueStr == NULL)
		{
			return false;
		}

		std::stringstream valueSS(valueStr);

		for (unsigned int i=0; i<count; i++)
		{
			valueSS >> out[i];
		}

		return true;
	}

	template <typename T>
	bool Read(const char *tag, const T &out)
	{
		const char *valueStr = FindTagValue(tag);
		if (valueStr == NULL)
		{
			return false;
		}

		out = valueStr;

		return true;
	}

	template <typename T>
	void Write(const char *tag, T &out)
	{

	}

private:
	IClassManager *m_ClassManager;
};


}

#endif	//#ifndef _ARCHIVE_H_
