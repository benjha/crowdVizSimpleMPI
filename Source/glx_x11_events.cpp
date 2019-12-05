//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#include <iostream>
#include <stdlib.h>
#include "glx_x11_events.h"

static bool 							ctxErrorOccurred 			= false;
static glXCreateContextAttribsARBProc 	glXCreateContextAttribsARB 	= 0;
//
//=======================================================================================
//
void initEarlyGLXfnPointers()
{
	glXCreateContextAttribsARB 	= ( GLXContext(*)
									(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list) )
											glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
/*
 * Uncomment this if using GLX 1.2
	glXChooseFBConfig 			= ( GLXFBConfig*(*)
									( Display *dpy, int screen, const int *attrib_list, int *nelements ) )
											glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");

	glXGetVisualFromFBConfig 	= ( XVisualInfo*(*)
									( Display *dpy, GLXFBConfig config) )
											glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
*/
}
//
//=======================================================================================
//
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}
//
//=======================================================================================
//
void CreateWindow(RenderContext *rcx, const char* display, int mayorGLversion, int minorGLversion)
{
	XSetWindowAttributes 	winAttribs;
	Window					root;
	GLint 					winmask;
    GLint 					nMajorVer 	= 0;
    GLint 					nMinorVer	= 0;
    XVisualInfo 			*visualInfo	= 0;
    GLXFBConfig 			*fbConfigs	= 0;
    int 					numConfigs	= 0;
    static int fbAttribs[] = {
                    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                    GLX_X_RENDERABLE,  True,
                    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                    GLX_DOUBLEBUFFER,  True,
                    GLX_RED_SIZE, 8,
                    GLX_BLUE_SIZE, 8,
                    GLX_GREEN_SIZE, 8,
                    0 };

    // Tell X we are going to use the display
    // if your system have more GPUs you can change the XOpenDisplay arguments:
    //
    // e.g. XOpenDisplay(":0.1"); access GPU1
    //		XOpenDisplay(":0.0"); access GPU0
    //
    // Notice that you previusly need to configure X-server to use both GPUs. Using
    // Nvidia GPUs you need to type in the command line:
    //
    // 				nvidia-xconfig --enable-all-gpus --no-xinerama
    //
    // More details: man nvidia-xconfig

    rcx->dpy = XOpenDisplay(display);

    if (!rcx->dpy)
    {
    	cout << "XOpenDisplay can't open the display. \n";
    	exit (0);
    }
    // Get Version info
    glXQueryVersion(rcx->dpy, &nMajorVer, &nMinorVer);
    cout << "GLX version - " << nMajorVer << "." << nMinorVer << endl;

    if(nMajorVer <= 1 && nMinorVer < 2)
    {
    	cout << "ERROR: GLX 1.2 or greater is necessary\n";
        XCloseDisplay(rcx->dpy);
        exit(0);
    }

    // Get a new fb config that meets our attrib requirements
    fbConfigs = glXChooseFBConfig(rcx->dpy, DefaultScreen(rcx->dpy), fbAttribs, &numConfigs);
    if (!fbConfigs)
    {
    	cout << "Frame buffer configuration is not supported. \n";
    	exit(0);
    }

    visualInfo = glXGetVisualFromFBConfig(rcx->dpy, fbConfigs[0]);
    if (!visualInfo)
    {
    	cout << "Information describing current visual incorrect \n";
    	exit (0);
    }


    // Now create an X window
    root = RootWindow(rcx->dpy, visualInfo->screen);

    winAttribs.event_mask = ExposureMask | VisibilityChangeMask |
                            KeyPressMask | PointerMotionMask    |
                            StructureNotifyMask ;

    winAttribs.border_pixel = 0;
    winAttribs.bit_gravity = StaticGravity;
    winAttribs.colormap = XCreateColormap(rcx->dpy,
                                          root,
                                          visualInfo->visual, AllocNone);
    winmask = CWBorderPixel | CWBitGravity | CWEventMask| CWColormap;



    rcx->win = 0;

    rcx->win = XCreateWindow(rcx->dpy, root, rcx->winX, rcx->winY,
                 rcx->winWidth, rcx->winHeight, 0,
                             visualInfo->depth, InputOutput,
                 visualInfo->visual, winmask, &winAttribs);



     // set hints and properties

    XSizeHints sizehints;
    sizehints.x = rcx->winX;
    sizehints.y = rcx->winY;
    sizehints.width  = rcx->winWidth;
    sizehints.height = rcx->winHeight;
    sizehints.flags = USSize | USPosition;
    XSetNormalHints(rcx->dpy, rcx->win, &sizehints);
    XSetStandardProperties(rcx->dpy, rcx->win, "GLX", 0,
                                  None, (char **)NULL, 0, &sizehints);

//    cout << "Mapped window 0" << endl;
    XMapWindow(rcx->dpy, rcx->win);
//    cout << "Mapped window 1" << endl;


    // Install an X error handler so the application won't exit if GL 4.x
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    // Also create a new GL 4.2 context for rendering
    GLint attribs[9];
    attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    attribs[1] = mayorGLversion;
    attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    attribs[3] = minorGLversion;
    attribs[4] = GLX_CONTEXT_FLAGS_ARB;
    attribs[5] = GLX_CONTEXT_DEBUG_BIT_ARB | GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
    attribs[6] = GLX_CONTEXT_PROFILE_MASK_ARB;
    attribs[7] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    attribs[8] = None;


    rcx->ctx = 0;
//    cout << "BEFORE. rcx->ctx: " <<   rcx->ctx << "rcx->dpy: " << rcx->dpy << " fbConfigs[0]:" << fbConfigs[0] << endl;
    rcx->ctx = glXCreateContextAttribsARB(rcx->dpy, fbConfigs[0], 0, true, attribs);

//    cout << "glXCreateContextAttribsARB 1" <<  rcx->ctx << endl;
    glXMakeCurrent(rcx->dpy, rcx->win, rcx->ctx);
//    cout << "glXMakeCurrent " << endl;

    // Sync to ensure any errors generated are processed.
    XFree ( fbConfigs );
    XSync ( rcx->dpy, False );
//    cout << "XSync " << endl;

    if ( !ctxErrorOccurred && rcx->ctx )
    {
    	cout << "Created GL 4.2 context\n";
    }
    else
    {
    	cout << "Unable Created GL 4.2 context\n";
    	exit (0);
    }

//    cout << "XSetErrorHandler 0 " << endl;
    XSetErrorHandler (oldHandler);
//    cout << "XSetErrorHandler 1 " << endl;

    cout << "Vendor: " << glXGetClientString (rcx->dpy, GLX_VENDOR  ) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER)	<< endl;


    rcx->windowless = false;
/*
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        // Problem: glewInit failed, something is seriously wrong.
        //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
*/
}
//
//=======================================================================================
//
void CreateWindowless ( RenderContext *rcx, const char *display, int mayorGLversion, int minorGLversion )
{
	GLXFBConfig 			*fbConfigs	= 0;
	int 					numConfigs	= 0;
    int 					nMajorVer 	= 0;
    int 					nMinorVer	= 0;

    static int fbAttribs[] = {
                    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                    GLX_X_RENDERABLE,  True,
                    GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
                    GLX_DOUBLEBUFFER,  True,
                    GLX_RED_SIZE, 8,
                    GLX_BLUE_SIZE, 8,
                    GLX_GREEN_SIZE, 8,
                    0 };

    // Tell X we are going to use the display
    // if your system have more GPUs you can change the XOpenDisplay arguments:
    //
	// e.g. XOpenDisplay(":0.1"); access GPU1
	//		XOpenDisplay(":0.0"); access GPU0
	//
	// Notice that you previusly need to configure X-server to use both GPUs. Using
	// Nvidia GPUs you need to type in the command line:
	//
	// 				nvidia-xconfig --enable-all-gpus --no-xinerama
	//
	// More details: man nvidia-xconfig

    rcx->dpy = XOpenDisplay(display);

	if (!rcx->dpy)
	{
		cout << "XOpenDisplay can't open the display. \n";
		exit (0);
	}
	// Get Version info
	glXQueryVersion(rcx->dpy, &nMajorVer, &nMinorVer);
	cout << "GLX version - " << nMajorVer << "." << nMinorVer << endl;

	if(nMajorVer <= 1 && nMinorVer < 2)
	{
		cout << "ERROR: GLX 1.2 or greater is necessary\n";
		XCloseDisplay(rcx->dpy);
		exit(0);
	}

	// Get a new fb config that meets our attrib requirements
	fbConfigs = glXChooseFBConfig(rcx->dpy, DefaultScreen(rcx->dpy), fbAttribs, &numConfigs);
	if (!fbConfigs)
	{
		cout << "Frame buffer configuration is not supported. \n";
		exit(0);
	}

	// Install an X error handler so the application won't exit if GL 4.x
	// context allocation fails.
	//
	// Note this error handler is global.  All display connections in all threads
	// of a process use the same error handler, so be sure to guard against other
	// threads issuing X commands while this code is running.
	ctxErrorOccurred = false;
	int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    // Also create a new GL 4.2 context for rendering
    GLint attribs[9];
    attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
    attribs[1] = mayorGLversion;
    attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;
    attribs[3] = minorGLversion;
    attribs[4] = GLX_CONTEXT_FLAGS_ARB;
    attribs[5] = GLX_CONTEXT_DEBUG_BIT_ARB | GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
    attribs[6] = GLX_CONTEXT_PROFILE_MASK_ARB;
    attribs[7] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
    attribs[8] = None;


    rcx->ctx = glXCreateContextAttribsARB(rcx->dpy, fbConfigs[0], 0, true, attribs);

    int pbufferAttribs[] = {
        GLX_PBUFFER_WIDTH,  rcx->winWidth,
        GLX_PBUFFER_HEIGHT, rcx->winHeight,
        None
    };
   rcx->pbuffer = glXCreatePbuffer( rcx->dpy, fbConfigs[0], pbufferAttribs );

    glXMakeContextCurrent( rcx->dpy, rcx->pbuffer, rcx->pbuffer, rcx->ctx  );


    XFree( fbConfigs );
    XSync( rcx->dpy, False );

    if ( !ctxErrorOccurred && rcx->ctx )
    	cout << "Created windowless GL " << mayorGLversion << "." << minorGLversion << " context\n";
    else
    	cout << "Unable to create GL context\n";

    XSetErrorHandler (oldHandler);

    cout << "Vendor: " << glXGetClientString (rcx->dpy, GLX_VENDOR  ) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) 					<< endl;

    rcx->windowless = true;
}
//
//=======================================================================================
//
void closeContext(RenderContext *rcx)
{
	// Unbind the context before deleting
    glXMakeCurrent(rcx->dpy, None, NULL);

    glXDestroyContext(rcx->dpy, rcx->ctx);
    rcx->ctx = NULL;

    if (rcx->windowless)
    	glXDestroyPbuffer(rcx->dpy, rcx->pbuffer);
    else
    	XDestroyWindow(rcx->dpy, rcx->win);


    XCloseDisplay(rcx->dpy);
    rcx->dpy = 0;
}
//
//=======================================================================================
//
// If you want a windowless context you don't need to call this method.
//
void setX11Events ( RenderContext *rcx )
{

	if (rcx->dpy && rcx->win)
	{
		// Enables closing window detection
		Atom wmDelete=XInternAtom(rcx->dpy, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(rcx->dpy, rcx->win, &wmDelete, 1);

		XSelectInput(rcx->dpy, rcx->win, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);
	}
	else
	{
		cout << "Warning in setX11Events method: is display or window initialized ? " << endl;
	}
}




