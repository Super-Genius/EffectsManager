
#include "IClassManager.h"
#include "IFileManager.h"
#include "IEffectManager.h"
#include "SOIL.h"

#include "TextureIO.h"
#include <stdlib.h>
#include <stdio.h>

namespace GLSLEFX
{

REGISTER_CLASS_SINGLETON(TextureIO);

TextureIO::TextureIO()
{
}

const char *TextureIO::GetLastError()
{
	return m_LastError.c_str();
}

bool TextureIO::Load(const char *sourceStr, unsigned int &texID)
{
	unsigned int newTexID = 0;

	char *data;
	unsigned long fLength;
	IFileManager *fm = IFileManager::GetInstance();
	if (!fm->Load(sourceStr, (void **)&data, &fLength))
	{
		m_LastError = fm->GetLastError();
		return false;
	}

	IEffectManager::GetInstance()->LockContext(true);
	newTexID = SOIL_load_OGL_texture_from_memory(
	  (const unsigned char *)data,
	  fLength,
	  SOIL_LOAD_AUTO,
	  texID,
	  SOIL_FLAG_MIPMAPS
	);

	IEffectManager::GetInstance()->LockContext(false);
	if (newTexID == 0)
	{
		m_LastError = SOIL_last_result();
		return false;
	}

	texID = newTexID;

	return true;
}

}
