
#ifndef _EFOBJECT_H_
#define _EFOBJECT_H_



namespace GLSLEFX
{

class Archive;

class EFObject
{
protected:
	EFObject() { };
	EFObject *m_Parent;
	const char *m_Type;

public:

	// virtual destructor to make sure children get called
	virtual ~EFObject() { };

	virtual void SetParent(EFObject *parent)
	{
		m_Parent = parent;
	}

	virtual EFObject *GetParent()
	{
		return m_Parent;
	}

	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false) { }

	virtual void SetType(const char *type)
	{
		m_Type = type;
	}

	// virtual so that can get derived class names too.
	virtual const char *GetType()
	{
		return m_Type;
	}

	virtual void DoArchive(Archive &ar, bool reading = true) { };
};

typedef EFObject *(*CREATEOBJECTFUNC)();

}
#endif	// #ifndef _EFOBJECT_H_
