import os
from jinja2 import Template
from SyntaxAnalyzerHLSL import SyntaxNode
from SyntaxAnalyzerHLSL import SyntaxAnalyzer
from itertools import groupby
from SyntaxAnalyzerHLSL import complete_system_types

srvs_types = [
	'Buffer',	# Buffer<float4>
	'Texture1D',
	'Texture1DArray',
	'Texture2D',
	'Texture2DArray',
	'Texture3D',
	'TextureCube',
	'TextureCubeArray',

	'Texture2DMS',
	'Texture2DMSArray',

    'StructuredBuffer'
	]

uavs_types = [
	'RWBuffer',
	'RWByteAddressBuffer',
	'RWStructuredBuffer',
    'AppendStructuredBuffer',
	'RWTexture1D',
	'RWTexture1DArray',
	'RWTexture2D',
	'RWTexture2DArray',
	'RWTexture3D'
	]

def class_name_from_filename(filename):
	basename = os.path.basename(filename)		# SomeShader.vs.hlsl
	(root, ext) = os.path.splitext(basename)	# root = SomeShader.vs, ext = .hlsl
	(sroot, sext) = os.path.splitext(root)		# sroot = SomeShader, sext = .vs
	return sroot+sext[1:].upper()				# SomeShaderVS

def pipeline_name_from_filename(filename):
	basename = os.path.basename(filename)		# SomeShader.vs.hlsl
	(root, ext) = os.path.splitext(basename)	# root = SomeShader.vs, ext = .hlsl
	(sroot, sext) = os.path.splitext(root)		# sroot = SomeShader, sext = .vs
	return sroot								# SomeShaderVS

class CodeGenerator:
	def __init__(self, root_level_definition_tokens, template, output, stage, binary_path, support_path):
		self.stage = stage
		self.binary_path = binary_path
		self.support_path = support_path
		filename, file_extension = os.path.splitext(os.path.basename(output))
		self.interface_path = filename + '.h'
		template_data = self.produce_template_data(root_level_definition_tokens, output)
		with open(template, 'r') as file:
			template_file = Template(file.read())
			with open(output, 'w') as output_file:
				output_file.write(template_file.render(template_data))
			
	def type_is_srv(self, type):
		parts = type.split('<')
		return parts[0] in srvs_types

	def type_is_uav(self, type):
		parts = type.split('<')
		return parts[0] in uavs_types

	def get_cpp_type(self, type):
		if self.type_is_srv(type):
			if 'Buffer' in type:
				if 'Bindless' in type:
					return 'BufferBindlessSRV'
				else:
					return 'BufferSRV'
			elif 'Texture' in type:
				if 'Bindless' in type:
					return 'TextureBindlessSRV'
				else:
					return 'TextureSRV'
		if self.type_is_uav(type):
			if 'Buffer' in type:
				if 'Bindless' in type:
					return 'BufferBindlessUAV'
				else:
					return 'BufferUAV'
			elif 'Texture' in type:
				if 'Bindless' in type:
					return 'TextureBindlessUAV'
				else:
					return 'TextureUAV'
		if type in complete_system_types:
			return type[:1].upper()+type[1:]

	def produce_template_data(self, root_level_tokens, output):
		class_name = class_name_from_filename(output)
		pipeline_configuration_class = pipeline_name_from_filename(output)

		result = {
			'has_constants' : False,
			'has_texture_srvs' : False,
			'has_texture_uavs' : False,
			'has_bindless_texture_srvs' : False,
			'has_bindless_texture_uavs' : False,
			'has_buffer_srvs' : False,
			'has_buffer_uavs' : False,
			'has_bindless_buffer_srvs' : False,
			'has_bindless_buffer_uavs' : False,
			'has_samplers' : False,
			'ShaderClass' : class_name,
			'class_type' : self.stage + 'Shader',
			'ShaderLoadInterfaceHeader' : self.interface_path,
			'shader_pipeline_configuration_class' : pipeline_configuration_class,
			'ShaderBinaryPath': self.binary_path.replace('\\', '/'),
			'ShaderSupportPath': self.support_path.replace('\\', '/'),
			'constant_structures' : [],
			'texture_srvs' : [],
			'texture_uavs' : [],
			'bindless_texture_srvs' : [],
			'bindless_texture_uavs' : [],
			'buffer_srvs' : [],
			'buffer_uavs' : [],
			'bindless_buffer_srvs' : [],
			'bindless_buffer_uavs' : [],
			'samplers' : [],
			'descriptor_count' : 0
			}
		for token in root_level_tokens:
			if token.type == 'cbuffer':
				result['has_constants'] = True
				identifiers = []
				for child in token.childs:
					identifiers.append({'type': self.get_cpp_type(child.type), 'name': child.name })
				result['constant_structures'].append({
					'name': token.name, 
					'identifier': token.name[:1].lower()+token.name[1:], 
					'identifiers': identifiers })
				result['descriptor_count'] += 1
			if token.type == 'sampler' or token.type == 'SamplerComparisonState':
				result['has_samplers'] = True
				result['samplers'].append({ 'name' : token.name, 'identifier': token.name[:1].lower()+token.name[1:] })
			else:
				if self.type_is_srv(token.type):
					if 'Buffer' in token.type:
						if 'Bindless' in token.type:
							result['bindless_buffer_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_bindless_buffer_srvs'] = True
						else:
							result['buffer_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_buffer_srvs'] = True
						result['descriptor_count'] += 1
					else:
						if 'Bindless' in token.type:
							result['bindless_texture_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_bindless_texture_srvs'] = True
						else:
							result['texture_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_texture_srvs'] = True
						result['descriptor_count'] += 1
						
				if self.type_is_uav(token.type):
					if 'Buffer' in token.type:
						if 'Bindless' in token.type:
							result['bindless_buffer_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_bindless_buffer_uavs'] = True
						else:
							result['buffer_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_buffer_uavs'] = True
						result['descriptor_count'] += 1
						
					else:
						if 'Bindless' in token.type:
							result['bindless_texture_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_bindless_texture_uavs'] = True
						else:
							result['texture_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_texture_uavs'] = True
						result['descriptor_count'] += 1
						
		return result
