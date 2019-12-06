//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================
#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
#include <vector>

#include "Vertex.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
/*
#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"			// Output data structure
#include "assimp/postprocess.h"		// Post processing flags
*/

using namespace std;

class cModel
{
public:
						cModel							( string& Filename, float  	scale );
						cModel							( string& Filename,
															float	scaleX, float scaleY, float scaleZ );
						~cModel							( void );
	void				fillVBO							(	);
	void				draw							( 	);
	void 				draw 							( unsigned int target, unsigned int texId			);
	void				drawInstanced					( unsigned int numInstances, 
															unsigned int target, unsigned int posId 		);

	void 				drawInstanced					( unsigned int numInstances,
															unsigned int target, unsigned int *simTex		);



	unsigned int		getVBO							( ) { return vbo; }
	unsigned long long	getVBOAddres					( ) { return vboAddres; }


private:

	void	assimp_recursive_gather_data				(	const struct aiScene*	sc,
																const struct aiNode*	nd );

	float					scale;
	float					scaleX, scaleY, scaleZ;
	bool					scaleByVector;
	unsigned int			vbo;
	unsigned int			size;
	GLuint64EXT				vboAddres;
	const struct aiScene*	scene;
	//Assimp::Importer		importer;
	vector<Vertex>			vertices;



};


#endif
