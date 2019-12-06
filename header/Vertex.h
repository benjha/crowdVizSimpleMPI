//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================


#ifndef __VERTEX
#define __VERTEX	1
	typedef struct VERTEX				
	{							
		GLfloat location[4];		//  0 + 16 =
		GLfloat texture[2];			// 16 +  8 =
		GLfloat normal[3];			// 24 + 12 =
		GLfloat color[4];			// 36 + 16 =
		GLfloat tangent[3];			// 52 + 12 =
	}							 
	Vertex;							// 64 byte padding.

#define BUFFER_OFFSET( i )			(/*NULL +*/ ( i ))

#define LOCATION_OFFSET				BUFFER_OFFSET(  0 )
#define TEXTURE_OFFSET				BUFFER_OFFSET( 16 )
#define NORMAL_OFFSET				BUFFER_OFFSET( 24 )
#define COLOR_OFFSET				BUFFER_OFFSET( 36 )
#define TANGENT_OFFSET				BUFFER_OFFSET( 52 )

#define INITVERTEX( v )								\
		v.location[0]	= 0.0f;						\
		v.location[1]	= 0.0f;						\
		v.location[2]	= 0.0f;						\
		v.location[3]	= 1.0f;						\
		v.texture[0]	= 0.0f;						\
		v.texture[1]	= 0.0f;						\
		v.normal[0]		= 0.0f;						\
		v.normal[1]		= 0.0f;						\
		v.normal[2]		= 0.0f;						\
		v.color[0]		= 0.0f;						\
		v.color[1]		= 0.0f;						\
		v.color[2]		= 0.0f;						\
		v.color[3]		= 0.0f;						\
		v.tangent[0]	= 0.0f;						\
		v.tangent[1]	= 0.0f;						\
		v.tangent[2]	= 0.0f

#endif

//=======================================================================================

#ifndef __LOCATION4
#define __LOCATION4 1
	typedef struct LOCATION4
	{
		GLfloat location[4];
	}
	Location4;

#define INITLOCATION4( l )						\
	l.location[0]	= 0.0f;						\
	l.location[1]	= 0.0f;						\
	l.location[2]	= 0.0f;						\
	l.location[3]	= 1.0f;

#endif

//=======================================================================================

#ifndef __NORMAL
#define __NORMAL 1
	typedef struct NORMAL
	{
		GLfloat normal[3];
	}
	Normal;

#define INITNORMAL( n )							\
	n.normal[0]	= 0.0f;							\
	n.normal[1]	= 0.0f;							\
	n.normal[2]	= 0.0f;						

#endif

//=======================================================================================

#ifndef __UV
#define __UV 1

	typedef struct UV
	{
		GLfloat uv[2];
	}Uv;

#define INITUV( u )								\
	u.uv[0]			= 0.0f;						\
	u.uv[1]			= 0.0f;

#endif

//=======================================================================================

	#ifndef __FACE3
#define __FACE3 1
	typedef struct FACE3
	{
		GLuint location_indices[3];
		GLuint normal_indices[3];
		GLuint uv_indices[3];
	}
	Face3;

#define INITFACE3( f )							\
	f.location_indices[0]	= 0;				\
	f.location_indices[1]	= 0;				\
	f.location_indices[2]	= 0;				\
	f.normal_indices[0]		= 0;				\
	f.normal_indices[1]		= 0;				\
	f.normal_indices[2]		= 0;				\
	f.uv_indices[0]			= 0;				\
	f.uv_indices[1]			= 0;				\
	f.uv_indices[2]			= 0;

#endif

//=======================================================================================
