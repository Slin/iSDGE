#################################
#Addon header
#################################
bl_addon_info = {
	'name': 'Export: iSDGE model format',
	'author': 'Nils Daumann',
	'version': '0.1 2010/07/26',
	'blender': (2, 5, 3),
	'category': 'Import/Export',
	'location': 'File > Export'}


#################################
#Includes
#################################
import os
import bpy


#################################
#Get triangles
#################################
class tri_wrapper(object):
	__slots__ = 'vertex_index', 'mat', 'image', 'faceuvs', 'offset'
	def __init__(self, vindex = (0,0,0), mat = None, image = None, faceuvs = None):
		self.vertex_index = vindex
		self.mat = mat
		self.image = image
		self.faceuvs = faceuvs
		self.offset = [0, 0, 0]

def uv_key(uv):
	return round(uv[0], 6), round(uv[1], 6)

def extract_triangles(mesh):
	tri_list = []
	do_uv = len(mesh.uv_textures)

	img = None
	for i, face in enumerate(mesh.faces):
		f_v = face.verts

		uf = mesh.active_uv_texture.data[i] if do_uv else None

		if do_uv:
			f_uv = uf.uv
			img = uf.image if uf else None
			if img: img = img.name

		if len(f_v)==3:
			new_tri = tri_wrapper((f_v[0], f_v[1], f_v[2]), face.material_index, img)
			if (do_uv): new_tri.faceuvs = uv_key(f_uv[0]), uv_key(f_uv[1]), uv_key(f_uv[2])
			tri_list.append(new_tri)

		else: #it's a quad
			new_tri = tri_wrapper((f_v[0], f_v[1], f_v[2]), face.material_index, img)
			new_tri_2 = tri_wrapper((f_v[0], f_v[2], f_v[3]), face.material_index, img)

			if (do_uv):
				new_tri.faceuvs = uv_key(f_uv[0]), uv_key(f_uv[1]), uv_key(f_uv[2])
				new_tri_2.faceuvs = uv_key(f_uv[0]), uv_key(f_uv[2]), uv_key(f_uv[3])

			tri_list.append(new_tri)
			tri_list.append(new_tri_2)

	return tri_list


#################################
#Get arrays
#################################
class _point_uv(object):
	__slots__ = 'uv'
	def __init__(self, point=(0.0,0.0)):
		self.uv = point

def extract_arrays(verts, tris):
	points_list = []
	uv_list = []
	index_list = []
	
	# initialize a list of UniqueLists, one per vertex:
	unique_uvs= [{} for i in range(len(verts))]

	# for each face uv coordinate, add it to the UniqueList of the vertex
	for tri in tris:
		for i in range(3):
			context_uv_vert= unique_uvs[tri.vertex_index[i]]
			uvkey= tri.faceuvs[i]

			offset_index__uv_3ds = context_uv_vert.get(uvkey)

			if not offset_index__uv_3ds:
				offset_index__uv_3ds = context_uv_vert[uvkey] = len(context_uv_vert), _point_uv(uvkey)

			tri.offset[i] = offset_index__uv_3ds[0]
	
	# Now we need to duplicate every vertex as many times as it has uv coordinates and make sure the
	# faces refer to the new face indices:
	vert_index = 0
	for i,vert in enumerate(verts):
		index_list.append(vert_index)

		pt = vert.co
		uvmap = [None] * len(unique_uvs[i])
		for ii, uv_3ds in unique_uvs[i].values():
			# add a vertex duplicate to the vertex_array for every uv associated with this vertex:
			points_list.append(pt)
			# add the uv coordinate to the uv array:
			# This for loop does not give uv's ordered by ii, so we create a new map
			# and add the uv's later
			uvmap[ii] = uv_3ds

		# Add the uv's in the correct order
		for uv_3ds in uvmap:
			# add the uv coordinate to the uv array:
			uv_list.append(uv_3ds)

		vert_index += len(unique_uvs[i])

	# Make sure the triangle vertex indices now refer to the new vertex list:
	for tri in tris:
		for i in range(3):
			tri.offset[i] += index_list[tri.vertex_index[i]]
		tri.vertex_index = tri.offset
	
	points_dict = {}
	uv_dict = {}
	index_dict = {}
	index_swap_dict = {}
	material_dict = {}
	
	#sort vertices depending on the texture
	for tri in tris:
		if tri.image not in points_dict:
			points_dict[tri.image] = []
			uv_dict[tri.image] = []
		
		if tri.image not in index_swap_dict:
			index_swap_dict[tri.image] = {}
		
		if tri.vertex_index[0] not in index_swap_dict[tri.image]:
			(index_swap_dict[tri.image])[tri.vertex_index[0]] = len(points_dict[tri.image])
			points_dict[tri.image].append(points_list[tri.vertex_index[0]])
			uv_dict[tri.image].append(uv_list[tri.vertex_index[0]])
		
		if tri.vertex_index[1] not in index_swap_dict[tri.image]:
			(index_swap_dict[tri.image])[tri.vertex_index[1]] = len(points_dict[tri.image])
			points_dict[tri.image].append(points_list[tri.vertex_index[1]])
			uv_dict[tri.image].append(uv_list[tri.vertex_index[1]])
			
		if tri.vertex_index[2] not in index_swap_dict[tri.image]:
			(index_swap_dict[tri.image])[tri.vertex_index[2]] = len(points_dict[tri.image])
			points_dict[tri.image].append(points_list[tri.vertex_index[2]])
			uv_dict[tri.image].append(uv_list[tri.vertex_index[2]])
	
	#correct the indices
	for tri in tris:
		if tri.image not in index_dict:
			index_dict[tri.image] = []
		index_dict[tri.image].append((index_swap_dict[tri.image])[tri.vertex_index[2]])
		index_dict[tri.image].append((index_swap_dict[tri.image])[tri.vertex_index[1]])
		index_dict[tri.image].append((index_swap_dict[tri.image])[tri.vertex_index[0]])
		
		#collect materials
		material_dict[tri.image] = tri.image
	
	return points_dict, uv_dict, index_dict, material_dict


#################################
#Writing the file
#################################
def exportobject(filename, context):
	bpy.ops.object.mode_set(mode='OBJECT')
	
	print("convert quads to tris")
	tri_list = extract_triangles(context.object.data)
	print("convert face uv to vertex uv")
	point_dict, uv_dict, index_dict, material_dict = extract_arrays(context.object.data.verts, tri_list)
	
	print("open or create file")
	file = open( filename, 'w' )

	file.write("<?xml version=\"1.0\" ?>\n")
	file.write("<environment>\n")
	
	print("write materials")
	for i, mat in enumerate(material_dict):
		file.write("\t<material id=\"%i\">\n" % i)
		file.write("\t\t<textures>%s</textures>\n" % mat)
		file.write("\t</material>\n")
	
	file.write("\n")
		
	print("write mesh")
	for i, mat in enumerate(material_dict):
		file.write("\t<mesh id=\"%i\" material=\"%i\" texcoordcount=\"1\">\n" % (i, i))
	
		print("write vertices")
		file.write("\t\t<vertexpos>")
		for vertex in point_dict[mat]:
			file.write("%f %f %f " % (vertex.x, vertex.z, vertex.y))
		file.write("</vertexpos>\n")
	
		print("write texcoords")
		file.write("\t\t<vertextexcoord id=\"0\">")
		for uv in uv_dict[mat]:
			file.write("%f %f " % (uv.uv[0], 1.0-uv.uv[1]))
		file.write("</vertextexcoord>\n")
	
		print("write indices")
		file.write("\t\t<indices>")
		for ind in index_dict[mat]:
			file.write("%i " % ind)
		file.write("</indices>\n")
		file.write("\t</mesh>\n")
	
	file.write("</environment>\n")

	file.close()
	print("finished export")


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
		wm = context.manager
		wm.add_fileselect(self)
		return {'RUNNING_MODAL'}

	def poll(self, context):
		return context.active_object != None


def menu_func(self, context):
	default_path = os.path.splitext(bpy.data.filepath)[0] + ".sgm"
	self.layout.operator(ExportSGM.bl_idname, text="iSDGE model (.sgm)").filepath = default_path


def register():
	bpy.types.register(ExportSGM)
	bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
	bpy.types.unregister(ExportSGM)
	bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
	register()