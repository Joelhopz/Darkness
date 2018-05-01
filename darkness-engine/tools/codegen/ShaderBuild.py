import os
from optparse import OptionParser
import subprocess
from ShaderCompiler import stage_from_filename
from CodeGenerator import class_name_from_filename
from CodeGenerator import parsePermutations
from CodeGenerator import optionPermute
from CodeGenerator import enumPermute
from CodeGenerator import permute
from jinja2 import Template
import sys
import json

relative_path_shader_compiler = 'ShaderCompiler.py'
relative_path_shader_codegen = 'ShaderCodegen.py'
relative_path_shader_cpp_interface_template = 'ShaderLoadInterfaceTemplate.h'
relative_path_shader_cpp_implementation_template = 'ShaderLoadInterfaceTemplate.cpp'
relative_path_pipeline_cpp_interface_template = 'ShaderPipelineTemplate.h'
relative_path_pipeline_cpp_implementation_template = 'ShaderPipelineTemplate.cpp'

api_specific_subpath = {
	'vulkan' : 'vulkan',
	'dx12' : 'dx12'
	}

api_specific_binary_ext = {
	'vulkan' : '.spv',
	'dx12' : '.cso'
	}

relative_path_shader_locations = [
	{ 
		'source_folder' : '../../shaders', 
		'destination_folder' : '../../data/shaders', 
		'interface_folder' : '../../include/shaders' }
	]

def recursive_filelist(location):
    result = []
    for root, dirs, files in os.walk(location):
        if dirs:
            for dir in dirs:
                result.extend(recursive_filelist(dir))
        if files:
            for file in files:
                result.append(os.path.join(root, file))
    return result

def printStdout(proc):
	while True:
		line = proc.stdout.readline()
		if line != '':
			print line.rstrip()
		else:
			break

class ShaderBuilder:
	def __init__(self, root_path, graphics_api):
		self.shader_builder_path = root_path
		self.shader_compiler_path = os.path.join(os.path.dirname(root_path), relative_path_shader_compiler)
		self.shader_code_generator_path = os.path.join(os.path.dirname(root_path), relative_path_shader_codegen)
		self.shader_cpp_interface_template = os.path.join(os.path.dirname(root_path), relative_path_shader_cpp_interface_template)
		self.shader_cpp_implementation_template = os.path.join(os.path.dirname(root_path), relative_path_shader_cpp_implementation_template)
		self.all_shaders = []
		
		for shader_location in relative_path_shader_locations:
			src_path = self.get_absolute_path(root_path, shader_location['source_folder'])
			dst_path = self.get_absolute_path(root_path, shader_location['destination_folder'])
			interface_path = self.get_absolute_path(root_path, shader_location['interface_folder'])
			
			files = recursive_filelist(src_path)
			for file in files:
				if file.endswith('.hlsl'):
					relative_path = os.path.relpath(os.path.dirname(file), src_path)
				
					dst_path_rebuilt = os.path.abspath(
						os.path.join(dst_path, api_specific_subpath[str(graphics_api).lower()], relative_path, os.path.basename(file)))
					dst_file = os.path.basename(dst_path_rebuilt)
					(root, ext) = os.path.splitext(dst_file)
					binary_file = os.path.join(os.path.dirname(dst_path_rebuilt), root + api_specific_binary_ext[str(graphics_api).lower()])

					interface_path_rebuilt = os.path.abspath(
						os.path.join(interface_path, relative_path, os.path.basename(file)))
					idst_file = os.path.basename(interface_path_rebuilt)
					(iroot, iext) = os.path.splitext(idst_file)
					interface_file = os.path.join(os.path.dirname(interface_path_rebuilt), iroot + '.h')
					implementation_file = os.path.join(os.path.dirname(interface_path_rebuilt), iroot + '.cpp')
				
					if not os.path.exists(os.path.dirname(interface_file)):
						os.makedirs(os.path.dirname(interface_file))

					if not os.path.exists(os.path.dirname(binary_file)):
						os.makedirs(os.path.dirname(binary_file))

					# check if the source file has changed after last generation
					src_file_exists = os.path.exists(file)
					src_file_modified = os.path.getmtime(file)

					changed = False

					# we need to know the permutations so we can check for changed binary files
					permutations = parsePermutations(file)
					p = permute(permutations)

					binary_file_for_check = binary_file

					binary_file_exists = False
					forceRebuild = False

					if len(p) > 0:
						found_all_permutation_binaries = True
						for combination in p:

							binpath = binary_file
							base_directory = os.path.dirname(os.path.normpath(binpath))
							base_filename_ext = os.path.basename(os.path.normpath(binpath))
							base_filename, base_file_extension = os.path.splitext(base_filename_ext)
							base_directory_and_file = os.path.join(base_directory, base_filename)
							binary_file_perm = base_directory_and_file + '_' + str(combination['id']) + base_file_extension

							permutation_exists = os.path.exists(binary_file_perm)
							if permutation_exists:
								permutation_modified = os.path.getmtime(binary_file_perm)
								if permutation_modified < src_file_modified:
									forceRebuild = True
							else:
								found_all_permutation_binaries = False
						binary_file_exists = found_all_permutation_binaries


					# compile shader
					binary_file_modified = src_file_modified
					if len(p) == 0:
						binary_file_exists = os.path.exists(binary_file_for_check)
						if binary_file_exists:
							binary_file_modified = os.path.getmtime(binary_file_for_check)

					if not binary_file_exists or binary_file_modified < src_file_modified or forceRebuild:
						changed = True
						
						if len(p) > 0:
							for combination in p:
								flags = []
								# combination['id'] == 001
								for perm in combination['list']:
									if perm['type'] == 'option' and perm['value'] == 'true':
										flags.append(perm['flag'])
									if perm['type'] == 'enum':
										flags.append(perm['flag'])
								flagStr = ''
								flagLen = len(flags)
								for i in range(flagLen):
									flagStr += '-D'+str(flags[i])
									if i < flagLen-1:
										flagStr += ' '

								binpath = binary_file
								base_directory = os.path.dirname(os.path.normpath(binpath))
								base_filename_ext = os.path.basename(os.path.normpath(binpath))
								base_filename, base_file_extension = os.path.splitext(base_filename_ext)
								base_directory_and_file = os.path.join(base_directory, base_filename)
								binary_file_perm = base_directory_and_file + '_' + str(combination['id']) + base_file_extension

								printStdout(subprocess.Popen([sys.executable, self.shader_compiler_path, '-g', graphics_api, '-i', file, '-o', binary_file_perm, '', flagStr], stdout = subprocess.PIPE))
						else:
							printStdout(subprocess.Popen([sys.executable, self.shader_compiler_path, '-g', graphics_api, '-i', file, '-o', binary_file], stdout = subprocess.PIPE))

					# generate shader interface
					interface_file_exists = os.path.exists(interface_file)
					interface_file_modified = src_file_modified
					if interface_file_exists:
						interface_file_modified = os.path.getmtime(interface_file)
					if not interface_file_exists or interface_file_modified < src_file_modified:
						changed = True
						#print 'python'+self.shader_code_generator_path+'-i '+file+' -t '+os.path.join(os.path.dirname(root_path), relative_path_shader_cpp_interface_template)+' -o '+interface_file+' -b '+binary_file+' -s '+stage_from_filename(file)

						# generate hotreload shader support file
						support_file_content = {'executable': sys.executable, 'shader_compiler_path': self.shader_compiler_path, 'graphics_api': graphics_api, 'file': file, 'binary_file': binary_file, 'root_path': root_path }
						support_file_path = os.path.join(os.path.dirname(dst_path_rebuilt), root + '.support')
						with open(support_file_path, 'w') as support_file:
							support_file.write(json.dumps(support_file_content, sort_keys=True, indent=4))

						# generate CPP interface
						printStdout(subprocess.Popen([
							'python', 
							self.shader_code_generator_path, 
							'-i', file, 
							'-t', self.shader_cpp_interface_template,
							'-o', interface_file,
							'-b', binary_file,
							'-x', '',
							'-s', stage_from_filename(file)], stdout = subprocess.PIPE, stderr = subprocess.PIPE))

						# generate CPP implementation
						printStdout(subprocess.Popen([
							'python', 
							self.shader_code_generator_path, 
							'-i', file, 
							'-t', self.shader_cpp_implementation_template,
							'-o', implementation_file,
							'-b', binary_file,
							'-x', support_file_path,
							'-s', stage_from_filename(file)], stdout = subprocess.PIPE, stderr = subprocess.PIPE))

					# save stage for pipeline processing
					info = self.get_shader_info(file, interface_file, changed)
					common_name = self.get_common_name(info['shader_type_name'])
					found = False
					for x in range(len(self.all_shaders)):
						if self.all_shaders[x]['common_shader_name'] == common_name:
							self.all_shaders[x]['stages'].append(info)
							found = True
					if not found:
						self.all_shaders.append({
							'common_shader_name' : common_name,
							'stages' : [info]
							})

		# create PipelineConfiguration
		for common_info in self.all_shaders:
			if common_info['stages'][0]['changed']:
				dir = os.path.dirname(common_info['stages'][0]['shader_interface_filepath'])
				vs = False
				ps = False
				gs = False
				hs = False
				ds = False
				cs = False

				cvs = 'false'
				cps = 'false'
				cgs = 'false'
				chs = 'false'
				cds = 'false'
				ccs = 'false'

				vs_type = ''
				ps_type = ''
				gs_type = ''
				hs_type = ''
				ds_type = ''
				cs_type = ''

				vs_if = ''
				ps_if = ''
				gs_if = ''
				hs_if = ''
				ds_if = ''
				cs_if = ''

				for stage in common_info['stages']:
					if stage['stage'] == 'Vertex':
						vs = True
						cvs = 'true'
						vs_type = stage['shader_type_name']
						vs_if = stage['shader_interface_filepath']
					if stage['stage'] == 'Pixel':
						ps = True
						cps = 'true'
						ps_type = stage['shader_type_name']
						ps_if = stage['shader_interface_filepath']
					if stage['stage'] == 'Geometry':
						gs = True
						cgs = 'true'
						gs_type = stage['shader_type_name']
						gs_if = stage['shader_interface_filepath']
					if stage['stage'] == 'Hull':
						hs = True
						chs = 'true'
						hs_type = stage['shader_type_name']
						hs_if = stage['shader_interface_filepath']
					if stage['stage'] == 'Domain':
						ds = True
						cds = 'true'
						ds_type = stage['shader_type_name']
						ds_if = stage['shader_interface_filepath']
					if stage['stage'] == 'Compute':
						cs = True
						ccs = 'true'
						cs_type = stage['shader_type_name']
						cs_if = stage['shader_interface_filepath']

				pipeline_if_filepath = os.path.join(dir, common_info['common_shader_name']+'.h')
				pipeline_im_filepath = os.path.join(dir, common_info['common_shader_name']+'.cpp')
				pipeline_info = {
					'pipeline_interface_filepath' : os.path.basename(os.path.join(dir, common_info['common_shader_name']+'.h')),
					'pipeline_implementation_filepath' : os.path.basename(os.path.join(dir, common_info['common_shader_name']+'.cpp')),
					'pipeline_type_name' : common_info['common_shader_name'],
					'has_vertex_shader' : vs,
					'has_pixel_shader' : ps,
					'has_geometry_shader' : gs,
					'has_hull_shader' : hs,
					'has_domain_shader' : ds,
					'has_compute_shader' : cs,
					'chas_vertex_shader' : cvs,
					'chas_pixel_shader' : cps,
					'chas_geometry_shader' : cgs,
					'chas_hull_shader' : chs,
					'chas_domain_shader' : cds,
					'chas_compute_shader' : ccs,
					'vertex_shader_type' : vs_type,
					'pixel_shader_type' : ps_type,
					'geometry_shader_type' : gs_type,
					'hull_shader_type' : hs_type,
					'domain_shader_type' : ds_type,
					'compute_shader_type' : cs_type,
					'vertex_shader_if' : os.path.basename(vs_if),
					'pixel_shader_if' : os.path.basename(ps_if),
					'geometry_shader_if' : os.path.basename(gs_if),
					'hull_shader_if' : os.path.basename(hs_if),
					'domain_shader_if' : os.path.basename(ds_if),
					'compute_shader_if' : os.path.basename(cs_if)
					}

				pipeline_interface_template = os.path.join(os.path.dirname(root_path), relative_path_pipeline_cpp_interface_template)
				pipeline_implementation_template = os.path.join(os.path.dirname(root_path), relative_path_pipeline_cpp_implementation_template)

				with open(pipeline_interface_template, 'r') as file:
					template_file = Template(file.read())
					with open(pipeline_if_filepath, 'w') as output_file:
						output_file.write(template_file.render(pipeline_info))

				with open(pipeline_implementation_template, 'r') as file:
					template_file = Template(file.read())
					with open(pipeline_im_filepath, 'w') as output_file:
						output_file.write(template_file.render(pipeline_info))

	def get_shader_info(self, file_name, interface_path, changed):
		return {
			'shader_interface_filepath' : interface_path,
			'shader_type_name' : class_name_from_filename(file_name),
			'stage' : stage_from_filename(file_name),
			'changed' : changed
			}

	def get_common_name(self, shader_type_name):
		return shader_type_name[:-2]

	def get_absolute_path(self, root_path, path):
		return os.path.normpath(os.path.join(os.path.dirname(root_path), path))

def main():
	this_file_path = os.path.realpath(__file__)

	parser = OptionParser()
	parser.add_option("-g", "--graphics-api", dest="graphicsapi", help="select graphics api. example 1: -g VULKAN , example 2: -g DX12")
	parser.add_option("-i", "--input", dest="input", help="input file. example: -i C:\\work\\Test.hlsl")
	parser.add_option("-o", "--output", dest="output", help="output file. example: -o C:\\work\\Test.frag.spv")
	parser.add_option("-D", "--define", action='append', dest="define", help="example: -D DEBUG")
	parser.add_option("-I", "--include", action='append', dest="include", help="example: -I ../inc")

	options, arguments = parser.parse_args()

	shader_builder = ShaderBuilder(this_file_path, options.graphicsapi)

if __name__ == "__main__":
	main()
