//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================
#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cMacros.h"
#include <GL/glu.h>
#include "cFrustum.h"

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <math.h>


#ifndef CAMERA_ROTATE_AXIS
	#define CAMERA_ROTATE_AXIS
#endif


//=======================================================================================

#ifndef __CAMERA
#define __CAMERA

class Camera
{
public:
						Camera( unsigned int	id,
								int				type,
								int				frustum_type );
						~Camera( void );

	enum			    CAM_TYPE{ FIXED, FREE };
	enum			    CAM_STEREO{ NO_STEREO, ACTIVE_STEREO, ANAGLYPH_STEREO };
	void				setPosition( vec3& pos );
	void				setDirection( vec3& dir );
	void				setUpVec( vec3& up );
	void				setPivot( vec3& pivot );
	void				moveForward( float dist );
	void				moveBackward( float dist );
	void				moveRight( float dist );
	void				moveLeft( float dist );
	void				moveUp( float dist );
	void				moveDown( float dist );
	void				setView( void );
	// for OpenGL 4.x:
	void 				setView (unsigned int MVPBufferId, glm::mat4 *transformMat, unsigned int viewPortWidth, unsigned int viewPortHeight); // uniform buffer id

	void				move( float dir, float deg_angle );
	void				rotateAngle( float deg_angle, vec3& axis );
	void				moveAround( float deg_angle, vec3& axis );
	void				getWorldFrustumCoords( float* coords );
	void				calcUnitVectors( void );
	void				draw( void );
	void				readMatrices( void );
	unsigned int		getId( void );
	void				setInactive( void );
	int					getType( void );
	double				projectionMatrix[16];
	double				modelviewMatrix[16];
	vec3&			    getPosition( void );
	vec3&			    getDirection( void );
	vec3&			    getUp( void );
	Frustum*			getFrustum( void );
	float				updatePlanes( void );
	void				setEyeSeparation( float value );
	void				calcStereoInternals( int stereoMode );
	void				setLeftView( int stereoMode );
	void				setRightView( int stereoMode );

private:
	std::string			name;
	vec3			    X;
	vec3			    Y;
	vec3			    Z;
	vec3			    R;
	vec3			    position;
	vec3			    direction;
	vec3			    up;
	vec3				right;
	vec3			    pivot;
	vec3			    lookat;
	unsigned int		id;
	bool				hasModel;
	bool				hasAudio;
	bool				active;
	int					type;
	float				fR, fG, fB;

	float				eye_separation;
	float				wd2;
	float				ndfl;
	float				fLeft;
	float				fRight;
	float				top;
	float				bottom;
	float				focallength;
	Frustum*			frustum;
};

#endif

//=======================================================================================
