
#include "IClassManager.h"

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <string>
#include "ResIO.h"
#include "Platform.h"

namespace GLSLEFX
{

REGISTER_CLASS_SINGLETON_AS(ResIO, FileIO, REGISTER_PRIORITY_NORMAL + 0x1000);

ResIO::ResIO()
{
	m_AssetManager = NULL;
	m_Error = "";
}

const char *ResIO::GetLastError()
{
	// return file loader last error message
	return m_Error.c_str();
}

bool ResIO::Load(const char *sourceName, std::string& out)
{
	unsigned char *data;
	unsigned long length;
	bool retVal;

	retVal = Load(sourceName, (void **)&data, &length);
	if (retVal)
	{
		out.reserve(length);
		out.append((const char *)data, length);
		delete [] data;
	}

	return retVal;
}

bool ResIO::Load(const char *sourceName, void **out, unsigned long *outLen)
{
	if (m_AssetManager == NULL)
	{
		m_Error = "No Asset Manager set, SetAppResourceInfo/SetAppDirectory (java) should have been called first";
		return false;
	}

	LOG_INFO("Loading file %s", sourceName);

	AAsset * asset;
	asset = AAssetManager_open(m_AssetManager, sourceName, AASSET_MODE_BUFFER);
	if (asset == NULL)
	{
		m_Error = sourceName;
		m_Error += " File Not Found";
		LOG_INFO(m_Error.c_str());
		return false;
	}

	off_t assetLen = AAsset_getLength(asset);

	if (outLen != NULL)
	{
		*outLen = assetLen;
	}

	char *content = new char[assetLen+1];

	unsigned int readLen = AAsset_read(asset, content, assetLen);
	content[assetLen] = '\0';

	if (readLen < 0)
	{
		m_Error = sourceName;
		m_Error += ": Read error reading ";
		m_Error += assetLen;
		m_Error += "bytes";
		LOG_INFO(m_Error.c_str());
		return false;
	}

	if (out != NULL)
	{
		*out = content;
	}

	AAsset_close(asset);

	return true;
}

void ResIO::SetAppResourceInfo(const char *appDirectory, AAssetManager *assetManager)
{
	m_AppDirectory = appDirectory;
	m_AssetManager = assetManager;
}

}
