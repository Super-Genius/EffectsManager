
#ifndef _JNIAPI_H
#define _JNIAPI_H

#ifdef __cplusplus

extern "C"
{
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererOnResume(JNIEnv* jenv, jclass);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererOnPause(JNIEnv* jenv, jclass);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererOnStop(JNIEnv* jenv, jclass);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererSetSurface(JNIEnv* jenv, jclass, jobject surface);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererUpdateAuxiliary(JNIEnv* jenv, jclass, jint width, jint height, jbyteArray array);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_RendererSetSurface(JNIEnv* jenv, jclass, jobject surface);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_EffectManagerSelectTechnique(JNIEnv* jenv, jclass, jstring effectName, jstring techniqueName);
    JNIEXPORT void Java_telly_EffectsManager_EffectsManager_SetAppDirectory(JNIEnv * env, jobject, jstring pkgname, jobject assetManager);
    JNIEXPORT bool JNICALL Java_telly_EffectsManager_EffectsManager_EffectManagerLoad(JNIEnv* jenv, jclass, jstring effectFileName);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_InitRenderer(JNIEnv* jenv, jclass);
    JNIEXPORT void JNICALL Java_telly_EffectsManager_EffectsManager_ReInitEffects(JNIEnv* jenv, jclass);

};

#endif

#endif // _JNIAPI_H
