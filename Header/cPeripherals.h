//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================
#include <GL/freeglut.h>
//
//=======================================================================================
//
enum
{
	_keyChangedFlag = 0,
	_key,
	_ctrlDown,
	_lMouseDown,
	_rMouseDown,
	_mMouseDown,
	_mouseChangedFlag,
	_mouseX,
	_mouseY,
	_flagExit
};
int				MPI_interaction[_flagExit+1] = {0};
//
//=======================================================================================
//
void keyboard( unsigned char key, int x, int y )
{
	vec3 camPos = vec3( 0.0f );
	vec3 camDir = vec3( 0.0f );
	vec3 camUp  = vec3( 0.0f );

	vec3 ps (0,1,0);

	switch( key )
	{
	case 43:	//'+'
		/*
		predef_cam_index = (predef_cam_index + 1) % predef_cam_pos.size();
		camPos = vec3(	predef_cam_pos[predef_cam_index].x,
                        predef_cam_pos[predef_cam_index].y,
                        predef_cam_pos[predef_cam_index].z );
		camDir = vec3(  predef_cam_dir[predef_cam_index].x,
                        predef_cam_dir[predef_cam_index].y,
                        predef_cam_dir[predef_cam_index].z );
		camUp  = vec3(  predef_cam_up[predef_cam_index].x,
                        predef_cam_up[predef_cam_index].y,
                        predef_cam_up[predef_cam_index].z );
        camera->setPosition( camPos );
        camera->setDirection( camDir );
        camera->setUpVec( camUp );
        camera->setView();
        */
		break;
	case 45:	//'-'
/*
		pdi = (int)predef_cam_index;
		if( (pdi-1) >= 0  )
		{
			predef_cam_index = predef_cam_index - 1;
		}
		else
		{
			predef_cam_index = predef_cam_pos.size() - 1;
		}
		camPos = vec3(	predef_cam_pos[predef_cam_index].x,
                        predef_cam_pos[predef_cam_index].y,
                        predef_cam_pos[predef_cam_index].z );
		camDir = vec3(  predef_cam_dir[predef_cam_index].x,
                        predef_cam_dir[predef_cam_index].y,
                        predef_cam_dir[predef_cam_index].z );
		camUp  = vec3(  predef_cam_up[predef_cam_index].x,
                        predef_cam_up[predef_cam_index].y,
                        predef_cam_up[predef_cam_index].z );
        camera->setPosition( camPos );
        camera->setDirection( camDir );
        camera->setUpVec( camUp );
        camera->setView();
*/
		break;
	case 19:	//'CTRL+s'
		camera->moveDown( camAccel );
		camera->setView();
		break;
	case 23:	//'CTRL+w'

		//camera->moveAround(10, ps);
		camera->moveUp( camAccel );
		camera->setView();
		break;

	case 27:
		glutLeaveMainLoop();
		exit( 0 );
		break;
	case 'a':
		camera->move( camAccel, 90 );
		//camera->moveAround ( camAccel, ps);
		break;
	case 'c':
		/*
		if( camera == camera1 )
		{
			camera = camera2;
		}
		else
		{
			camera = camera1;
		}
		camNear = camera->getFrustum()->getNearD();
		*/
		break;
	case 'd':
		camera->move( camAccel, -90 );
		break;
	case 'w':
		camera->moveForward( camAccel );
		break;
	case 's':
		camera->moveBackward( camAccel );
		break;
	case 'H':

		break;
	case ' ':

		break;
	case 'o':

		break;
	case 'e':

		break;
	case 'r':

		break;
	case 'R':

		break;
	case 'm':

		break;
	case '.':
		break;
	case ',':

		break;
	case 'S':

		break;
	}
}
//
//=======================================================================================
//
void keyboard_master( unsigned char key, int x, int y )
{
	vec3 camPos = vec3( 0.0f );
	vec3 camDir = vec3( 0.0f );
	vec3 camUp  = vec3( 0.0f );

	vec3 ps (0,1,0);

	switch( key )
	{
	case 43:	//'+'
/*
		predef_cam_index = (predef_cam_index + 1) % predef_cam_pos.size();
		camPos = vec3(	predef_cam_pos[predef_cam_index].x,
                        predef_cam_pos[predef_cam_index].y,
                        predef_cam_pos[predef_cam_index].z );
		camDir = vec3(  predef_cam_dir[predef_cam_index].x,
                        predef_cam_dir[predef_cam_index].y,
                        predef_cam_dir[predef_cam_index].z );
		camUp  = vec3(  predef_cam_up[predef_cam_index].x,
                        predef_cam_up[predef_cam_index].y,
                        predef_cam_up[predef_cam_index].z );
        camera->setPosition( camPos );
        camera->setDirection( camDir );
        camera->setUpVec( camUp );
        camera->setView();
*/
		break;
	case 45:	//'-'
/*
		pdi = (int)predef_cam_index;
		if( (pdi-1) >= 0  )
		{
			predef_cam_index = predef_cam_index - 1;
		}
		else
		{
			predef_cam_index = predef_cam_pos.size() - 1;
		}
		camPos = vec3(	predef_cam_pos[predef_cam_index].x,
                        predef_cam_pos[predef_cam_index].y,
                        predef_cam_pos[predef_cam_index].z );
		camDir = vec3(  predef_cam_dir[predef_cam_index].x,
                        predef_cam_dir[predef_cam_index].y,
                        predef_cam_dir[predef_cam_index].z );
		camUp  = vec3(  predef_cam_up[predef_cam_index].x,
                        predef_cam_up[predef_cam_index].y,
                        predef_cam_up[predef_cam_index].z );
        camera->setPosition( camPos );
        camera->setDirection( camDir );
        camera->setUpVec( camUp );
        camera->setView();
*/
		break;
	case 19:	//'CTRL+s'
		camera->moveDown( camAccel );
		camera->setView();
		break;
	case 23:	//'CTRL+w'

		//camera->moveAround(10, ps);
		camera->moveUp( camAccel );
		camera->setView();
		break;

	case 27:
		//cout << "flagExit: " << flagExit << endl;

		MPI_interaction[_flagExit] = true;
		//exit( 0 );
		break;
	case 'a':
		camera->move( camAccel, 90 );
		//camera->moveAround ( camAccel, ps);
		break;
	case 'c':
		/*
		if( camera == camera1 )
		{
			camera = camera2;
		}
		else
		{
			camera = camera1;
		}
		camNear = camera->getFrustum()->getNearD();
		*/
		break;
	case 'd':
		camera->move( camAccel, -90 );
		break;
	case 'w':
		camera->moveForward( camAccel );
		break;
	case 's':
		camera->moveBackward( camAccel );
		break;
	case 'H':

		break;
	case ' ':

		break;
	case 'o':

		break;
	case 'e':

		break;
	case 'r':

		break;
	case 'R':

		break;
	case 'm':

		break;
	case '.':
		break;
	case ',':

		break;
	case 'S':

		break;
	}
	MPI_interaction[_keyChangedFlag] = true;
	MPI_interaction[_key] = key;

}
//
//=======================================================================================
//
void special( int key, int x, int y )
{
	glut_mod = glutGetModifiers();
	switch( key )
	{
		case GLUT_KEY_LEFT:
			camera->moveLeft( camAccel * 20.0f );
			break;
		case GLUT_KEY_RIGHT:
			camera->moveRight( camAccel * 20.0f );
			break;
		case GLUT_KEY_UP:
			camera->moveForward( camAccel * 20.0f );
			break;
		case GLUT_KEY_DOWN:
			camera->moveBackward( camAccel * 20.0f );
			break;
	}
}
//
//=======================================================================================
//
void mouse ( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN )
	{
		if( button == GLUT_LEFT_BUTTON )
		{
			lMouseDown = true;
			rMouseDown = false;
			mMouseDown = false;
		}
		else if( button == GLUT_RIGHT_BUTTON )
		{
			lMouseDown = false;
			rMouseDown = true;
			mMouseDown = false;
		}
		else if( button ==  GLUT_MIDDLE_BUTTON  )
		{
			lMouseDown = false;
			rMouseDown = false;
			mMouseDown = true;
		}
	}
}
//
//=======================================================================================
//
void mouse_master( int button, int state, int x, int y )
{
	if( state == GLUT_DOWN )
	{
		if( button == GLUT_LEFT_BUTTON )
		{
			lMouseDown = true;
			rMouseDown = false;
			mMouseDown = false;
		}
		else if( button == GLUT_RIGHT_BUTTON )
		{
			lMouseDown = false;
			rMouseDown = true;
			mMouseDown = false;
		}
		else if( button ==  GLUT_MIDDLE_BUTTON  )
		{
			lMouseDown = false;
			rMouseDown = false;
			mMouseDown = true;
		}
	}

	MPI_interaction[_lMouseDown] = lMouseDown;
	MPI_interaction[_rMouseDown] = rMouseDown;
	MPI_interaction[_mMouseDown] = mMouseDown;
}
//
//=======================================================================================
//
void motion( int x, int y )
{
    vec3 up( 0.0f, 1.0f, 0.0f );
    vec3 ri( 1.0f, 0.0f, 0.0f );

    if (MPI_interaction[_mouseChangedFlag])
    {
    	lMouseDown = MPI_interaction[_lMouseDown];
    	rMouseDown = MPI_interaction[_rMouseDown];
    	mMouseDown = MPI_interaction[_mMouseDown];
    }
    else
    {
		int modifiers = glutGetModifiers();
		if( modifiers & GLUT_ACTIVE_CTRL )
		{
			printf( "GLUT_ACTIVE_CTRL\n" );
			ctrlDown = true;
		}
		else
		{
			ctrlDown = false;
		}
    }

	static int xlast = -1, ylast = -1;
	int dx, dy;

	dx = x - xlast;
	dy = y - ylast;
	if( lMouseDown )
	{
		if( ctrlDown == false )
		{
			if( dx > 0 )
			{
				camera->rotateAngle(  camAccel/4.0f, up );
			}
			else if( dx < 0 )
			{
				camera->rotateAngle( -camAccel/4.0f, up );
			}
		}
		else
		{
			if( dy > 0 )
			{
				camera->rotateAngle( -camAccel/4.0f, ri );
			}
			else if( dy < 0 )
			{
				camera->rotateAngle(  camAccel/4.0f, ri );
			}
		}
	}
	else if( rMouseDown )
	{
		if( dy > 0 )
		{
			camera->rotateAngle( -camAccel/4.0f, ri );
		}
		else if( dy < 0 )
		{
			camera->rotateAngle(  camAccel/4.0f, ri );
		}
	}
	xlast = x;
	ylast = y;

}
//
//=======================================================================================
//
void motion_master ( int x, int y )
{
    vec3 up( 0.0f, 1.0f, 0.0f );
    vec3 ri( 1.0f, 0.0f, 0.0f );


	int modifiers = glutGetModifiers();
	if( modifiers & GLUT_ACTIVE_CTRL )
	{
		printf( "GLUT_ACTIVE_CTRL\n" );
		ctrlDown = true;
	}
	else
	{
		ctrlDown = false;
	}

	static int xlast = -1, ylast = -1;
	int dx, dy;

	dx = x - xlast;
	dy = y - ylast;
	if( lMouseDown )
	{
		if( ctrlDown == false )
		{
			if( dx > 0 )
			{
				camera->rotateAngle(  camAccel/4.0f, up );
			}
			else if( dx < 0 )
			{
				camera->rotateAngle( -camAccel/4.0f, up );
			}
		}
		else
		{
			if( dy > 0 )
			{
				camera->rotateAngle( -camAccel/4.0f, ri );
			}
			else if( dy < 0 )
			{
				camera->rotateAngle(  camAccel/4.0f, ri );
			}
		}
	}
	else if( rMouseDown )
	{
		if( dy > 0 )
		{
			camera->rotateAngle( -camAccel/4.0f, ri );
		}
		else if( dy < 0 )
		{
			camera->rotateAngle(  camAccel/4.0f, ri );
		}
	}
	xlast = x;
	ylast = y;

	MPI_interaction[_mouseX] 			= x;
	MPI_interaction[_mouseY] 			= y;
	MPI_interaction[_ctrlDown] 			= ctrlDown;
	MPI_interaction[_mouseChangedFlag] 	= true;
}
//
//=======================================================================================
//
// This method is used to send interaction events to other nodes
//
void checkMasterUserInput (void)
{
	MPI_Bcast ( MPI_interaction,_flagExit+1, MPI_INT, 	0, MPI_COMM_WORLD );

	//cout << "MPI_interaction[_mouseX] " << MPI_interaction[_mouseX] << endl;

	if (MPI_interaction[_flagExit])
	{
		MPI_Barrier(MPI_COMM_WORLD);
		cout << "Ending process " << pid << " of " << np << endl;
		MPI_Finalize();
		glutLeaveMainLoop();
	}

	if (pid == 0 )
	{
		if (MPI_interaction[_keyChangedFlag])
		{
			MPI_interaction[_keyChangedFlag] = false;
		}
		if (MPI_interaction[_mouseChangedFlag])
		{
			MPI_interaction[_mouseChangedFlag] = false;
		}
	}
	else
	{
		if (MPI_interaction[_keyChangedFlag])
		{
			keyboard (MPI_interaction[_key], MPI_interaction[_mouseX], MPI_interaction[_mouseY]);
		}
		if (MPI_interaction[_mouseChangedFlag])
		{
			motion (MPI_interaction[_mouseX], MPI_interaction[_mouseY]);
		}
	}

}
//
//=======================================================================================
//
