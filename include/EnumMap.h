
#ifndef _ENUMMAP_H
#define _ENUMMAP_H

#include <map>
#include <stdio.h>
// compare to const char * as pointers to strings, not pointer comparisons
class CCStringCompare
{
	public:
      bool operator()(const char *x, const char *y) const
      {
    	  return strcmp(x, y) < 0;
      }
};

template <typename Type, class compareFunc = CCStringCompare>
class EnumMap : public std::map<const char *, Type, compareFunc>
{
public:
    EnumMap(const char *key, const Type& val)
    {
        (*this)[key] = val;
    }

    EnumMap& operator()(const char *key, const Type& val)
    {
        (*this)[key] = val;
        return *this;
    }

};

template <typename Type>
class EnumFilter
{
private:
	Type& m_TypeRef;
	const EnumMap<Type>& m_EnumMap;

public:
	EnumFilter(const EnumMap<Type>& enumMap, Type& typeRef ) :
		m_TypeRef(typeRef),
		m_EnumMap(enumMap)
	{
	}

	const EnumFilter& operator =(const char *name) const
	{
		typename EnumMap<Type>::const_iterator enumIter = m_EnumMap.find(name);
		if (enumIter != m_EnumMap.end())
		{
			m_TypeRef = enumIter->second;
		}
		else
		{
			// throw?
			printf("Unable to find %s in the Enum map\n", name);
		}

		return *this;
	}


};

#endif		//#ifndef _ENUMMAP_H
