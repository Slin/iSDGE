#################################
#Addon header
#################################
bl_addon_info = {
	'name': 'Export: iSDGE model format',
	'author': 'Nils Daumann',
	'version': '0 2011/03/19',
	'blender': (2, 5, 3),
	'category': 'Import-Export',
	'location': 'File > Export'}


#################################
#Includes
#################################
import os
import bpy
import struct
import math


#Container classes for a better overview
class c_vertex(object):
	__slots__ = 'blendindex', 'position', 'uvs', 'color', 'normal', 'tangent'
	def __init__(self, blendindex, position = (0, 0, 0), uvs = [], color = None, normal = (0, 0, 0), tangent = (0, 0, 0, 0)):
		self.blendindex = blendindex	#index within the blender mesh
		self.position = position
		self.uvs = uvs
		self.color = color
		self.normal = normal
		self.tangent = tangent

class c_triangle(object):
	__slots__ = 'vertices', 'images', 'newindices'
	def __init__(self, vertices = [], images = [], newindices = None):
		self.vertices = vertices
		self.images = images
		self.newindices = newindices		#indices within the c_mesh
		
class c_mesh(object):
	__slots__ = 'triangles', 'images', 'vertices', 'indices'
	def __init__(self, images = [], tri = None):
		self.vertices = []
		self.triangles = []
		if tri != None:
			self.triangles.append(tri)
		self.images = images
		self.indices = []
	
	#doublicates face vertices with different uv coords and sets the new index
	def uvsplit(self):
		for tri in self.triangles:
			ind = []
			for trivert in tri.vertices:
				check = 0
				for i, vert in enumerate(self.vertices):
					if vert == trivert:
						ind.append(i)
						check = 1
						break
				if check == 0:
					ind.append(len(self.vertices))
					self.vertices.append(trivert)
			
			self.indices.append(ind[0])
			self.indices.append(ind[1])
			self.indices.append(ind[2])
	
	#generates tangent for the given face
	def genfacetangent(self, vertex, neighbour_a, neighbour_b, bitangent0, bitangent1, bitangent2):
		posdir1 = (neighbour_a.position[0]-vertex.position[0], neighbour_a.position[1]-vertex.position[1], neighbour_a.position[2]-vertex.position[2])
		posdir2 = (neighbour_b.position[0]-vertex.position[0], neighbour_b.position[1]-vertex.position[1], neighbour_b.position[2]-vertex.position[2])
		uvdir1 = (neighbour_a.uvs[0][0]-vertex.uvs[0][0], neighbour_a.uvs[0][1]-vertex.uvs[0][1])
		uvdir2 = (neighbour_b.uvs[0][0]-vertex.uvs[0][0], neighbour_b.uvs[0][1]-vertex.uvs[0][1])
		
		r = (uvdir1[0]*uvdir2[1]-uvdir2[0]*uvdir1[1])
		if r != 0:
			r = 1.0/r
		else:
			r = 1.0
		tangent = ((uvdir2[1]*posdir1[0]-uvdir1[1]*posdir2[0])*r, (uvdir2[1]*posdir1[1]-uvdir1[1]*posdir2[1])*r, (uvdir2[1]*posdir1[2]-uvdir1[1]*posdir2[2])*r, 0.0)
		bitangent = ((uvdir1[0]*posdir2[0]-uvdir2[0]*posdir1[0])*r, (uvdir1[0]*posdir2[1]-uvdir2[0]*posdir1[1])*r, (uvdir1[0]*posdir2[2]-uvdir2[0]*posdir1[2])*r)
		
		vertex.tangent = (vertex.tangent[0]+tangent[0], vertex.tangent[1]+tangent[1], vertex.tangent[2]+tangent[2], 0.0)
		neighbour_a.tangent = (neighbour_a.tangent[0]+tangent[0], neighbour_a.tangent[1]+tangent[1], neighbour_a.tangent[2]+tangent[2], 0.0)
		neighbour_b.tangent = (neighbour_b.tangent[0]+tangent[0], neighbour_b.tangent[1]+tangent[1], neighbour_b.tangent[2]+tangent[2], 0.0)
		
		bitangent0 = (bitangent0[0]+bitangent[0], bitangent0[1]+bitangent[1], bitangent0[2]+bitangent[2])
		bitangent1 = (bitangent1[0]+bitangent[0], bitangent1[1]+bitangent[1], bitangent1[2]+bitangent[2])
		bitangent2 = (bitangent2[0]+bitangent[0], bitangent2[1]+bitangent[1], bitangent2[2]+bitangent[2])
		return bitangent0, bitangent1, bitangent2
	
	#generates tangents
	def gentangents(self):
		i = 0
		bitangents = []
		while i < len(self.vertices):
			bitangents.append((0, 0, 0))
			i += 1
		
		#Calculate the tangent of each vertex
		i = 0
		while i < len(self.indices):
			bitangents[self.indices[i]], bitangents[self.indices[i+2]], bitangents[self.indices[i+1]] = self.genfacetangent(self.vertices[self.indices[i]], self.vertices[self.indices[i+2]], self.vertices[self.indices[i+1]], bitangents[self.indices[i]], bitangents[self.indices[i+2]], bitangents[self.indices[i+1]])
			i += 3
		
		#Normalize all tangents
		i = 0
		while i < len(self.vertices):
			temptangent = (self.vertices[i].tangent[0]-self.vertices[i].normal[0]*self.vertices[i].normal[0]*self.vertices[i].tangent[0], self.vertices[i].tangent[1]-self.vertices[i].normal[1]*self.vertices[i].normal[1]*self.vertices[i].tangent[1], self.vertices[i].tangent[2]-self.vertices[i].normal[2]*self.vertices[i].normal[2]*self.vertices[i].tangent[2], 0.0)
			l = temptangent[0]*temptangent[0]+temptangent[1]*temptangent[1]+temptangent[2]*temptangent[2]
			l = math.sqrt(l)
			if l == 0: l = 1
			temptangent = (temptangent[0]/l, temptangent[1]/l, temptangent[2]/l, 0.0)
			self.vertices[i].tangent = temptangent
			i += 1
			
		#Calculate bitangent direction
		i = 0
		while i < len(self.vertices):
			temptangent = self.vertices[i].tangent
			bicross = (self.vertices[i].normal[1]*temptangent[2]-self.vertices[i].normal[2]*temptangent[1], self.vertices[i].normal[2]*temptangent[0]-self.vertices[i].normal[0]*temptangent[2], self.vertices[i].normal[0]*temptangent[1]-self.vertices[i].normal[1]*temptangent[0])
			bidot = bicross[0]*bitangents[i][0]+bicross[1]*bitangents[i][1]+bicross[2]*bitangents[i][2]
			if bidot < 0.0:
				bidot = -1.0
			else:
				bidot = 1.0
			self.vertices[i].tangent = (temptangent[0], temptangent[1], temptangent[2], bidot)
			i += 1


class c_object(object):
	__slots__ = 'meshs'
	#splits the blender object into triangle meshs with the same textures
	def __init__(self, obj, exptangents, expshadow):
		triangles = []
		#generate vertices and triangles
		for i, face in enumerate(obj.faces):
			images = []
			for tex in obj.uv_textures:
				imgpath = tex.data[i].image.filepath
				img = imgpath.split('/')
				images.append((tex.data[i].image.name, img[len(img)-1]))
			
			verts = []
			for n, vertind in enumerate(face.vertices):
				uvs = []
				for tex in obj.uv_textures:
					uvs.append((round(tex.data[i].uv[n][0], 6), 1.0-round(tex.data[i].uv[n][1], 6)))
				color = None
				if len(obj.vertex_colors) > 0:
					alpha = 1.0
					if len(obj.vertex_colors) > 1:
						alpha = obj.vertex_colors[1].data[i].color1[0]
					if n == 0:
						color = (obj.vertex_colors[0].data[i].color1[0], obj.vertex_colors[0].data[i].color1[1], obj.vertex_colors[0].data[i].color1[2], alpha)
					if n == 1:
						color = (obj.vertex_colors[0].data[i].color2[0], obj.vertex_colors[0].data[i].color2[1], obj.vertex_colors[0].data[i].color2[2], alpha)
					if n == 2:
						color = (obj.vertex_colors[0].data[i].color3[0], obj.vertex_colors[0].data[i].color3[1], obj.vertex_colors[0].data[i].color3[2], alpha)
					if n == 3:
						color = (obj.vertex_colors[0].data[i].color4[0], obj.vertex_colors[0].data[i].color4[1], obj.vertex_colors[0].data[i].color4[2], alpha)
				
				position = (-obj.vertices[vertind].co.x, obj.vertices[vertind].co.z, obj.vertices[vertind].co.y)
				normal = (-obj.vertices[vertind].normal.x, obj.vertices[vertind].normal.z, obj.vertices[vertind].normal.y)
				verts.append(c_vertex(vertind, position, uvs, color, normal))
			
			if len(face.vertices) == 3:
				triangles.append(c_triangle(verts, images))
			else:
				tri1 = [verts[0], verts[1], verts[2]]
				tri2 = [verts[0], verts[2], verts[3]]
				triangles.append(c_triangle(tri1, images))
				triangles.append(c_triangle(tri2, images))
		
		#generate meshs
		self.meshs = []
		m = c_mesh(triangles[0].images)
		self.meshs.append(m)
		for tri in triangles:
			check = 0
			for mesh in self.meshs:
				if mesh.images == tri.images:
					mesh.triangles.append(tri)
					check = 1
					break
			if check == 0:
				m = c_mesh(tri.images, tri)
				self.meshs.append(m)
		
		for mesh in self.meshs:
			mesh.uvsplit()
		
		if exptangents == True:
			for mesh in self.meshs:
				mesh.gentangents()
	
	
	def write(self, filename, exptextures, exptangents, expshadow):
		print("open or create file")
		file = open(filename, 'wb')

		print("write file format version number: 0")
		file.write(struct.pack('<B', 0))
		
		print("write materials")
		file.write(struct.pack('<B', len(self.meshs)))  #number of materials
		for i, mesh in enumerate(self.meshs):
			file.write(struct.pack('<B', i))			#material id
			texcount = len(mesh.images)
			if exptextures != True:
				texcount = 0
			file.write(struct.pack('<B', texcount)) #number of textures
			if texcount > 0:
				for img in mesh.images:
					file.write(struct.pack('<H', len(img[1])+1))
					file.write(struct.pack('<%is'%(len(img[1])+1), img[1]))
		
		print("write meshs")
		file.write(struct.pack('<B', len(self.meshs)))
		for i, mesh in enumerate(self.meshs):
			datachannels = 0
			if mesh.vertices[0].color != None:
				datachannels = 4
				
			file.write(struct.pack('<B', i))	#mesh id
			file.write(struct.pack('<B', i))	#material id
			file.write(struct.pack('<H', len(mesh.vertices)))   #vertexnum
			file.write(struct.pack('<B', len(mesh.images))) #texcoord count
			file.write(struct.pack('<B', datachannels)) #texdata count
			
			if exptangents == True:
				file.write(struct.pack('<B', 1)) #has tangents
			else:
				file.write(struct.pack('<B', 0)) #does not have tangents
		
			print("write interleaved vertex data")
			
			for vertex in mesh.vertices:
				bindata = struct.pack('<fff', vertex.position[0], vertex.position[1], vertex.position[2])
				file.write(bindata)
				
				bindata = struct.pack('<fff', vertex.normal[0], vertex.normal[1], vertex.normal[2])
				file.write(bindata)

				set = 0
				while set < len(mesh.images):
					bindata = struct.pack('<ff', vertex.uvs[set][0], vertex.uvs[set][1])
					file.write(bindata)
					set += 1
				
				if mesh.vertices[0].color != None:
					bindata = struct.pack('<ffff', vertex.color[0], vertex.color[1], vertex.color[2], vertex.color[3])
					file.write(bindata)
				
				if exptangents == True:
					bindata = struct.pack('<ffff', vertex.tangent[0], vertex.tangent[1], vertex.tangent[2], vertex.tangent[3])
					file.write(bindata)
					
			print("finished writing interleaved vertex data")
			
			print("write indices")
			file.write(struct.pack('<H', len(mesh.indices)))
			for ind in mesh.indices:
				bindata = struct.pack('<H', ind)
				file.write(bindata)
	
		file.close()
		print("finished export")


#################################
#Writing the file
#################################
#def exportobject(filename, context):
	
#   bpy.ops.object.mode_set(mode='OBJECT')
#   obj = c_object(context.object.data)
#   obj.write(filename)
	
	
#   for i, modi in enumerate(context.object.modifiers):
#   if modi.type == 'ARMATURE':
#   print("write armature")
			
#   modi.object.data.pose_position = 'REST'
#   file.write("\t<armature name=\"%s\">\n" % modi.object.data.name)
#   for i, bone in enumerate(modi.object.data.bones):
#	 for parnum, parent in enumerate(modi.object.data.bones):
#		 if parent == bone.parent:
#			 break
#		 elif parnum == (len(modi.object.data.bones)-1):
#			 parnum = -1
				
#	 file.write("\t\t<name=\"%s\" bone id=\"%i\" parent=\"%i\">\n" % (bone.name, (i+1), (parnum+1)))
#	 file.write("\t\t\t<abshead>%f %f %f</abshead>\n" % (bone.head_local.x, bone.head_local.y, bone.head_local.z))
#	 file.write("\t\t\t<abstail>%f %f %f</abstail>\n" % (bone.tail_local.x, bone.tail_local.y, bone.tail_local.z))
				
#	 for n, mat in enumerate(weight_dict):
#		 iseffected = 0
#		 for ind in weight_dict[mat]:
#			 for group in context.object.data.vertices[ind[0]].groups:
#				 if context.object.vertex_groups[group.group].name == bone.name:
#					 iseffected += 1
					
#		 if iseffected > 0:
#			 file.write("\t\t\t<vertices mesh=\"%i\">" % n)
#			 for ind in weight_dict[mat]:
#				 for group in context.object.data.vertices[ind[0]].groups:
#					 if context.object.vertex_groups[group.group].name == bone.name:
#						 file.write("%i %f " % (ind[1], group.weight))
#			 file.write("</vertices>\n")
				
#	 file.write("\t\t</bone>\n")
#   file.write("\t</armature>\n\n")
	
#   print("write animations")
#   modi.object.data.pose_position = 'POSE'
#   for act in bpy.data.actions:
#	 actinuse = 0
				
#	 for group in act.groups:
#		 for bone in modi.object.data.bones:
#			 if group.name == bone.name:
#				 actinuse = 1
#				 break
#		 if actinuse == 1:
#			 break
#	 if not len(act.fcurves):
#		 actinuse = 0
				
#	 if actinuse == 1:
#		 keyframes = []
#		 for group in act.groups:
#			 for fc in group.channels:
#				 for kf in fc.keyframe_points:
#					 if int(kf.co[0]) not in keyframes:
#						 keyframes.append(int(kf.co[0]))
					
#		 framemin, framemax = act.frame_range
#		 start_frame = int(framemin)
#		 end_frame = int(framemax)
#		 scene_frames = range(start_frame, end_frame+1)
#		 frame_count = len(scene_frames)
#		 file.write("\t<animation name=\"%s\" duration=\"%i\">\n" % (act.name, frame_count))
#		 for bone in modi.object.data.bones:
#			 file.write("\t\t<bone name=\"%s\">" % bone.name)
#			 for f in keyframes:
#				 context.scene.frame_set(f)
#				 file.write("%i %f %f %f %f %f %f " % (f, bone.head.x, bone.head.y, bone.head.z, bone.tail.x, bone.tail.y, bone.tail.z))
#			 file.write("\t\t</bone>\n")
					
#		 file.write("\t</animation>\n")
			
#   break
			

	


#################################
#Interface and stuff
#################################
from bpy.props import *
class ExportSGM(bpy.types.Operator):
	'''Export to iSDGE model file format (.sgm)'''
	bl_idname = "export.isdge_sgm"
	bl_label = 'Export iSDGE model'

	filepath = StringProperty(name="File Path", description="Filepath used for exporting the iSDGE model file", maxlen= 1024, default= "")
	check_existing = BoolProperty(name="Check Existing", description="Check and warn on overwriting existing files", default=True, options={'HIDDEN'})
	
	#properties
	exptextures = BoolProperty(name="Export textures", description="Reference external image files to be used by the model.", default=True)
	exptangents = BoolProperty(name="Export tangents", description="Generate tangents for the model to use for example tangent space normal mapping.", default=False)
	expshadow = BoolProperty(name="Export shadow", description="Prepare the mesh for shadow volume rendering to speed up loading.", default=False)

	def execute(self, context):
		bpy.ops.object.mode_set(mode='OBJECT')
		obj = c_object(context.object.data, self.exptangents, self.expshadow)
		obj.write(self.properties.filepath, self.exptextures, self.exptangents, self.expshadow)
		return {'FINISHED'}

	def invoke(self, context, event):
		wm = context.window_manager
		wm.fileselect_add(self)
		return {'RUNNING_MODAL'}


def menu_func(self, context):
	default_path = os.path.splitext(bpy.data.filepath)[0] + ".sgm"
	self.layout.operator(ExportSGM.bl_idname, text="iSDGE model (.sgm)").filepath = default_path

def register():
	bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
	bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
	register()