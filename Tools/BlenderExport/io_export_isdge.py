#################################
#Addon header
#################################
bl_addon_info = {
	'name': 'Export: iSDGE model format',
	'author': 'Nils Daumann',
	'version': '0.2 2011/01/25',
	'blender': (2, 5, 3),
	'category': 'Import/Export',
	'location': 'File > Export'}


#################################
#Includes
#################################
import os
import bpy


#Container classes for a better overview
class c_vertex(object):
	__slots__ = 'blendindex', 'position', 'uvs', 'color'
	def __init__(self, blendindex, position = (0,0,0), uvs = [], color = None):
		self.blendindex = blendindex	#index within the blender mesh
		self.position = position
		self.uvs = uvs
		self.color = color

class c_triangle(object):
	__slots__ = 'vertices', 'images', 'newindices'
	def __init__(self, vertices = [], images = [], newindices = None):
		self.vertices = vertices
		self.images = images
		self.newindices = newindices		#indices within the c_mesh
		
class c_mesh(object):
	__slots__ = 'triangles', 'images', 'vertices', 'indices'
	def __init__(self, images = [], triangles = [], vertices = [], indices = []):
		self.vertices = vertices
		self.triangles = triangles
		self.images = images
		self.indices = indices
	
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
			


class c_object(object):
	__slots__ = 'meshs'
	#splits the blender object into triangle meshs with the same textures
	def __init__(self, obj):
		triangles = []
		#generate vertices and triangles
		for i, face in enumerate(obj.faces):
			images = []
			for tex in obj.uv_textures:
				imgpath = tex.data[i].image.filepath
				img = imgpath.split('/')
				images.append(img[len(img)-1])
			
			verts = []
			for n, vertind in enumerate(face.vertices):
				uvs = []
				for tex in obj.uv_textures:
					uvs.append((round(tex.data[i].uv[n][0], 6), round(tex.data[i].uv[n][1], 6)))
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
				verts.append(c_vertex(vertind, obj.vertices[vertind].co, uvs, color))
			
			if len(face.vertices) == 3:
				triangles.append(c_triangle(verts, images))
			else:
				tri1 = [verts[0], verts[1], verts[2]]
				tri2 = [verts[0], verts[2], verts[3]]
				triangles.append(c_triangle(tri1, images))
				triangles.append(c_triangle(tri2, images))
		
		#generate meshs
		self.meshs = []
		self.meshs.append(c_mesh(triangles[0].images))
		for tri in triangles:
			check = 0
			for mesh in self.meshs:
				if mesh.images == tri.images:
					mesh.triangles.append(tri)
					check = 1
					break
			if check == 0:
				self.meshs.append(c_mesh(tri.images, [tri]))
		
		for mesh in self.meshs:
			mesh.uvsplit()
	
	
	
	def write(self, filename):
		print("open or create file")
		file = open( filename, 'w' )
	
		file.write("<?xml version=\"1.0\" ?>\n")
		file.write("<environment>\n")
		
		print("write materials")
		for i, mesh in enumerate(self.meshs):
			file.write("\t<material id=\"%i\">\n" % i)
			file.write("\t\t<textures>")
			for img in mesh.images:
				file.write("%s " % img)
			file.write("</textures>\n")
			file.write("\t</material>\n")
		
		file.write("\n")
		
		print("write meshs")
		for i, mesh in enumerate(self.meshs):
			file.write("\t<mesh id=\"%i\" material=\"%i\" texcoordcount=\"%i\">\n" % (i, i, len(mesh.images)))
		
			print("write vertices")
			file.write("\t\t<vertexpos>")
			for vertex in mesh.vertices:
				file.write("%f %f %f " % (-vertex.position.x, vertex.position.z, vertex.position.y))
			file.write("</vertexpos>\n")
		
			print("write texcoords")
			set = 0
			while set < len(mesh.images):
				file.write("\t\t<vertextexcoord id=\"%i\">" % set)
				for vertex in mesh.vertices:
					file.write("%f %f " % (vertex.uvs[set][0], 1.0-vertex.uvs[set][1]))
				file.write("</vertextexcoord>\n")
				set += 1
				
			if mesh.vertices[0].color != None:
				print("write colors")
				file.write("\t\t<vertexcol>")
				for vertex in mesh.vertices:
					file.write("%f %f %f %f " % (vertex.color[0], vertex.color[1], vertex.color[2], vertex.color[3]))
				file.write("</vertexcol>\n")
		
			print("write indices")
			file.write("\t\t<indices>")
			for ind in mesh.indices:
				file.write("%i " % ind)
			file.write("</indices>\n")
			file.write("\t</mesh>\n")
			
		file.write("\n")
		
		file.write("</environment>\n")
	
		file.close()
		print("finished export")


#################################
#Writing the file
#################################
def exportobject(filename, context):
	
	bpy.ops.object.mode_set(mode='OBJECT')
	obj = c_object(context.object.data)
	obj.write(filename)
	
	
#   for i, modi in enumerate(context.object.modifiers):
#	   if modi.type == 'ARMATURE':
#		   print("write armature")
			
#		   modi.object.data.pose_position = 'REST'
#		   file.write("\t<armature name=\"%s\">\n" % modi.object.data.name)
#		   for i, bone in enumerate(modi.object.data.bones):
#			   for parnum, parent in enumerate(modi.object.data.bones):
#				   if parent == bone.parent:
#					   break
#				   elif parnum == (len(modi.object.data.bones)-1):
#					   parnum = -1
				
#			   file.write("\t\t<name=\"%s\" bone id=\"%i\" parent=\"%i\">\n" % (bone.name, (i+1), (parnum+1)))
#			   file.write("\t\t\t<abshead>%f %f %f</abshead>\n" % (bone.head_local.x, bone.head_local.y, bone.head_local.z))
#			   file.write("\t\t\t<abstail>%f %f %f</abstail>\n" % (bone.tail_local.x, bone.tail_local.y, bone.tail_local.z))
				
#			   for n, mat in enumerate(weight_dict):
#				   iseffected = 0
#				   for ind in weight_dict[mat]:
#					   for group in context.object.data.vertices[ind[0]].groups:
#						   if context.object.vertex_groups[group.group].name == bone.name:
#							   iseffected += 1
					
#				   if iseffected > 0:
#					   file.write("\t\t\t<vertices mesh=\"%i\">" % n)
#					   for ind in weight_dict[mat]:
#						   for group in context.object.data.vertices[ind[0]].groups:
#							   if context.object.vertex_groups[group.group].name == bone.name:
#								   file.write("%i %f " % (ind[1], group.weight))
#					   file.write("</vertices>\n")
				
#			   file.write("\t\t</bone>\n")
#		   file.write("\t</armature>\n\n")
	
#		   print("write animations")
#		   modi.object.data.pose_position = 'POSE'
#		   for act in bpy.data.actions:
#			   actinuse = 0
				
#			   for group in act.groups:
#				   for bone in modi.object.data.bones:
#					   if group.name == bone.name:
#						   actinuse = 1
#						   break
#				   if actinuse == 1:
#					   break
#			   if not len(act.fcurves):
#				   actinuse = 0
				
#			   if actinuse == 1:
#				   keyframes = []
#				   for group in act.groups:
#					   for fc in group.channels:
#						   for kf in fc.keyframe_points:
#							   if int(kf.co[0]) not in keyframes:
#								   keyframes.append(int(kf.co[0]))
					
#				   framemin, framemax = act.frame_range
#				   start_frame = int(framemin)
#				   end_frame = int(framemax)
#				   scene_frames = range(start_frame, end_frame+1)
#				   frame_count = len(scene_frames)
#				   file.write("\t<animation name=\"%s\" duration=\"%i\">\n" % (act.name, frame_count))
#				   for bone in modi.object.data.bones:
#					   file.write("\t\t<bone name=\"%s\">" % bone.name)
#					   for f in keyframes:
#						   context.scene.frame_set(f)
#						   file.write("%i %f %f %f %f %f %f " % (f, bone.head.x, bone.head.y, bone.head.z, bone.tail.x, bone.tail.y, bone.tail.z))
#					   file.write("\t\t</bone>\n")
					
#				   file.write("\t</animation>\n")
			
#		   break
			

	


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

	def execute(self, context):
		exportobject(self.properties.filepath, context)
		return {'FINISHED'}

	def invoke(self, context, event):
		wm = context.window_manager
		wm.add_fileselect(self)
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