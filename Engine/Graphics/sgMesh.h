//
//	sgMesh.h
//	iSDGE
//
//	Created by Nils Daumann on 17.04.10.
//	Copyright (c) 2010 Nils Daumann

//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:

//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.

//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#ifndef __SGMESH_H__
#define __SGMESH_H__

#include "sgBase.h"
#include "sgVector4.h"
#include "sgVertex.h"

class sgVertex;
class sgVector3;

/**
 * Mesh class. This is the mesh format.
 */
class sgMesh : public sgBase
{
	public:
		/**
		 *	Constructor.
		 *	Initializes the instance.
		 */
		sgMesh();
		
		/**
		 *	Deconstructor.
		 *	Frees all memory allocated by this class.
		 */
		~sgMesh();
	
		/**
		 *	Generate VBO.
		 *	Generates a vertex buffer object of this mesh.
		 */
		void generateVBO(bool dyn = false);
	
		/**
		 *	Update VBO.
		 *	Updates the VBO to the current mesh, if it is dynamic and toggles the active VBO.
		 */
		void updateVBO();
	
		/**
		 *	Calculate normals.
		 *	Calculates new normals for the mesh, which is needed for correct shading after the mesh was manipulated.
		 */
		void calculateNormals();
	
		/**
		 *	Calculate tangents.
		 *	Calculates new tangents for the mesh, which is needed for correct rendering after the mesh was manipulated.
		 */
		void calculateTangents();
		
		/**
		 *	Calculate face normal.
		 *	Calculates the normal of the layer, streched between the three given vertices.
		 * @param vertex a first vertex
		 * @param neighbour_a a second vertex
		 * @param neighbour_b a third vertex
		 */
		void calculateFaceNormal(sgVertex *vertex, sgVertex *neighbour_a, sgVertex *neighbour_b);
	
		/**
		 *	Calculate face tangent.
		 *	Calculates the tangent of the layer, streched between the three given vertices.
		 * @param vertex a first vertex
		 * @param neighbour_a a second vertex
		 * @param neighbour_b a third vertex
		 */
		void calculateFaceTangent(sgVertex *vertex, sgVertex *neighbour_a, sgVertex *neighbour_b, sgVector3 *tangent0, sgVector3 *tangent1, sgVector3 *tangent2, sgVector3 *bitangent0, sgVector3 *bitangent1, sgVector3 *bitangent2);
	
		/**
		 *	Invert texcoords X.
		 *	Inverts the texture coordinates along the x axis.
		 */
		void invertTexCoordsX();
	
		/**
		 *	Invert texcoords Y.
		 *	Inverts the texture coordinates along the y axis.
		 */
		void invertTexCoordsY();
	
		/**
		 *	Calculate cull sphere.
		 *	Calculates the cull sphere for this mesh.
		 */
		void calcCullSphere();
	
		void recreate(const char *filename);
	
		/**
		 *	Bone number.
		 *	The number of this meshs bones.
		 */
		unsigned int bonenum;
	
		/**
		 *	Bone indices.
		 *	Maps the indices within the vertex data to the real bone indices to allow more than 30 bones per object, by restricting it to 30 bones per mesh.
		 */
		unsigned int *boneindices;
	
		/**
		 *	Bone matrices.
		 *	The matrices used to transform this meshs bones.
		 */
		float *bonematrices;
	
		/**
		 *	Vertex size.
		 *	The size of a vertex of this mesh.
		 */
		unsigned char vtxsize;
	
		/**
		 *	Vertex features.
		 *	Bitmask of the feautures of the interleaved vertex data.
		 */
		unsigned long vtxfeatures;
	
		/**
		 *	Vertex number.
		 *	The number of vertices, this mesh consists of.
		 */
		unsigned int vertexnum;
	
		/**
		 *	Vertex array.
		 *	Pointer to the meshs array of vertices.
		 */
		float *vertices;
	
		/**
		 *	Index number.
		 *	The number of the meshs indices.
		 */
		unsigned int indexnum;
	
		/**
		 *	Index size.
		 *	The size of each mesh index in bytes.
		 */
		unsigned int indexsize;
	
		/**
		 *	Index array.
		 *	Pointer to the meshs array of indices.
		 */
		void *indices;
	
		/**
		 *	VBO id.
		 *	The id of the meshs current vertex buffer object.
		 */
		unsigned int vbo;
	
		/**
		 *	Index VBO id.
		 *	The id of the meshs current vertex buffer object handling the indces.
		 */
		unsigned int ivbo;
	
		/**
		 *	Dynamic VBO.
		 *	True, if the mesh is meant to be dynamically changed.
		 */
		bool dynamic;
	
		/**
		 *	Culled.
		 *	True, if the mesh wasn´t rendered the previous frame.
		 */
		bool culled;
	
		/**
		 *	Cull sphere.
		 *	Sphere checked against the view frustum for culling. XYZ is the local center and W the radius.
		 */
		sgVector4 cullsphere;
	
	private:
		bool vbotoggle;
		unsigned int vbo0;
		unsigned int vbo1;
		unsigned int ivbo0;
		unsigned int ivbo1;
};

#endif
