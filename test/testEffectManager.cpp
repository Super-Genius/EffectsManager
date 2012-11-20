

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "IEffectManager.h"

using namespace GLSLEFX;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
int    gWindow    = 0;
GLuint gTextureID = 0;

float gSpinX           = 0.0f;
float gSpinY           = 0.0f;
int   gLastMousePositionX = 0;
int   gLastMousePositionY = 0;

//-----------------------------------------------------------------------------
// Name: keyboardFunc()
// Desc: Called when a keyboard key is pressed
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 27:
			glutDestroyWindow( gWindow );
        	exit(0);
			break;
    }
}

//-----------------------------------------------------------------------------
// Name: mouseFunc()
// Desc: Called when a mouse button is pressed
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		gLastMousePositionX = x;
        gLastMousePositionY = y;
	}
}

//-----------------------------------------------------------------------------
// Name: motionFunc()
// Desc: Called when the mouse moves
//-----------------------------------------------------------------------------
void motionFunc( int x, int y )
{
	gSpinX -= (x - gLastMousePositionX);
	gSpinY -= (y - gLastMousePositionY);

	gLastMousePositionX = x;
    gLastMousePositionY = y;
}

//-----------------------------------------------------------------------------
// Name: idleFunc()
// Desc: Called during idle time
//-----------------------------------------------------------------------------
void idleFunc( void )
{
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Name: reshapeFunc()
// Desc: Called when the window size has been changed by the user
//-----------------------------------------------------------------------------
void reshapeFunc( int w, int h )
{
	glViewport( 0, 0, w, h );
}

//-----------------------------------------------------------------------------
// Name: displayFunc()
// Desc: Called when GLUT is ready to render
//-----------------------------------------------------------------------------
void displayFunc( void )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -5.0f );
    glRotatef( -gSpinY, 1.0f, 0.0f, 0.0f );
    glRotatef( -gSpinX, 0.0f, 1.0f, 0.0f );

    glBindTexture( GL_TEXTURE_2D, gTextureID );
    //glInterleavedArrays( GL_T2F_V3F, 0, g_quadVertices );
    //glDrawArrays( GL_QUADS, 0, 4 );
    //glutSolidCube(1.0f);
    glutSolidTeapot(1.0f);
    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc:
//-----------------------------------------------------------------------------
void init()
{
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
	glutInitWindowSize( 640, 480 );
	gWindow = glutCreateWindow( "Test Effect Manager" );

	glutDisplayFunc( displayFunc );
	glutKeyboardFunc( keyboardFunc );
	glutMouseFunc( mouseFunc );
	glutMotionFunc( motionFunc );
	glutReshapeFunc( reshapeFunc );
	glutIdleFunc( idleFunc );

	IEffectManager::GetInstance()->LoadEffectFile("shaders/noir.json");

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable( GL_TEXTURE_2D );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
}

//-----------------------------------------------------------------------------
// Name: main()
// Desc:
//-----------------------------------------------------------------------------
int main( int argc, char **argv )
{
	glutInit( &argc, argv );

    init();

    glutSwapBuffers();
    glutMainLoop();

    return 0;
}


