
#ifndef __MISC_H__
#define __MISC_H__

#ifndef PI
	#define PI 3.14159265358979323846
#endif PI

#ifndef RAND_2to2
	#define RAND_2to2 (rand()*1.0f/(RAND_MAX-1.0f)) *4.0 - 2.0f
#endif RAND_2to2


#ifndef RAND_1to1
	#define RAND_1to1 (rand()*1.0f/(RAND_MAX-1.0f)) *2.0 - 1.0f
#endif RAND_1to1

#ifndef RAND0to1
	#define RAND0to1 (rand()*1.0f/(RAND_MAX-1.0f))
#endif RAND0to1

#ifndef RAND_5to5
	#define RAND_5to5 (rand()*1.0f/(RAND_MAX-1.0f)) * 10.0f - 5.0f
#endif RAND_5to5

#ifndef RAND0to1
	#define RAND0to1 rand()/(RAND_MAX-1.0f)+0.1
#endif RAND0to1


#ifndef FREE_MEMORY
#define FREE_MEMORY(ptr)	\
    if (ptr) {				\
		delete ptr;			\
        ptr=0;			\
    }
#endif FREE_MEMORY

#ifndef FREE_ARRAY
#define FREE_ARRAY(ptr)	\
    if (ptr) {				\
		delete [] ptr;			\
        ptr=0;			\
    }
#endif FREE_MEMORY

#ifndef FREE_VBO
#define FREE_VBO(ptr) \
	if (ptr) { \
		glDeleteBuffers(1,&ptr); \
		ptr = 0; \
	}
#endif FREE_VBO

#ifndef FREE_TEXTURE
#define FREE_TEXTURE(ptr) \
	if (ptr) { \
		glDeleteTextures(1,&ptr); \
		ptr = 0; \
	}
#endif FREE_TEXTURE

#ifndef FREE_OGL_LIST
#define FREE_OGL_LIST(ptr) \
	if (ptr) { \
		glDeleteLists(ptr,1); \
		ptr = 0; \
	}
#endif FREE_OGL_LIST

struct sVBOLod
{
	unsigned int id;
	unsigned int primitivesWritten;
};

#endif __MISC_H__