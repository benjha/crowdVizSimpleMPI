//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

using namespace std;

typedef struct RenderContextRec
{
    GLXContext	ctx;
    Display 	*dpy;
    Window 		win;
    GLXPbuffer 	pbuffer;
    int 		winWidth;
    int 		winHeight;
    int			winX;
    int			winY;
    int			mouseX;
    int			mouseY;
    bool		windowless;
} RenderContext;


typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);



void 		initEarlyGLXfnPointers	(	);
// Tell X we are going to use the display, string ":0.1" access GPU1
//											string ":0.0"  access GPU0
void 		CreateWindow			( RenderContext *rcx, const char *display, int mayorGLVersion, int minorGLVersion ); // creates a window based OpenGL context
void		CreateWindowless		( RenderContext *rcx, const char *display, int mayorGLVersion, int minorGLVersion ); // creates a windowless OpenGL context

void 		closeContext			( RenderContext *rcx );
void		setX11Events			( RenderContext *rcx );

//static int 	ctxErrorHandler			( Display *dpy, XErrorEvent *ev );



