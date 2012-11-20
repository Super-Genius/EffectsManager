

#include <stdint.h>
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "IEffectManager.h"
#include "JniApi.h"
#include "Platform.h"
#include "Renderer.h"
#include "ResIO.h"

using namespace GLSLEFX;

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererOnResume(JNIEnv* jenv, jclass)
{
    LOG_INFO("RendererOnResume");
    Renderer::GetInstance()->Resume();
}

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererOnPause(JNIEnv* jenv, jclass)
{
    LOG_INFO("RendererOnPause");
    Renderer::GetInstance()->Pause();
}

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererOnStop(JNIEnv* jenv, jclass)
{
    LOG_INFO("RendererOnStop");
    Renderer::GetInstance()->Stop();
}

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererSetSurface(JNIEnv* jenv, jclass, jobject surface)
{
    if (surface != 0)
    {
    	ANativeWindow *window = ANativeWindow_fromSurface(jenv, surface);
        LOG_INFO("Got window %p", window);
        Renderer::GetInstance()->SetWindow(window);
    }
    else
    {
        LOG_INFO("Releasing window");
        ANativeWindow *window = Renderer::GetInstance()->SetWindow(NULL);
        ANativeWindow_release(window);
    }
}

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_EffectManagerSelectTechnique(JNIEnv* jenv, jclass, jstring effectName, jstring techniqueName)
{
	const char *effectNameString = jenv->GetStringUTFChars(effectName, NULL);
	const char *techiqueNameString = jenv->GetStringUTFChars(techniqueName, NULL);

	IEffectManager::GetInstance()->SelectTechnique(effectNameString, techiqueNameString);

	jenv->ReleaseStringUTFChars(effectName, effectNameString);
	jenv->ReleaseStringUTFChars(techniqueName, techiqueNameString);
}

JNIEXPORT bool JNICALL Java_telly_EffectsManager_EffectsManager_EffectManagerLoad(JNIEnv* jenv, jclass, jstring effectFileName)
{
	const char *effectFileNameString = jenv->GetStringUTFChars(effectFileName, NULL);

	bool retVal = IEffectManager::GetInstance()->LoadEffectFile(effectFileNameString);

	jenv->ReleaseStringUTFChars(effectFileName, effectFileNameString);

	return retVal;
}

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererUpdateAuxiliary(JNIEnv* jenv, jclass, jint width, jint height, jbyteArray array)
{
	//static char tempAux[] = { 255, 0, 255, 0 };

	IEffectManager *emMan = IEffectManager::GetInstance();
	static bool bFirstLoad = false;
	static jbyteArray lastAuxiliaryObject = NULL;
	jboolean isCopy;
	//unsigned char tempBuff[640*480];

	if ((lastAuxiliaryObject != NULL) && (lastAuxiliaryObject != array))
	{
		jenv->DeleteGlobalRef(lastAuxiliaryObject);
		jenv->NewGlobalRef(array);
	}


	if (array == NULL)
	{
		LOG_ERROR("Auxiliary array (texture buffer) is NULL");
		return;
	}

	jbyte* buffer = jenv->GetByteArrayElements(array, &isCopy);
	jsize length = jenv->GetArrayLength(array);

	if (buffer == NULL)
	{
		LOG_ERROR("Auxiliary texture buffer is NULL");
		return;
	}

	if (!bFirstLoad)
	{
		LOG_INFO("Loading auxiliary texture for first time, width %d, height %d",
				width, height);
		bFirstLoad = true;
	}

	//width = 512;
	//height = 512;
	//emMan->UpdateAuxiliaryTexture(1, GL_LUMINANCE, 2, 2, (unsigned char *)tempAux, 4);
	emMan->UpdateAuxiliaryTexture(1, GL_LUMINANCE, width, height, (unsigned char *)buffer, (width * height));
	emMan->UpdateAuxiliaryTexture(2, GL_LUMINANCE_ALPHA, width/2, height/2, (unsigned char *)(buffer + (width * height)), (width * (height /2 )));

	jenv->ReleaseByteArrayElements(array, buffer, JNI_ABORT);
}

JNIEXPORT void Java_telly_EffectsManager_EffectsManager_SetAppDirectory(JNIEnv * env, jobject, jstring pkgname, jobject assetManager)
{
	const char *sPkgName = env->GetStringUTFChars( pkgname, NULL);
	AAssetManager *assetMgr = AAssetManager_fromJava(env, assetManager);
	// add reference for eternity, as we don't want it released.
	env->NewGlobalRef(assetManager);
    ResIO *resIO = ResIO::GetInstance();
    resIO->SetAppResourceInfo(sPkgName, assetMgr);
	env->ReleaseStringUTFChars(pkgname, sPkgName);
}

JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_ReInitEffects(JNIEnv* jenv, jclass)
{
    LOG_INFO("ReInitEffects");
    IEffectManager::GetInstance()->ReInitEffects();
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv *env;
	LOG_INFO("JNI_OnLoad called");
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	{
		LOG_ERROR("Failed to get the environment using GetEnv()");
		return -1;
	}

	return JNI_VERSION_1_6;
}


