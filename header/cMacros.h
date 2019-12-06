//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================
#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES	1
#endif

#include <math.h>

//=======================================================================================

#ifndef DEG2RAD
	#define DEG2RAD	0.01745329251994329576f
#endif

//=======================================================================================

#ifndef RAD2DEG
	#define RAD2DEG	57.29577951308232087679f
#endif

//=======================================================================================

// For cScreenText.cpp
#ifndef SCREEN_TEXT_BUFFER_SIZE
	#define SCREEN_TEXT_BUFFER_SIZE	1024
#endif

//=======================================================================================

#ifndef FREE_TEXTURE
	#define FREE_TEXTURE( ptr )				\
	if( ptr )								\
	{										\
		glDeleteTextures( 1, &ptr );		\
		ptr = 0;							\
	}
#endif

//=======================================================================================

#ifndef FREE_INSTANCE
	#define FREE_INSTANCE( ptr )			\
	if( ptr )								\
	{										\
		delete ptr;							\
		ptr = 0;							\
	}
#endif

//=======================================================================================

#ifndef BYTE2KB
	#define BYTE2KB( b ) b / 1024
#endif

//=======================================================================================

#ifndef BYTE2MB
	#define BYTE2MB( b ) b / 1048576
#endif

//=======================================================================================

#ifndef BYTE2GB
	#define BYTE2GB( b ) b / 1073741824
#endif

//=======================================================================================


// For cXmlParser.cpp:
#ifndef TIXML_USE_STL
	#define TIXML_USE_STL 1
#endif

#define NUM_INDENTS_PER_SPACE 2;

//=======================================================================================

// For cGlslManager.cpp:
#ifndef SHADER_OBJECT_MAX_VARIABLES
    #define SHADER_OBJECT_MAX_VARIABLES 64
#endif

#ifndef USE_GEOMETRY_SHADERS
    #define USE_GEOMETRY_SHADERS		1
#endif

#ifndef USE_GI_MODELS
    #define USE_GI_MODELS				0
#endif

#ifndef USE_INSTANCING
    #define USE_INSTANCING				1
#endif

#ifndef STRING_UTILS_BUFFER_SIZE
    #define STRING_UTILS_BUFFER_SIZE	65536
#endif

//=======================================================================================

#ifndef __XYZ
#define __XYZ 1
typedef struct {
   double x,y,z;
} XYZ;
#endif

#ifndef __PIXELA
#define __PIXELA 1
typedef struct {
   unsigned char r,g,b,a;
} PIXELA;
#endif

#ifndef __COLOUR
#define __COLOUR 1
typedef struct {
   double r,g,b;
} COLOUR;
#endif

#define CROSSPROD( p1, p2, p3 ) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

#ifndef __NORMALIZE
#define __NORMALIZE 1
#define NORMALIZE(p,length) \
   length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z); \
   if( length != 0 ) { \
      p.x /= length; \
      p.y /= length; \
      p.z /= length; \
   } else { \
      p.x = 0; \
      p.y = 0; \
      p.z = 0; \
   }
#endif

#ifndef LOD_STRUCT
#define LOD_STRUCT

struct sVBOLod
{
	unsigned int id;
	unsigned int primitivesWritten;
	unsigned int primitivesGenerated;
};

#endif

#ifndef FREE_MEMORY
#define FREE_MEMORY(ptr)	\
    if (ptr) {				\
		delete ptr;			\
        ptr=0;			\
    }
#endif

#ifndef FREE_ARRAY
#define FREE_ARRAY(ptr)	\
    if (ptr) {				\
		delete [] ptr;			\
        ptr=0;			\
    }
#endif

#ifndef FREE_VBO
#define FREE_VBO(ptr) \
	if (ptr) { \
		glDeleteBuffers(1,&ptr); \
		ptr = 0; \
	}
#endif

#ifndef FREE_TEXTURE
#define FREE_TEXTURE(ptr) \
	if (ptr) { \
		glDeleteTextures(1,&ptr); \
		ptr = 0; \
	}
#endif

#ifndef FREE_OGL_LIST
#define FREE_OGL_LIST(ptr) \
	if (ptr) { \
		glDeleteLists(ptr,1); \
		ptr = 0; \
	}
#endif

//=======================================================================================

// For cVboManager.cpp:
#ifndef MAX_INSTANCES
	#define MAX_INSTANCES 20000
#endif

//=======================================================================================

#define PORT 7000

//=======================================================================================
// Sim parameters


