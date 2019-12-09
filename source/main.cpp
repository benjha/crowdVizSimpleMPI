//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================


#include <pthread.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <mpi.h>
#include <GL/glew.h>

#define MULTIGPU

#ifdef MULTIGPU
	#include "glx_x11_events.h"
#else
	#include <GL/freeglut.h>
#endif

#include "cModel.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LoadShaders.h"
#include "framebufferObject.h"
#include "cCamera.h"

#include "cGlobals.h"

using namespace std;

// variables 
cModel *model;

float			fps = 0.0;
long			frames, time_,timebase=0;
long			time_2,timebase2=0;
unsigned long	frameCount;

Camera			*camera;

glm::mat4			transformMat[3];		 // tranformation matrices: [0] - Model; [1] - View; [2] - Projection
unsigned int		MVPmatUBO			= 0; // Modelview Pojection Matrix


unsigned int	agentPosTBO;
unsigned int 	agentPosVBO;

unsigned int	passthruInstancingShader;
ShaderInfo		passthruInstancing[] = {
	{ GL_VERTEX_SHADER,		"shaders/passthruInstancing.vert" },
	{ GL_FRAGMENT_SHADER,	"shaders/passthruInstancing.frag" },
	{ GL_NONE,				NULL }				
};

int				colorLoc;
float			red[3] = {1.0f,0.0f,0.0f};
float			green[3] = {0.0f,1.0f,0.0f};
float			blue[3] = {0.0f,0.0f,1.0f};

//int				screenTexLoc;
unsigned int	screenCompositionShader;
ShaderInfo		screenComposition[] = {
	{ GL_VERTEX_SHADER,		"shaders/screen_composition.vert" },
	{ GL_FRAGMENT_SHADER,	"shaders/screen_composition.frag" },
	{ GL_NONE,				NULL }
};

bool running = 0;
//
//=======================================================================================
//
#ifdef MULTIGPU
void display(RenderContext *rcx)
{
	int i,j;
	myScreenType *ptr;

	glClear			( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	camera->setView ( MVPmatUBO,  transformMat, WIN_WIDTH, WIN_HEIGHT);

	glUseProgram	( passthruInstancingShader );
		glUniform1i (colorLoc, pid );
		glBindBufferBase		( GL_UNIFORM_BUFFER, 0, MVPmatUBO 	); // passing Modelview Projection matrix
		model->drawInstanced	( NUM_CHARACTERS, GL_TEXTURE_BUFFER, agentPosTBO );
	glUseProgram	(0);

	//displayAxis ();

	if (depthPixels)
	{
		// Reading color and depth information
		glReadBuffer ( GL_BACK );
		glReadPixels ( 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_RGBA, GL_FLOAT, (myScreenType*)colorPixels);
		glReadPixels ( 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depthPixels );
		// Storing depth into color pixels
		ptr = (myScreenType*)colorPixels;
		for (i=0, j=0;i<WIN_WIDTH*WIN_HEIGHT*4;i+=4,j++)
		{
			ptr[i+3] = depthPixels[j];
		}
	}

	if (rcx->windowless)
		glXSwapBuffers(rcx->dpy, rcx->pbuffer);
	else
		glXSwapBuffers(rcx->dpy, rcx->win);
}
#endif
//
//=======================================================================================
//
void display(void)
{
	int i,j;
	myScreenType *ptr;

	glClear			( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	camera->setView ( MVPmatUBO,  transformMat, WIN_WIDTH, WIN_HEIGHT);

	glUseProgram	( passthruInstancingShader );
		glUniform1i (colorLoc, pid );
		glBindBufferBase		( GL_UNIFORM_BUFFER, 0, MVPmatUBO 	); // passing Modelview Projection matrix
		model->drawInstanced	( NUM_CHARACTERS, GL_TEXTURE_BUFFER, agentPosTBO );
	glUseProgram	(0);

	//displayAxis ();

	if (depthPixels)
	{
		// Reading color and depth information
		glReadBuffer ( GL_BACK );
		glReadPixels ( 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_RGBA, GL_FLOAT, (myScreenType*)colorPixels);
		glReadPixels ( 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, depthPixels );
		// Storing depth into color pixels
		ptr = (myScreenType*)colorPixels;
		for (i=0, j=0;i<WIN_WIDTH*WIN_HEIGHT*4;i+=4,j++)
		{
			ptr[i+3] = depthPixels[j];
		}
	}

	glutSwapBuffers ();
}
//
//=======================================================================================
//
void display_master (void)
{
	int i = 0;
	glClear			( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//camera->setView ( MVPmatUBO,  transformMat, WIN_WIDTH, WIN_HEIGHT);

	fboMaster->Bind();
		glDrawBuffer (GL_COLOR_ATTACHMENT0_EXT);
		glClear			( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		for (i=0;i<np-1;i++)
		{
			glUseProgram	( screenCompositionShader );
				model->draw(GL_TEXTURE_2D, screenTexIds[i]);
			glUseProgram	(0);
		}
	fboMaster->Disable();

	glUseProgram	( screenCompositionShader );
		model->draw(GL_TEXTURE_2D, fboMasterColorTexId);
	glUseProgram	(0);

	glutSwapBuffers ();
}
//
//=======================================================================================
//
void sendReceiveFrames 		()
{
	int i;

	if (pid != 0)
	{
		MPI_Send ((myScreenType*)colorPixels, pixelSizeMsg, MPI_BYTE, 0, 0, MPI_COMM_WORLD); // master pID = 0
	}
	else
	{
		MPI_Status status;
		for (i=0;i<np-1;i++)
		{
			MPI_Recv (colorPixels[i], pixelSizeMsg, MPI_BYTE, i+1, 0, MPI_COMM_WORLD, &status);

//			printf ("Np: %d, Character culled: %d, Total Chars: %d\n", i-1, culling[0], culling[1]);
		}
//		printf ("\n\n");
//		for (i=64;i<128*4;i+=4)
//		{
//			printf ("[%d, %d, %d, %d]\t", colorPixels[0][i], colorPixels[0][i+1], colorPixels[0][i+2], colorPixels[0][i+3]);
//		}
//		printf ("\n\n");

	}
}
//
//=======================================================================================
//
void uploadScreenTextures () // Target: screen color or depth
{
	int i;

	for (i=0;i<np-1;i++)
	{
		//cout << " screenTexIds " << screenTexIds[i] << " np " << np << endl;
		glTextureImage2DEXT (screenTexIds[i], GL_TEXTURE_2D, 0, GL_RGBA32F,
				WIN_WIDTH, WIN_HEIGHT, 0, GL_RGBA, GL_FLOAT, colorPixels[i]  );


	}
// For debugging:
//			printf ("\n\n");
//			for (i=64;i<128*4;i+=4)
//			{
//				printf ("[%d, %d, %d, %d]\t", colorPixels[0][i], colorPixels[0][i+1], colorPixels[0][i+2], colorPixels[0][i+3]);
//			}
//			printf ("\n\n");

}
//
//=======================================================================================
//
#include "cPeripherals.h"
//
//=======================================================================================
//
// TODO: update idle
void idle (void)
{
	frames++;
	time_=glutGet(GLUT_ELAPSED_TIME);

	if (time_ - timebase > 1000) {
		fps	= frames*1000.0f/(time_-timebase);
		printf ("Process: %d FPS:%4.2f ms:%4.3f \n",pid, fps, 1.0f/fps*1000.0f);
		timebase	= time_;
		frames		= 0;

	}

	if (np>1)
	{
		sendReceiveFrames 		();
		checkMasterUserInput   	();
	}
	if ( pid==0 && np > 1)
	{
		uploadScreenTextures();
	}

	if (pid==0) // Master node uses GLUT
		glutPostRedisplay ();
}
//
//=======================================================================================
//
#ifdef MULTIGPU
void glLoop (RenderContext *rcx)
{
	// TODO: runnig var. should be set to true by the master and sent to the workers
	//while (running)
	while (true)
	{
		if (pid!=0) // composite node
			display (rcx);

		idle	();
	}
}
#endif
//
//=======================================================================================
//
void init (void) 
{
// Next line solve the bug in Ubuntu 3.10 and Nvidia Drivers
//	https://bugs.launchpad.net/ubuntu/+source/nvidia-graphics-drivers-319/+bug/1248642
	int thread=pthread_getconcurrency();

	passthruInstancingShader	= LoadShaders ( passthruInstancing 	);

	glUseProgram	( passthruInstancingShader );
		colorLoc	= glGetUniformLocation( passthruInstancingShader,"color" );
	glUseProgram	( 0);

	glClearColor		( 0.7, 0.7, 0.7, 1.0 	);
	glEnable			( GL_CULL_FACE 			);
	glEnable			( GL_DEPTH_TEST 		);
	glViewport			( 0, 0, WIN_WIDTH, WIN_HEIGHT );

	glGenBuffers				( 1, &MVPmatUBO ); 
	glNamedBufferDataEXT		( MVPmatUBO, sizeof(glm::mat4) * 3, 0, GL_DYNAMIC_DRAW );

	camera = new Camera (0, Camera::FREE, Frustum::RADAR );

	vec3 pos (60,60,300);
	vec3 dir (0.0, 0.0, -1.0 );
	vec3 up	 ( 0.0f, 1.0f, 0.0f );
	vec3 piv = pos + dir;


	camera->setPosition( pos );
	camera->setDirection( dir );
	camera->setUpVec ( up );
	camera->setPivot(piv);
	camera->getFrustum()->setFovY(45);
	camera->getFrustum()->setNearD(0.1);
	camera->getFrustum()->setFarD(PLANE_SIZE*np);

	// generating Texture Buffer Object (TBO) to store agent positions
	// every TBO is associated with a VBO
	int i, sign;
	float *agentPos;
	float x, z;

	agentPos = new float[NUM_CHARACTERS * 3 * sizeof(GLfloat)];
	int col, row;

	if (np > 2 )
	{
		col = (pid-1) / 3;
		row = (pid-1) - col * 3;
		cout << "pid: " << pid << " col: " << col << " row: " << row  << endl;

		for (i=0;i<NUM_CHARACTERS*3;i+=3)
		{
			x = (float)(rand() % (int)(PLANE_SIZE)) + PLANE_SIZE*col;
			z = (float)(rand() % (int)(PLANE_SIZE)) + PLANE_SIZE*row;
			int sign = rand() % 100;

			if( sign < 50 )
			{
				x = -x;
			}
			sign = rand() % 100;
			if( sign < 50 )
			{
				z = -z;
			}

			agentPos[i  ] = x;
			agentPos[i+1] = 0.0;
			agentPos[i+2] = z;
		}
	}
	else
	{
		for (i=0;i<NUM_CHARACTERS*3;i+=3)
		{
			x = (float)(rand() % (int)(PLANE_SIZE));
			z = (float)(rand() % (int)(PLANE_SIZE));
			int sign = rand() % 100;
			if( sign < 50 )
			{
				x = -x;
			}
			sign = rand() % 100;
			if( sign < 50 )
			{
				z = -z;
			}

			agentPos[i  ] = x;
			agentPos[i+1] = 0.0;
			agentPos[i+2] = z;
		}
	}

	glGenBuffers(1, &agentPosVBO);
	glNamedBufferDataEXT (agentPosVBO, NUM_CHARACTERS * 3 * sizeof(GLfloat), agentPos, GL_DYNAMIC_DRAW );
	glGenTextures(1,&agentPosTBO);
	glBindTexture(GL_TEXTURE_BUFFER, agentPosTBO);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, agentPosVBO);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	FREE_ARRAY(agentPos);

	std::string filename = "media/xAxis.obj";
	filename = "media/woman.obj";
	model = new cModel (filename, 1.0f);

}
//
//=======================================================================================
//
void freeAll ()
{
	glDeleteProgram ( passthruInstancingShader	);
	glDeleteBuffers ( 1, &MVPmatUBO 			);

	FREE_MEMORY		( model 					);
	FREE_ARRAY		( colorPixels				);
	FREE_ARRAY		( depthPixels				);

	if (np==1)
		MPI_Finalize();
}
//
//=======================================================================================
// TODO: freeAll_master
void freeAll_master ()
{
	/*
	glDeleteProgram ( passthruInstancingShader	);
	glDeleteProgram ( drawAxisShader			);
	glDeleteBuffers ( 1, &MVPmatUBO 			);

	FREE_MEMORY		( model 					);

	cout << "Calling MPI_Finalize() \n";
	MPI_Finalize();
	*/

	int i;
	for (i=0;i<np-1;i++)
	{
		FREE_ARRAY(colorPixels[i]);
	}
	FREE_ARRAY	( colorPixels	);
	FREE_ARRAY	( screenTexIds	);
	FREE_MEMORY ( fboMaster 	);
}
//
//=======================================================================================
//
//void reshape (int w, int h)
//{
//	glViewport(0, 0, w, h);
//
//	windowWidth = w;
//	windowHeight = h;
//	//cam1.setView (MVPmatUBO, transformMat, w, h);
//	camera->setView( MVPmatUBO, transformMat, w, h );
//}

//
//=======================================================================================
//
void runRenderingGLUT (int argc, char** argv)
{
	stringstream winTitle;
	winTitle << "Process ID " << pid;

	int col, row;
	col = (pid) / 3;
	row = (pid) - col * 3;


	glutInit				( &argc, argv);
	glutInitDisplayMode		( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion	( 4, 3);
	glutInitContextFlags	( GLUT_FORWARD_COMPATIBLE 		);

	glutInitContextProfile	( GLUT_COMPATIBILITY_PROFILE	);
	glutInitWindowSize		( WIN_WIDTH, WIN_HEIGHT			);

	glutInitWindowPosition	( WIN_WIDTH*row, WIN_HEIGHT*col+100);

	glutCreateWindow		( winTitle.str().data() 		);

	int glew_status = glewInit();
	if( glew_status != GLEW_OK )
	{
		printf	( "GLEW initialization failed!.\n" );
		exit	( 0 );
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	init ();
	//glutReshapeFunc			( reshape	);
	if (np == 1)
	{
		glutMouseFunc			( mouse		);
		glutMotionFunc			( motion	);
		glutKeyboardFunc		( keyboard	);
	}
	glutDisplayFunc			( display	);
	glutIdleFunc 			( idle		);
	glutCloseFunc			( freeAll	);

	glutMainLoop();
}
//
//=======================================================================================
//
#ifdef MULTIGPU
void runRenderingGLX (int argc, char** argv)
{
	int col, row;
	col = (pid) / 3;
	row = (pid) - col * 3;

	RenderContext rcx;

	rcx.winWidth 	= WIN_WIDTH;
	rcx.winHeight	= WIN_HEIGHT;
	rcx.winX		= WIN_WIDTH*row;
	rcx.winY		= WIN_HEIGHT*col+100;

	initEarlyGLXfnPointers();

	//Sintaxis only works for 2 GPUs
	if (pid%2)
		CreateWindow(&rcx, ":0.1", 4, 2);
	else
		CreateWindow(&rcx, ":0.0", 4, 2);

	int glew_status = glewInit();
	if( glew_status != GLEW_OK )
	{
		cout <<	"GLEW initialization failed!.\n";
		exit	( 0 );
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	init ();

	glLoop  		( &rcx );
	closeContext 	( &rcx );
	freeAll			(	);

}
#endif
//
//=======================================================================================
//
void init_master ()
{
	int i;
	glClearColor		( 0.7, 0.7, 0.7, 1.0 );
	glEnable			( GL_CULL_FACE );
	glEnable			( GL_DEPTH_TEST );

	glGenTextures 		(np, screenTexIds);
	for ( i=0; i<np-1; i++)
	{
		glBindTexture(GL_TEXTURE_2D, screenTexIds[i]);
			glTexParameterf	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D	( GL_TEXTURE_2D, 0, GL_RGBA32F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL	);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	screenCompositionShader = LoadShaders(screenComposition);

	glGenBuffers				( 1, &MVPmatUBO );
	glNamedBufferDataEXT		( MVPmatUBO, sizeof(glm::mat4) * 3, 0, GL_DYNAMIC_DRAW );

	camera = new Camera (0, Camera::FREE, Frustum::RADAR );

	vec3 pos (60,60,300);
	vec3 dir (0.0, 0.0, -1.0 );
	vec3 up	 ( 0.0f, 1.0f, 0.0f );
	vec3 piv = pos + dir;


	camera->setPosition( pos );
	camera->setDirection( dir );
	camera->setUpVec ( up );
	camera->setPivot(piv);
	camera->getFrustum()->setFovY(45);
	camera->getFrustum()->setNearD(0.1);
	camera->getFrustum()->setFarD(PLANE_SIZE*np);

	string filename = "media/quad.obj";
	model = new cModel (filename, 1.0f);

	fboMaster = new FramebufferObject ();
	glGenTextures (1, &fboMasterColorTexId);
	glBindTexture(GL_TEXTURE_2D, fboMasterColorTexId);
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST										);
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST										);
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP										);
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP										);
		glTexImage2D	( GL_TEXTURE_2D, 0, GL_RGBA32F, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL	);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures (1, &fboMasterDepthTexId);
	glBindTexture(GL_TEXTURE_2D, fboMasterDepthTexId);
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE          );
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR        );
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_LINEAR        );
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,       GL_CLAMP_TO_EDGE );
		glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,       GL_CLAMP_TO_EDGE );
		glTexImage2D	( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8_EXT, WIN_WIDTH, WIN_HEIGHT, 0, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, NULL	);
	glBindTexture(GL_TEXTURE_2D, 0);


	fboMaster->Bind();
		fboMaster->AttachTexture(GL_TEXTURE_2D, fboMasterColorTexId, GL_COLOR_ATTACHMENT0, 0, 0);
		fboMaster->AttachTexture(GL_TEXTURE_2D, fboMasterDepthTexId, GL_DEPTH_ATTACHMENT, 0, 0);
		if (fboMaster->IsValid())
		{
			cout << "Master FBO is complete.\n";
		}
		else
		{
			cout << "Check Master FBO configuration, rendering might not work. \n";
		}
	fboMaster->Disable();



}
//
//=======================================================================================
//
void initScreenArraysMaster ()
{
	int i;
	int wh = WIN_WIDTH * WIN_HEIGHT* 4;


	// initializing arrays that will store each node screen
	colorPixels = new myScreenType *[np-1]; // a screenBuffer per slave
	for (i=0;i<np-1;i++)
	{
		colorPixels[i] = new myScreenType [ wh ];
	}
	pixelSizeMsg = wh * 4 * sizeof (myScreenType );
	screenTexIds = new unsigned int [np];

//	for (i = 0; i< np; i++)
//	{
//		glTextureImage2DEXT (screenTexIds[i], GL_TEXTURE_RECTANGLE, 0, GL_RGBA, WIN_WIDTH, WIN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0  );
//	}
}
//
//=======================================================================================
//
void initScreenArrays ()
{
	int wh = WIN_WIDTH * WIN_HEIGHT;
	colorPixels = new myScreenType*[ wh * 4 ];
	depthPixels	= new myScreenType [ wh ];
	pixelSizeMsg 	= wh * 4 * sizeof (myScreenType );
}
//
//=======================================================================================
//
void runComposite (int argc, char** argv)
{
	stringstream winTitle;
	winTitle << "Process ID " << pid;

	glutInit				( &argc, argv);
	glutInitDisplayMode		( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion	( 4, 3);
	glutInitContextFlags	( GLUT_FORWARD_COMPATIBLE );

	glutInitContextProfile	( GLUT_COMPATIBILITY_PROFILE );
	glutInitWindowSize		( WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition	( 0, 0);
	glutCreateWindow		( winTitle.str().data() );

	int glew_status = glewInit();
	if( glew_status != GLEW_OK )
	{
		printf	( "GLEW initialization failed!.\n" );
		exit	( 0 );
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	//init 					(	);
	init_master 			(	);
//	glutReshapeFunc 		( reshape	);
	glutMotionFunc			( motion_master		);
	glutMouseFunc		 	( mouse_master		);
	glutKeyboardFunc		( keyboard_master	);
	glutDisplayFunc			( display_master	);
	glutIdleFunc 			( idle				);
	glutCloseFunc			( freeAll_master	);



	glutMainLoop();
}
//
//=======================================================================================
//
int main(int argc, char** argv)
{
	//srand						(	time( NULL )				);
	
	// Initializing MPI
	MPI_Init (&argc, &argv);
	// gets the process id
	MPI_Comm_rank (MPI_COMM_WORLD, &pid);
	// gets the number of process
	MPI_Comm_size (MPI_COMM_WORLD, &np);

	cout << "This is process " << pid << " Number of processes " << np << endl;


	MPI_Barrier(MPI_COMM_WORLD);

	if ( pid == 0 ) // master node
	{
		if (np == 1)
		{
			cout << "Status: Running in one node mode \n";

			runRenderingGLUT (argc, argv); // runs in single process rendering mode
		}
		else if (np > 1 )
		{
			cout << "runRenderingAndComposite\n";
			initScreenArraysMaster();
			runComposite  (argc, argv); // Runs multi-process composition mode
		}
	}
	else
	{
		cout << "runRendering\n";
		initScreenArrays();
#ifdef MULTIGPU
		runRenderingGLX (argc, argv);
#else
		runRenderingGLUT (argc, argv);
#endif

	}



	return 0;
}




