import os
import itertools
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

def parsePermutations(filename):
	permutations = {'enums': [], 'options': []}
	with open(filename, 'r') as file:
		data = file.readlines()
		for line in data:
			if str(line).startswith('#if') and str(line).find('ENUM_') != -1:
				s = str(str(line).rstrip()).split(' ')
				for part in s:
					if str(part).startswith('ENUM'):
						enum_parts = str(part).split('_')
						enum_typename = str(enum_parts[1]).lower().title()
						enum_value = str(enum_parts[2]).lower().title()
						for value_part in enum_parts[3:]:
							enum_value += str(value_part).lower().title()
						#enum_value = str(str.join(enum_parts[2:], " ").title()).split(' ').join()
						#enum_value = str(enum_value[0]).upper() + str(enum_value[1:])
						found = False
						for x in range(len(permutations['enums'])):
							if enum_typename in permutations['enums'][x]:
								enum_value_found = False
								for key, value in permutations['enums'][x].iteritems():
									for evalue in value:
										if evalue['value'] == enum_value:
											enum_value_found = True
											break
								if not enum_value_found:
									permutations['enums'][x][enum_typename].append({'value': enum_value, 'flag': str(part)})
								found = True
								break
						if not found:
							permutations['enums'].append({enum_typename : [{'value': enum_value, 'flag': str(part)}]})
			if str(line).startswith('#if') and str(line).find('OPTION_') != -1:
				s = str(str(line).rstrip()).split(' ')
				for part in s:
					if str(part).startswith('OPTION'):
						option_parts = str(part).split('_')
						option_value = str(option_parts[1]).lower()
						for value_part in option_parts[2:]:
							option_value += str(value_part).lower().title()

						found = False
						for option_value_test in permutations['options']:
							for okey, ovalue in option_value_test.iteritems():
								if okey == 'value' and ovalue == option_value:
									found = True
									break
							if found:
								break
						if not found:
							if option_value not in permutations['options']:
								permutations['options'].append({'value': option_value, 'flag': str(part)})
	return permutations

def optionPermute(options):
	result = []
	count = 1 << len(options)
	currentValue = 0
	for i in range(count):
		res = []
		for a in range(len(options)):
			if currentValue & (1 << a) == (1 << a):
				res.append({'type': 'option', 'variable_name': options[a]['value'], 'value': 'true', 'flag': options[a]['flag']})
			else:
				res.append({'type': 'option', 'variable_name': options[a]['value'], 'value': 'false', 'flag': options[a]['flag']})
		result.append(res)
		currentValue += 1
	return result

def enumPermute(enums):
	lists = []
	for dic in enums:
		temp = []
		for key, value in dic.iteritems():
			for enum_value in value:
				temp.append({'type': 'enum', 'variable_name': str(key).lower(), 'value': str(key).title()+'::'+str(enum_value['value']), 'flag': enum_value['flag']})
		lists.append(temp)
	return list(itertools.product(*lists))

def permute(permutations):
	result = []

	op = optionPermute(permutations['options'])
	en = enumPermute(permutations['enums'])
	
	permutation_id = 0
	for o in op:
		for e in en:
			tempdict = {}
			temp = []
			temp.extend(o)
			temp.extend(e)
			if len(temp) > 0:
				tempdict['list'] = temp
				tempdict['id'] = '%03d' % permutation_id

				flags = []
				for perm in tempdict['list']:
					if perm['type'] == 'option' and perm['value'] == 'true':
						flags.append(perm['flag'])
					if perm['type'] == 'enum':
						flags.append(perm['flag'])

				tempdict['defines'] = flags
				result.append(tempdict)
				permutation_id += 1
	return result

class CodeGenerator:
	def __init__(self, root_level_definition_tokens, template, output, stage, binary_path, support_path, src_path):
		self.stage = stage
		self.binary_path = binary_path
		self.support_path = support_path
		filename, file_extension = os.path.splitext(os.path.basename(output))
		self.interface_path = filename + '.h'
		permutations = parsePermutations(src_path)
		p = permute(permutations)

		# permutations['enums'] structure is a list of dictionaries
		# permutations['enums'] = [{'enum_typename', ['enum_value1', 'enum_value2']}, 
		#							{'enum_typename2', ['enum_value1', 'enum_value2']}]
		# permutations['enums'][enum number]['Mode'][enum value number] == 'enum_value1'
		#for perm in permutations['enums']:
		#	for enum_typename, enum_values in perm.iteritems():
		#		for enum_value in enum_values:
		#			print str(enum_typename) + str(enum_value)
		# permutations['options'] is just a list of boolean type variable names

		template_data = self.produce_template_data(root_level_definition_tokens, output)
		template_data['permutations'] = p
		template_data['enums'] = permutations['enums']
		template_data['options'] = permutations['options']

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

		binpath = self.binary_path.replace('\\', '/')
		base_directory = os.path.dirname(os.path.normpath(binpath))
		base_filename_ext = os.path.basename(os.path.normpath(binpath))
		base_filename, base_file_extension = os.path.splitext(base_filename_ext)
		base_directory_and_file = os.path.join(base_directory, base_filename)

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

			'BaseExt': base_file_extension,
			'BasePathAndFile': base_directory_and_file.replace('\\', '/'),

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
			'descriptor_count' : 0,

			'srvs' : [],
			'uavs' : [],
			'dimensions' : [],

			'input_parameters' : []
			}
		for token in root_level_tokens:
			if token.syntax_type == 'function' and token.name == 'main':
				for child in token.childs:
					result['input_parameters'].append({'name' : child.name, 'semantic' : child.semantic, 'type' : child.type })
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
							result['srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_bindless_buffer_srvs'] = True
						else:
							result['buffer_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_buffer_srvs'] = True
						result['descriptor_count'] += 1
					else:
						if 'Bindless' in token.type:
							result['bindless_texture_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['dimensions'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name, 'dimension': token.dimension })
							result['has_bindless_texture_srvs'] = True
						else:
							result['texture_srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['srvs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['dimensions'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name, 'dimension': token.dimension })
							result['has_texture_srvs'] = True
						result['descriptor_count'] += 1
						
				if self.type_is_uav(token.type):
					if 'Buffer' in token.type:
						if 'Bindless' in token.type:
							result['bindless_buffer_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_bindless_buffer_uavs'] = True
						else:
							result['buffer_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['has_buffer_uavs'] = True
						result['descriptor_count'] += 1
						
					else:
						if 'Bindless' in token.type:
							result['bindless_texture_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['dimensions'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name, 'dimension': token.dimension })
							result['has_bindless_texture_uavs'] = True
						else:
							result['texture_uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['uavs'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name })
							result['dimensions'].append({'type' : self.get_cpp_type(token.type), 'identifier': token.name, 'dimension': token.dimension })
							result['has_texture_uavs'] = True
						result['descriptor_count'] += 1
						
		return result
