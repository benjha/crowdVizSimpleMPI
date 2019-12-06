
//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#include <iostream>
#include <GL/glew.h>

#include "cMacros.h"
#include "cModel.h"
#include "Vertex.h"

unsigned int ppsteps =
		aiProcess_GenSmoothNormals					|		// generates smooth normals for all vertices in the mesh.
		//aiProcess_CalcTangentSpace					|	// calculate tangents and bitangents if possible
		aiProcess_Triangulate						|	// use only triangles
		//aiProcess_SortByPType						|	// use only triangles
		//aiProcess_JoinIdenticalVertices				|	// join identical vertices / optimize indexing
		//aiProcess_ValidateDataStructure				|	// perform a full validation of the loader's output
		//aiProcess_ImproveCacheLocality				| 	// improve the cache locality of the output vertices
		//aiProcess_FixInfacingNormals				|	// This step tries to determine which meshes have normal vectors that are facing inwards.
		//aiProcess_RemoveRedundantMaterials			|	// remove redundant materials
		//aiProcess_FindDegenerates					|	// remove degenerated polygons from the import
		//aiProcess_FindInvalidData					|	// detect invalid model data, such as invalid normal vectors
		//aiProcess_GenUVCoords						|	// convert spherical, cylindrical, box and planar mapping to proper UVs
		//aiProcess_TransformUVCoords					|	// preprocess UV transformations (scaling, translation ...)
		//aiProcess_FindInstances						|	// search for instanced meshes and remove them by references to one master
		//aiProcess_LimitBoneWeights					|	// limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes					|	// join small meshes, if possible;
		//aiProcess_FlipWindingOrder					|
		0;

cModel::cModel ( std::string &filename, float scale )
{
	cModel::scale			= scale;
	cModel::scaleX			= 0.0;
	cModel::scaleY			= 0.0;
	cModel::scaleZ			= 0.0;
	cModel::scaleByVector	= false;

	cModel::vbo				= 0;
	cModel::vboAddres		= 0;
	cModel::size			= 0;
	// generare VBO's

	// Let ASSIMP work it's magic to load an OBJ:
	//scene = importer.ReadFile( filename, ppsteps );

	scene = aiImportFile( filename.data(), ppsteps );

	if (!scene)
		cout << filename << "not found! \n";

	// This one is custom made. Refer to ASSIMP's documentation.
	assimp_recursive_gather_data( scene, scene->mRootNode );

	fillVBO	();
	vertices.erase (vertices.begin(), vertices.end());

}
//
//=======================================================================================
//
cModel::cModel ( std::string &filename, float scaleX, float scaleY, float scaleZ )
{
	cModel::scale			= 0.0f;
	cModel::scaleX			= scaleX;
	cModel::scaleY			= scaleY;
	cModel::scaleZ			= scaleZ;
	cModel::scaleByVector	= true;

	cModel::vbo				= 0;
	cModel::vboAddres		= 0;
	cModel::size			= 0;
	// generare VBO's

	// Let ASSIMP work it's magic to load an OBJ:
	//scene = importer.ReadFile( filename, ppsteps );

	scene = aiImportFile( filename.data(), ppsteps );

	if (!scene)
		cout << filename << "not found! \n";

	// This one is custom made. Refer to ASSIMP's documentation.
	assimp_recursive_gather_data( scene, scene->mRootNode );

	fillVBO	();
	vertices.erase (vertices.begin(), vertices.end());
}
//
//=======================================================================================
//
cModel::~cModel	( void )
{
	FREE_VBO ( vbo );
}
//
//=======================================================================================
//
void cModel::assimp_recursive_gather_data(	const struct aiScene* sc, 
											const struct aiNode*  nd
										)
{
	for( unsigned int n = 0; n < nd->mNumMeshes; ++n )
	{
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		for( unsigned int f = 0; f < mesh->mNumFaces; ++f )
		{
			const struct aiFace* face = &mesh->mFaces[f];
			for( unsigned int i = 0; i < face->mNumIndices; i++ )
			{
				unsigned int index = face->mIndices[i];
				Vertex vert;
				INITVERTEX( vert );
				if( mesh->HasTextureCoords( 0 ) )
				{
					vert.texture[0] = mesh->mTextureCoords[0][index].x;
					vert.texture[1] = mesh->mTextureCoords[0][index].y;
				}
				else
				{
					vert.texture[0] = 0.0f;
					vert.texture[1] = 0.0f;
				}
				if( mesh->HasTangentsAndBitangents() )
				{
					vert.tangent[0] = mesh->mTangents[index].x;
					vert.tangent[1] = mesh->mTangents[index].y;
					vert.tangent[2] = mesh->mTangents[index].z;
				}
				if( mesh->mNormals != NULL )
				{
					vert.normal[0] = mesh->mNormals[index].x;
					vert.normal[1] = mesh->mNormals[index].y;
					vert.normal[2] = mesh->mNormals[index].z;
				}
				else
				{
					vert.normal[0] = 0.0f;
					vert.normal[1] = 0.0f;
					vert.normal[2] = 0.0f;
				}
				if( mesh->HasVertexColors( 0 ) )
				{
					vert.color[0] = mesh->mColors[0][index].r;
					vert.color[1] = mesh->mColors[0][index].g;
					vert.color[2] = mesh->mColors[0][index].b;
					vert.color[3] = mesh->mColors[0][index].a;
				}
				else
				{
					vert.color[0] = 1.0f;
					vert.color[1] = 1.0f;
					vert.color[2] = 1.0f;
					vert.color[3] = 1.0f;
				}



				struct aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];

				aiString texPath;
				if( AI_SUCCESS == mtl->GetTexture( aiTextureType_DIFFUSE, 0, &texPath ) )
				{

				}

				aiColor4D diffuse;
				if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
				{
					vert.color[0] = diffuse.r;
					vert.color[1] = diffuse.g;
					vert.color[2] = diffuse.b;
					vert.color[3] = diffuse.a;
				}
				else
				{
					vert.color[0] = 0.0f;
					vert.color[1] = 0.0f;
					vert.color[2] = 0.0f;
					vert.color[3] = 1.0f;
				}

				aiColor4D ambient;
				if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
				{

				}
				else
				{

				}
		 
				aiColor4D specular;
				if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
				{

				}
				else
				{

				}
		 
				aiColor4D emission;
				if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
				{

				}
				else
				{

				}
		 
				float shininess = 0.0;
				unsigned int max;
				aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
				{

				}

				if (scaleByVector )
				{
					vert.location[0] = mesh->mVertices[index].x * scaleX;
					vert.location[1] = mesh->mVertices[index].y * scaleY;
					vert.location[2] = mesh->mVertices[index].z * scaleZ;
				}
				else
				{
					vert.location[0] = mesh->mVertices[index].x * scale;
					vert.location[1] = mesh->mVertices[index].y * scale;
					vert.location[2] = mesh->mVertices[index].z * scale;
				}
					vert.location[3] = 1.0f;
				vertices.push_back( vert );
			}
		}
	}
	for( unsigned int n = 0; n < nd->mNumChildren; ++n )
	{
		assimp_recursive_gather_data( sc, nd->mChildren[n] );
	}
}
//
//=======================================================================================
//
void cModel::fillVBO( )
{
	size = vertices.size ();
	if (!vbo)
	{
		glGenBuffers						( 1, &vbo );
		glNamedBufferDataEXT				( vbo, sizeof ( Vertex ) * size, &vertices[0], GL_STATIC_DRAW );
		glGetNamedBufferParameterui64vNV 	( vbo, GL_BUFFER_GPU_ADDRESS_NV, &vboAddres );
		glMakeNamedBufferResidentNV			( vbo, GL_READ_ONLY );
		glEnableClientState					( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV ); // Can this line affect other ways of displaying vertices ?
	}
	
}
//
//=======================================================================================
//
void cModel::draw ()
{

	glEnableVertexAttribArray	( 0 ); // from the shader: layout ( location = 0 ) position
	glEnableVertexAttribArray	( 1 ); // from the shader: layout ( location = 1 ) normals
	glEnableVertexAttribArray	( 2 ); // from the shader: layout ( location = 2 ) uvs

	glVertexAttribFormatNV		( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying vertexPosition's format
	glVertexAttribFormatNV		( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying normals's format
	glVertexAttribFormatNV		( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying uv's format


	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, vboAddres, sizeof ( Vertex ) * size ); 
	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 1, vboAddres + (GLsizei)NORMAL_OFFSET, sizeof ( Vertex ) * size - (GLsizei)NORMAL_OFFSET   );
	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 2, vboAddres + (GLsizei)TEXTURE_OFFSET, sizeof ( Vertex ) * size - (GLsizei)TEXTURE_OFFSET  );

	glDrawArrays				( GL_TRIANGLES, 0, size ); // watch out your mesh is made of triangles

	glDisableVertexAttribArray	( 0 );
	glDisableVertexAttribArray	( 1 );
	glDisableVertexAttribArray	( 2 );

}
//
//=======================================================================================
//
void cModel::draw (unsigned int target, unsigned int texId)
{

	glEnableVertexAttribArray	( 0 ); // from the shader: layout ( location = 0 ) position
	glEnableVertexAttribArray	( 1 ); // from the shader: layout ( location = 1 ) normals
	glEnableVertexAttribArray	( 2 ); // from the shader: layout ( location = 2 ) uvs

	glVertexAttribFormatNV		( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying vertexPosition's format
	glVertexAttribFormatNV		( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying normals's format
	glVertexAttribFormatNV		( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying uv's format

	glActiveTexture				( GL_TEXTURE0 		); 		// from shader
	glBindTexture				( target,	texId	);

	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, vboAddres, sizeof ( Vertex ) * size );
	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 1, vboAddres + (GLsizei)NORMAL_OFFSET, sizeof ( Vertex ) * size - (GLsizei)NORMAL_OFFSET   );
	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 2, vboAddres + (GLsizei)TEXTURE_OFFSET, sizeof ( Vertex ) * size - (GLsizei)TEXTURE_OFFSET  );

	glDrawArrays				( GL_TRIANGLES, 0, size ); // watch out your mesh is made of triangles

	glBindTexture				( target,	0 );

	glDisableVertexAttribArray	( 0 );
	glDisableVertexAttribArray	( 1 );
	glDisableVertexAttribArray	( 2 );

}
//
//=======================================================================================
//
void cModel::drawInstanced(unsigned int numInstances, unsigned int target, unsigned int *simTex)
{

	glEnableVertexAttribArray	( 0 ); // from the shader: layout ( location = 0 ) position
	glEnableVertexAttribArray	( 1 ); // from the shader: layout ( location = 1 ) normals

	glVertexAttribFormatNV		( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying vertexPosition's format
	glVertexAttribFormatNV		( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying normals's format


	//using OpenGL DirectAcces for textures
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 1, GL_TEXTURE_2D_ARRAY, simTex[0]); // from shader: layout (binding = 1 ) uniform sampler2DArray pos;
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 2, GL_TEXTURE_2D_ARRAY, simTex[1]); // from shader: layout ( binding = 2 ) uniform sampler2DArray vel;
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 3, GL_TEXTURE_2D_ARRAY, simTex[2]); // from shader: layout ( binding = 2 ) uniform sampler2DArray dir;
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 4, GL_TEXTURE_2D_ARRAY, simTex[3]); // from shader: layout ( binding = 2 ) uniform sampler2DArray ids;

	/*
	 *
	glActiveTexture				( GL_TEXTURE0 + 1 					);	// from shader: layout (binding = 1 ) uniform sampler2DArray pos;
	glBindTexture				( GL_TEXTURE_2D_ARRAY,	simTex[0]	);

	glActiveTexture				( GL_TEXTURE0 + 2 					); // from shader: layout ( binding = 2 ) uniform sampler2DArray vel;
	glBindTexture				( GL_TEXTURE_2D_ARRAY,  simTex[1]	);

	glActiveTexture				( GL_TEXTURE0 + 3 					); // from shader: layout ( binding = 2 ) uniform sampler2DArray dir;
	glBindTexture				( GL_TEXTURE_2D_ARRAY,  simTex[2]	);

	glActiveTexture				( GL_TEXTURE0 + 4 					); // from shader: layout ( binding = 2 ) uniform sampler2DArray ids;
	glBindTexture				( GL_TEXTURE_2D_ARRAY,  simTex[3]	);

*/

	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, vboAddres, sizeof ( Vertex ) * size ); 
	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 1, 
									vboAddres + (GLsizei)NORMAL_OFFSET, sizeof ( Vertex ) * size - (GLsizei)NORMAL_OFFSET );
	
	glDrawArraysInstanced		( GL_TRIANGLES, 0, size, numInstances ); // watch out your mesh is made of triangles

	glBindMultiTextureEXT		(  GL_TEXTURE0 + 1, GL_TEXTURE_2D_ARRAY, 0 );
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 2, GL_TEXTURE_2D_ARRAY, 0 );
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 3, GL_TEXTURE_2D_ARRAY, 0 );
	glBindMultiTextureEXT		(  GL_TEXTURE0 + 4, GL_TEXTURE_2D_ARRAY, 0 );


/*
	glBindTexture				( GL_TEXTURE_2D_ARRAY, 0 );

	glActiveTexture				( GL_TEXTURE0 + 3 					);
	glBindTexture				( GL_TEXTURE_2D_ARRAY, 0 );

	glActiveTexture				( GL_TEXTURE0 + 2 					);
	glBindTexture				( GL_TEXTURE_2D_ARRAY, 0 );

	glActiveTexture				( GL_TEXTURE0 + 1 					);
	glBindTexture				( GL_TEXTURE_2D_ARRAY, 0 );
*/

	glDisableVertexAttribArray	( 0 );
	glDisableVertexAttribArray	( 1 );


}
//
//=======================================================================================
//
void cModel::drawInstanced(unsigned int numInstances, unsigned int target, unsigned int posTexId)
{


	glEnableVertexAttribArray	( 0 ); // from the shader: layout ( location = 0 ) position
	glEnableVertexAttribArray	( 1 ); // from the shader: layout ( location = 1 ) normals
	glEnableVertexAttribArray	( 2 ); // from the shader: layout ( location = 2 ) uv;

	glVertexAttribFormatNV		( 0, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying vertexPosition's format
	glVertexAttribFormatNV		( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying normals's format
	glVertexAttribFormatNV		( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ) ); // specifying uv's format

	glActiveTexture				( GL_TEXTURE0 + 1 					); // from shader: layout (binding = 1 ) uniform sampler2DArray pos;
	glBindTexture				( GL_TEXTURE_BUFFER,	posTexId	);


	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, vboAddres, sizeof ( Vertex ) * size );
	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 1,
									vboAddres + (GLsizei)NORMAL_OFFSET, sizeof ( Vertex ) * size - (GLsizei)NORMAL_OFFSET );

	glBufferAddressRangeNV		( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 2,
										vboAddres + (GLsizei)TEXTURE_OFFSET, sizeof ( Vertex ) * size - (GLsizei)TEXTURE_OFFSET);

	glDrawArraysInstanced		( GL_TRIANGLES, 0, size, numInstances ); // watch out your mesh is made of triangles

	glBindTexture				( GL_TEXTURE_BUFFER, 0 );

	glDisableVertexAttribArray	( 0 );
	glDisableVertexAttribArray	( 1 );
	glDisableVertexAttribArray	( 2 );
}
