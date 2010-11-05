//
//	sgShadowVolume.h
//	iSDGE
//
//	Created by Nils Daumann on 11.08.10.
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

#include "sgVector3.h"

class sgMesh;
class sgObject;
class sgLight;
class sgShadowVolume;
class sgVertex;

/**
 * Shadow volume edge class. Represents an edge as it is needed to generate the shadow volume.
 */
class sgShadowEdge
{
	public:
		bool operator== (const sgShadowEdge &other);
		
		sgVertex *verts;
		unsigned int vert1;
		unsigned int vert2;
		unsigned int counter1;
		unsigned int counter2;
		bool lastaddor;
};

/**
 * Shadow volume face class. Represents a face as it is needed to generate the shadow volume.
 */
class sgShadowFace
{
	public:
		unsigned int edge1;
		bool flipped1;
		unsigned int edge2;
		bool flipped2;
		unsigned int edge3;
		bool flipped3;
		sgVector3 normal;
};

/**
 * Shadow volume class. Generates and represents a shadow volume needed for stencil shadows.
 */
class sgShadowVolume
{
	public:
		/**
		 *	Constructor.
		 *	Initializes the shadow volume.
		 *  @param obj pointer to the object, this volume is attached to.
		 */
		sgShadowVolume(sgObject *obj);
	
		/**
		 *	Deconstructor.
		 *	Frees the shadow volume.
		 */
		~sgShadowVolume();
	
		/**
		 *	Update shadow volume.
		 *	Updates the shadow volumes mesh.
		 *  @param firstlight pointer to the first light of the lightlist to generate the volume for.
		 */
		void update(sgLight *firstlight);
	
		/**
		 *	Mesh.
		 *	The mesh of the shadows volume. Generated automatically.
		 */
		sgMesh *mesh;
	
		/**
		 *	Object vertices.
		 *	All vertices of the object. Better don´t touch.
		 */
		sgVertex *vertices;
	
		/**
		 * Face count.
		 * Number of faces.
		 **/
		unsigned int facenum;
		
		/**
		 * Edge count.
		 * Number of edges.
		 **/
		unsigned int edgenum;
	
	private:
		void calculateFaceNormal(sgShadowFace *face, sgVertex *vertex, sgVertex *neighbour_a, sgVertex *neighbour_b);
		sgObject *object;
		sgShadowEdge *edges;
		sgShadowEdge *tempedges;
		sgShadowFace *faces;
};
