

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "IClassManager.h"
#include <pthread.h>
#include <semaphore.h>

namespace GLSLEFX
{

class Renderer
{
private:
	DECLARE_CLASS_SINGLETON(Renderer);

public:
    virtual ~Renderer();

    // resume/start the rendering process
    void Resume();
    // paus the rendering process
    void Pause();

    // kill the renderer completely
    void Stop();
    // set the window for the renderer
    ANativeWindow *SetWindow(ANativeWindow *window);

    void DeleteTextureID(unsigned int texID);
    unsigned int CreateTextureID();

    static void LockUnLock(bool lock = true);

    void Lock();
    void Unlock();

    void InitSystemRenderer();

private:

    enum RenderThreadMessage
    {
        MSG_NONE = 0,
        MSG_INIT,
        MSG_RENDER_LOOP_EXIT,
    };

    pthread_t m_ThreadID;
    pthread_mutex_t m_Mutex;
    volatile enum RenderThreadMessage m_Msg;
    
    ANativeWindow* m_Window;

    EGLDisplay m_Display;
    EGLSurface m_Surface;
    EGLContext m_Context;
    EGLContext m_LocalThreadContext;
    EGLConfig m_Config;
    int m_Width;
    int m_Height;
    int m_Format;
    
    bool m_bRenderingEnabled;

    // RenderLoop is called in a rendering thread started in Resume() method
    // It creates rendering context and renders scene until Pause() is called
    void RenderLoop();

    // Initialize context, etc
    bool Init();
    
    // initialize the surface
    bool InitSurface();

    // kill the renderer completely
    void Destroy();

    void DrawFrame();

    // Helper method for starting the thread 
    static void* ThreadCallback(void *myself);

    void GetVertexAttributes(unsigned int programID);

    unsigned int m_RendererLockCount;

    unsigned int m_Position;
    unsigned int m_TexCoord;
    unsigned int m_TexCoord2;

    // current passes program ID operating on.
    unsigned int m_ProgramID;

    // the pid of the rendering thread
    pid_t m_RenderThreadID;

    sem_t m_Semaphore;

};


}

#endif // _RENDERER_H_
