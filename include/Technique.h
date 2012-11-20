
#ifndef _Technique_H_
#define _Technique_H_ 1

#include "IEffectManager.h"
#include <map>
#include <string>

#include "Platform.h"

namespace GLSLEFX
{

class Pass;
class Effect;

class Technique : public EFObject
{
private:
	DECLARE_CLASS(Technique);

public:
	virtual ~Technique();

	typedef std::map<unsigned int, Pass *> MAPPASSES;

	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false);

	virtual void DoArchive(Archive &ar, bool reading = true);

	/// add a new pass to the technique
	void AddPass(Pass *pass, unsigned int passNum);

	/// Get an existing pass by number
	Pass* GetPass(unsigned int passNum);

	// grab a vector of passes to go through
	const MAPPASSES *GetPasses();

	unsigned int GetNumPasses() { return m_Passes.size(); }

	// sets the pass X auxiliary texture (auxID) to use texID (opengl texture ID)
	bool SetAuxiliaryTextureID(unsigned int pass, unsigned int auxID, unsigned int texID);

	/// Set the vertex layout used by all techniques and passes. If they
	/// don't share the same layout, call SetLayout on them manually.
	/// @param layout
	/// @param stride size of a single vertex (in bytes)
	/// @param n
	/// @return
	int SetLayout(LAYOUTDESC* layout, size_t stride, size_t n);

	// bind the technique into OpenGL
	void Activate();

	// Tells whenever the effect is valid (it is considered valid if all the techniques and passes
	//are considered valid.
	bool IsValid();


private:
	std::string m_Name;
	// vector of passes for technique
	MAPPASSES m_Passes;

};

}
#endif		// #ifndef _Technique_H_
