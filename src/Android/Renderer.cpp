

#include <stdint.h>
#include <unistd.h>
#include <android/native_window.h>

#include "Platform.h"
#include "Renderer.h"
#include "Pass.h"
#include "Effect.h"
#include "Technique.h"

#include "sys/time.h"

namespace GLSLEFX
{

static GLfloat vertices[][3]= {
		{ -1.0f,  1.0f,  0.0f },
		{  1.0f,  1.0f,  0.0f },
		{ -1.0f, -1.0f,  0.0f },
		{  1.0f, -1.0f,  0.0f }
};

static GLfloat texCoords[][2] = {
		{  0.0f, 1.0f },
		{  0.0f, 0.0f },
		{  1.0f, 1.0f },
		{  1.0f, 0.0f }
};

REGISTER_CLASS_SINGLETON(Renderer);

// return number of Milliseconds in system time
static float getNowMS()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return ((float)tv.tv_sec * 1000.0f) + ((float)tv.tv_usec / 1000);
}

Renderer::Renderer()
{
    LOG_INFO("Renderer instance created");
    pthread_mutex_init(&m_Mutex, 0);
    sem_init(&m_Semaphore, 0, 0);
    m_bRenderingEnabled = false;
    m_Display = EGL_NO_DISPLAY;
    m_Surface = EGL_NO_SURFACE;
    m_Context = EGL_NO_CONTEXT;
    m_LocalThreadContext = EGL_NO_CONTEXT;
    m_Window = NULL;
    m_RendererLockCount = 0;
	m_Position = 0;
	m_TexCoord = 0;
	m_TexCoord2 = 0;
	m_ProgramID = 0;
	m_Msg = MSG_NONE;

    pthread_create(&m_ThreadID, 0, ThreadCallback, this);

	IEffectManager::GetInstance()->SetLockFunction(LockUnLock);


}

void Renderer::Stop()
{
    // send message to render thread to stop rendering
    LOG_INFO("Stopping renderer thread");
    pthread_mutex_lock(&m_Mutex);
    m_Msg = MSG_RENDER_LOOP_EXIT;
    pthread_mutex_unlock(&m_Mutex);
    pthread_join(m_ThreadID, 0);
    LOG_INFO("Renderer thread stopped");
    pthread_mutex_destroy(&m_Mutex);

    Destroy();
}

void Renderer::Resume()
{
    pthread_mutex_lock(&m_Mutex);
    m_bRenderingEnabled = true;
    pthread_mutex_unlock(&m_Mutex);
}

void Renderer::Pause()
{
    pthread_mutex_lock(&m_Mutex);
    m_bRenderingEnabled = false;
    pthread_mutex_unlock(&m_Mutex);
}

ANativeWindow *Renderer::SetWindow(ANativeWindow *window)
{
	ANativeWindow *oldWindow = m_Window;
	if (window == NULL)
	{
		Pause();
		Destroy();
		IEffectManager::GetInstance()->SelectTechnique(NULL, NULL);
		m_Window = NULL;
		return oldWindow;
	}

	if (m_Window != window)
	{
		LOG_INFO("Created Thread, going to wait for create context");
		// notify render thread that window has changed
		pthread_mutex_lock(&m_Mutex);
		m_Window = window;
		m_Msg = MSG_INIT;
		pthread_mutex_unlock(&m_Mutex);

		// wait for Rendering thread to process createContext stuff.
		sem_wait(&m_Semaphore);

		LOG_INFO("Semaphore concluded that MSG_INIT processed");

		EGLint contextAttrs[] =
		{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

		if (m_Context == 0)
		{
			LOG_ERROR("m_Context wasn't initialized for some reason");
		}

		// create a shared context for this thread
		m_LocalThreadContext = eglCreateContext(m_Display, m_Config, m_Context, contextAttrs);
		if (m_LocalThreadContext == 0)
		{
			LOG_ERROR("Unable to create a shared thread context Error: %d", eglGetError());
		}

	}

    return oldWindow;
}

void Renderer::GetVertexAttributes(unsigned int programID)
{
    /* Get attribute locations of non-fixed attributes like colour and texture coordinates. */
    m_Position = glGetAttribLocation(programID, "position");
    m_TexCoord = glGetAttribLocation(programID, "inputTextureCoordinate");
    m_TexCoord2 = glGetAttribLocation(programID, "inputTextureCoordinate");

}

void Renderer::RenderLoop()
{
    
    LOG_INFO("Entered RenderLoop");

    bool exitLoop = false;
    bool didMessageFirstDraw = false;
    m_RenderThreadID = gettid();

    while (!exitLoop)
    {
        pthread_mutex_lock(&m_Mutex);
        // process incoming messages
        switch (m_Msg) {
            case MSG_INIT:
        		Init();
        		LOG_INFO("posting semaphore for completion of Init()");
        		sem_post(&m_Semaphore);
                break;

            case MSG_RENDER_LOOP_EXIT:
            	exitLoop = true;
                break;

            default:
                break;
        }

        m_Msg = MSG_NONE;
        
        if (m_Display && m_bRenderingEnabled && (m_Window != NULL))
        {
    		// for this thread make this the current context
    		eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);

        	if (!didMessageFirstDraw)
        	{
        		didMessageFirstDraw = true;
        		LOG_INFO("Calling DrawFrame for the first time!");
        	}

        	// frame time expects hundreths of seconds
        	float frameTime = getNowMS() / 1000.0f;
        	Effect *curEffect = IEffectManager::GetInstance()->GetCurrentEffect();
        	if (curEffect != NULL)
        	{
        		Technique *curTechnique = curEffect->GetCurrentTechnique();
        		if (curTechnique != NULL)
        		{

        			Technique::MAPPASSES::const_iterator passEndIter = curTechnique->GetPasses()->end();
        			for (Technique::MAPPASSES::const_iterator passIter =  curTechnique->GetPasses()->begin(); passIter != passEndIter; ++passIter)
        			{
            			Pass *curPass;
            			curPass = passIter->second;

        				unsigned int newProgramID = curPass->GetProgramID();
        				if (newProgramID != m_ProgramID)
        				{
            				GetVertexAttributes(newProgramID);
            				m_ProgramID = newProgramID;
            			}

        				curPass->SetUniform("fragment", "frameTime", 1, &frameTime);
            			curPass->Activate();
            			DrawFrame();
        			}
        		}
        	}

        	if (!eglSwapBuffers(m_Display, m_Surface))
            {
                LOG_ERROR("eglSwapBuffers() returned error %d", eglGetError());
            }

            eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        }
        
        pthread_mutex_unlock(&m_Mutex);

    }
    
    LOG_INFO("Exiting RenderLoop");
    
    return;
}

void Renderer::LockUnLock(bool lock)
{
	Renderer *renderer = GetInstance();
	if (lock)
	{
		renderer->Lock();
	}
	else
	{
		renderer->Unlock();
	}
}

void Renderer::Lock()
{
	if (gettid() != m_RenderThreadID)
	{
		if (m_RendererLockCount == 0)
		{
			// create a shared context for this thread
			pthread_mutex_lock(&m_Mutex);
			// for this thread make this the current context
			eglMakeCurrent(m_Display, m_Surface, m_Surface, m_LocalThreadContext);
		}

		m_RendererLockCount++;
	}
}

void Renderer::Unlock()
{
	if (gettid() != m_RenderThreadID)
	{
		if (m_RendererLockCount != 0)
		{
			m_RendererLockCount--;
			if (m_RendererLockCount == 0)
			{
				eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
				pthread_mutex_unlock(&m_Mutex);
			}
		}
	}
}

unsigned int Renderer::CreateTextureID()
{
	GLuint texID;

	glGenTextures(1, &texID);

	return texID;
}

void Renderer::DeleteTextureID(unsigned int texID)
{
	if (texID != 0)
	{
		glDeleteTextures(1, &texID);
	}
}

bool Renderer::Init()
{
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_BLUE_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_RED_SIZE, 5,
        EGL_NONE
    };
    EGLint numConfigs;
    GLfloat ratio;
    
    LOG_INFO("Initializing context");
    
    if ((m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY)
    {
        LOG_ERROR("eglGetDisplay() returned error %d", eglGetError());
        return false;
    }

    LOG_INFO("Initializing Display");
    if (!eglInitialize(m_Display, 0, 0)) {
        LOG_ERROR("eglInitialize() returned error %d", eglGetError());
        return false;
    }

    LOG_INFO("Choosing config");
    if (!eglChooseConfig(m_Display, attribs, &m_Config, 1, &numConfigs)) {

        LOG_ERROR("eglChooseConfig() returned error %d", eglGetError());
        return false;
    }

    LOG_INFO("getting format");
    if (!eglGetConfigAttrib(m_Display, m_Config, EGL_NATIVE_VISUAL_ID, &m_Format))
    {
        LOG_ERROR("eglGetConfigAttrib() returned error %d", eglGetError());
        return false;
    }

    EGLint contextAttrs[] = {
         EGL_CONTEXT_CLIENT_VERSION, 2,
         EGL_NONE
    };

    LOG_INFO("creating context");
    if (!(m_Context = eglCreateContext(m_Display, m_Config, 0, contextAttrs)))
    {
        LOG_ERROR("eglCreateContext() returned error %d", eglGetError());
        return false;
    }
    
    LOG_INFO("Init Context succeeded");

    ANativeWindow_setBuffersGeometry(m_Window, 0, 0, m_Format);

    if (!(m_Surface = eglCreateWindowSurface(m_Display, m_Config, m_Window, 0)))
    {
        LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
        return false;
    }

    if (!eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context))
    {
        LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
        return false;
    }

    if (!eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &m_Width) ||
        !eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &m_Height))
    {
        LOG_ERROR("eglQuerySurface() returned error %d", eglGetError());
        return false;
    }

    LOG_INFO("Created surface of size %d x %d", m_Width, m_Height);

    glClearColor(255, 255, 255, 0);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, m_Width, m_Height);

	eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    return true;
}

Renderer::~Renderer()
{
	// stops and destroy's context
	Stop();
}

void Renderer::Destroy()
{
    LOG_INFO("Destroying context");

    if (m_Display != EGL_NO_DISPLAY)
    {
    	eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    	if (m_Context != EGL_NO_CONTEXT)
    	{
    		eglDestroyContext(m_Display, m_Context);
    	}
    	if (m_LocalThreadContext != EGL_NO_CONTEXT)
    	{
    		eglDestroyContext(m_Display, m_LocalThreadContext);
    	}
    	if (m_Surface != EGL_NO_SURFACE)
    	{
    		eglDestroySurface(m_Display, m_Surface);
    	}
    	eglTerminate(m_Display);
    }
    
    m_Display = EGL_NO_DISPLAY;
    m_Surface = EGL_NO_SURFACE;
    m_Context = EGL_NO_CONTEXT;
	m_LocalThreadContext = EGL_NO_CONTEXT;
	m_Window = 0;
    m_Width = 0;
    m_Height = 0;
}

void Renderer::DrawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pass the triangle vertex positions to the shader
    glVertexAttribPointer(m_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(m_Position);

    // Pass the texture coordinates to the shader
	glVertexAttribPointer(m_TexCoord, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glEnableVertexAttribArray(m_TexCoord);

    // Pass the texture coordinates to the shader
	glVertexAttribPointer(m_TexCoord2, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	glEnableVertexAttribArray(m_TexCoord2);

    glFrontFace(GL_CW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //m_Angle += 1.2f;
}

void* Renderer::ThreadCallback(void *myself)
{
    Renderer *renderer = (Renderer *)myself;

    renderer->RenderLoop();

    pthread_exit(0);
    
    return 0;
}

}
