import os
import string
import random
from optparse import OptionParser
from PreprocessorHLSL import PreprocessorException
from PreprocessorHLSL import Preprocessor

from LexicalAnalyzerHLSL import LexicalAnalyzer
from SyntaxAnalyzerHLSL import SyntaxAnalyzer

def stage_from_filename(filename):
	if filename[-7:] == 'cs.hlsl':
		return 'Compute'
	if filename[-7:] == 'vs.hlsl':
		return 'Vertex'
	if filename[-7:] == 'ps.hlsl':
		return 'Pixel'
	if filename[-7:] == 'gs.hlsl':
		return 'Geometry'
	if filename[-7:] == 'hs.hlsl':
		return 'Hull'
	if filename[-7:] == 'ds.hlsl':
		return 'Domain'

VulkanStages = {
	'Compute' : 'comp',
	'Domain'  : 'tesc',
	'Geometry': 'geom',
	'Hull'    : 'tese',
	'Pixel'   : 'frag',
	'Vertex'  : 'vert'
}

class VulkanCompiler:
	def __init__(self, defines, includes):
		self.compiler_binary = 'C:\\VulkanSDK\\1.0.61.1\\Bin\\glslangValidator.exe'
		self.input_flag = '-D --auto-map-bindings -e main -V'
		self.output_flag = '-o'
		self.include_paths = []
		self.defines = []
		if includes is not None:
			self.include_paths.extend(includes)
		if defines is not None:
			self.defines.extend(defines)

	def compile(self, input_file, output_file, bindless):
		temporary_file_path = self.createPreprocessedFile(input_file)
		os.system(self.compiler_binary+' -S '+VulkanStages[stage_from_filename(input_file)]+' '+self.input_flag+' '+temporary_file_path+' '+self.output_flag+' '+output_file)
		self.removePreprocessedFile(temporary_file_path)

	def removePreprocessedFile(self, input_file):
		os.remove(input_file)

	def createPreprocessedFile(self, input_file):
		(dir, filename) = os.path.split(input_file)
		temporary_file_path = os.path.join(dir, self.createTemporaryFilename(filename))
		with open(temporary_file_path, 'w') as file:
			with open(input_file, 'r') as input_file:
				preprocessor = Preprocessor(input_file, self.defines, self.include_paths)
				for chr in preprocessor:
					file.write(chr)
		return temporary_file_path

	def createTemporaryFilename(self, inputFile):
		random_part = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(8))
		return inputFile + '.' + random_part

DX12Stages = {
	'Compute' : 'cs_5_1',
	'Domain'  : 'ds_5_1',
	'Geometry': 'gs_5_1',
	'Hull'    : 'hs_5_1',
	'Pixel'   : 'ps_5_1',
	'Vertex'  : 'vs_5_1'
}

class DX12Compiler:
	def __init__(self, defines, includes):
		self.compilerBinary = '"C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.16299.0\\x64\\fxc.exe"'
		self.inputFlag = ''
		# /Od  for disable optimization
		# /Zpr = Row major
		self.outputFlag = '/nologo /Zpr /Od /Zi /Fo'
		self.include_paths = []
		self.defines = []
		
		if includes is not None:
			self.include_paths.extend(includes)
		if defines is not None:
			self.defines.extend(defines)

	def profile(self, inputFile):
		return DX12Stages[stage_from_filename(inputFile)]

	def compile(self, input_file, output_file, bindless):

		# check input_file for bindless texture
		# /enable_unbounded_descriptor_tables

		defineStr = ''
		for i in range(len(self.defines)):
			defineStr += '/D'+str(self.defines[i])
			if i < len(self.defines)-1:
				defineStr += ' '

		filename, file_extension = os.path.splitext(output_file)
		if not bindless:
			cmd = self.compilerBinary+' /T '+self.profile(input_file)+' '+input_file+' '+self.outputFlag+' '+output_file+' /Fd '+filename+'.pdb'
			if defineStr != '':
				cmd += ' '+defineStr
			os.system(cmd)
		else:
			cmd = self.compilerBinary+' /enable_unbounded_descriptor_tables /T '+self.profile(input_file)+' '+input_file+' '+self.outputFlag+' '+output_file+' /Fd '+filename+'.pdb'
			if defineStr != '':
				cmd += ' '+defineStr
			os.system(cmd)

class Compiler:
	def __init__(self, graphicsApi, defines, includes):
		if graphicsApi.lower() == "vulkan":
			self.compiler = VulkanCompiler(defines, includes)
		elif graphicsApi.lower() == "dx12":
			self.compiler = DX12Compiler(defines, includes)

	def compile(self, inputFile, outputFile, bindless):
		self.compiler.compile(inputFile, outputFile, bindless)

# cd "$(ProjectDir)..\..\data\engine\graphics\shaders" && 
# del %(Filename).frag.spv && 
# C:\VulkanSDK\1.0.21.1\Bin\glslangValidator.exe -s -V "%(FullPath)" && 
# rename frag.spv %(Filename).frag.spv
# -i C:\work\darkness\darkness-engine\shaders\core\culling\OcclusionCulling.cs.hlsl -t C:\work\darkness\darkness-engine\tools\codegen\ShaderLoadInterfaceTemplate.cpp -o C:\work\darkness\darkness-engine\include\shaders\core\culling\OcclusionCulling.cs.cpp -b C:\work\darkness\darkness-engine\data\shaders\dx12\core\culling\OcclusionCulling.cs.cso -s Compute -x C:\work\darkness\darkness-engine\data\shaders\dx12\core\culling\OcclusionCulling.cs.support

def main():
	parser = OptionParser()
	parser.add_option("-g", "--graphics-api", dest="graphicsapi", help="select graphics api. example 1: -g VULKAN , example 2: -g DX12")
	parser.add_option("-i", "--input", dest="input", help="input file. example: -i C:\\work\\Test.frag")
	parser.add_option("-o", "--output", dest="output", help="output file. example: -o C:\\work\\Test.frag.spv")
	parser.add_option("-D", "--define", action='append', dest="define", help="example: -DDEBUG")
	parser.add_option("-I", "--include", action='append', dest="include", help="example: -I ../inc")

	options, arguments = parser.parse_args()

	bindless = False
	with open(options.input, 'r') as file:
		preprocessor = Preprocessor(file, options.define, options.include)
		lexical_analyzer = LexicalAnalyzer(preprocessor)
		syntax_analyzer = SyntaxAnalyzer(lexical_analyzer)

		for token in syntax_analyzer.root_level_declarations():
			if token.type != 'cbuffer':
				if 'Bindless' in token.type:
					bindless = True

	compiler = Compiler(options.graphicsapi, options.define, options.include)
	compiler.compile(options.input, options.output, bindless)

if __name__ == "__main__":
	main()
