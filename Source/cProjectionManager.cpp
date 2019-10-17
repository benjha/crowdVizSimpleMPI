//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================
//
#include "cProjectionManager.h"
//
//=======================================================================================
//
ProjectionManager::ProjectionManager( void )
{
	backup_viewport = false;
	type			= ORTHOGRAPHIC;
}
//
//=======================================================================================
//
ProjectionManager::~ProjectionManager( void )
{

}
//
//=======================================================================================
//
void ProjectionManager::setOrthoProjection( unsigned int	w,
											unsigned int	h,
											bool			backup_vp
										  )
{
	type			= ORTHOGRAPHIC;
	backup_viewport	= backup_vp;
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity();
		if( backup_viewport )
		{
			glGetIntegerv( GL_VIEWPORT, &viewport[0] );
			glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
		}
		glOrtho( 0.0, (double)w, 0.0, (double)h, -1.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity();
}
//
//=======================================================================================
//
void ProjectionManager::setOrthoProjection( unsigned int	x,
											unsigned int	y,
											unsigned int    w,
											unsigned int    h,
											unsigned int	l,
											unsigned int	r,
											unsigned int    b,
											unsigned int    t,
											bool			backup_vp
										  )
{
	type			= ORTHOGRAPHIC;
	backup_viewport	= backup_vp;
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity();
		if( backup_viewport )
		{
			glGetIntegerv( GL_VIEWPORT, &viewport[0] );
			glViewport( x, y, (GLsizei)w, (GLsizei)h );
		}
		glOrtho( (double)l, (double)r, (double)b, (double)t, -1.0, 1.0 );
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
			glLoadIdentity();
}
//
//=======================================================================================
//
void ProjectionManager::setTextProjection( unsigned int w, unsigned int h )
{
	type = TEXT;
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( 0, w, h, 0 );
		glMatrixMode( GL_MODELVIEW );
}
//
//=======================================================================================
//
void ProjectionManager::restoreProjection( void )
{
	if( type == ORTHOGRAPHIC )
	{
			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			if( backup_viewport )
			{
				glViewport( viewport[0],
							viewport[1],
							(GLsizei)viewport[2],
							(GLsizei)viewport[3] );
			}
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
	}
	else if( type == TEXT )
	{
			glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		glMatrixMode( GL_MODELVIEW );
	}
}
//
//=======================================================================================
//
GLint* ProjectionManager::getViewport( void )
{
	glGetIntegerv( GL_VIEWPORT, &viewport[0] );
	return viewport;
}
//
//=======================================================================================
//
float ProjectionManager::getAspectRatio( void )
{
	glGetIntegerv( GL_VIEWPORT, &viewport[0] );
	aspect_ratio = (float)viewport[2] / (float)viewport[3];
	return aspect_ratio;
}
//
//=======================================================================================
//
void ProjectionManager::displayText( int x, int y, char* txt )
{
	GLint viewportCoords[ 4 ];
	glColor3f( 0.0, 0.0, 1.0 );
	glGetIntegerv( GL_VIEWPORT, viewportCoords );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0.0, viewportCoords[2], 0.0, viewportCoords[3] );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2i( x, viewportCoords[3]-y );

	while (*txt) {glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *txt ); txt++;}
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
}
//
//=======================================================================================
