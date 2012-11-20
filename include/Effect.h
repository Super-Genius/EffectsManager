
#ifndef _EFFECT_H_
#define _EFFECT_H_ 1

#include "IEffectManager.h"
#include <map>
#include <string>

#include "Platform.h"

namespace GLSLEFX
{

class Technique;

// Vertex layout description.
typedef struct LAYOUTDESC {
	std::string name; /* name of component (must match in shader pass) */
	GLint components; /* number of components. 1-4 */
	GLenum type;      /* datatype of attribute */
	off_t offset;     /* offset in struct */
} LAYOUTDESC;

// Effect class, a representation of an effect.
class Effect : public EFObject
{
private:
	DECLARE_CLASS(Effect);

public:
	typedef std::map<std::string, Technique *> NAMETOTECHNIQUEMAP;

	virtual ~Effect();

	// Initialize the object after object and children are all created
	virtual void Init(bool reInit = false);

	virtual void DoArchive(Archive &ar, bool reading = true);
	/// add a new technique to the effect
	void AddTechnique(const std::string& name, Technique *newTechnique);

	/// Get an existing technique by name.
	Technique* GetTechnique(const std::string& name);

	/// Activate/Select a technique
	bool ActivateTechnique(const char *name);

	/// return the techniques that are in this effect
	const NAMETOTECHNIQUEMAP *GetTechniques();

	/// Set the vertex layout used by all techniques and passes. If they
	/// don't share the same layout, call SetLayout on them manually.
	/// @param layout
	/// @param stride size of a single vertex (in bytes)
	/// @param n
	/// @return
	int SetLayout(LAYOUTDESC* layout, size_t stride, size_t n);

	/// Test if the effect is valid (it is considered valid if all the techniques and passes
	/// are considered valid.
	bool IsValid();

	// get the name of the Effect that was loaded.
	const char *GetName()
	{
		return m_Name.c_str();
	}

	Technique *GetCurrentTechnique() { return m_CurrentTechnique; }

private:
	std::string	m_Name;
	NAMETOTECHNIQUEMAP m_TechniquesByName;
	Technique *m_CurrentTechnique;


};


}	// namespace EffectsManager

#endif		//_EFFECT_H_
