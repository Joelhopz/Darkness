import os
from optparse import OptionParser
from PreprocessorHLSL import Preprocessor
from LexicalAnalyzerHLSL import LexicalAnalyzer
from SyntaxAnalyzerHLSL import SyntaxAnalyzer
from SyntaxAnalyzerHLSL import SyntaxNode
from HistoryIterator import HistoryIterException
from HistoryIterator import HistoryIter
from CodeGenerator import CodeGenerator

def print_nodes(node, ident = ''):
	msg = str(node.linenumber) + ident + ' ' + node.type + ' ' + node.name + ' ' + node.register
	if len(node.parameters) > 0:
		msg += '('
	for syntax_node in node.parameters:
		msg += syntax_node.type + ' ' + syntax_node.name + ' ' + syntax_node.register + ', '
	if len(node.parameters) > 0:
		msg += ')'
	print msg
	for syntax_node in node.childs:
		print_nodes(syntax_node, ident + '    ')
	

def main():
	parser = OptionParser()
	parser.add_option("-i", "--input", dest="input", help="input file. example: -i C:\\work\\Test.hlsl")
	parser.add_option("-o", "--output", dest="output", help="output file. example: -o C:\\work\\Test.frag.spv")
	parser.add_option("-D", "--define", action='append', dest="define", help="example: -D DEBUG")
	parser.add_option("-I", "--include", action='append', dest="include", help="example: -I ../inc")
	parser.add_option("-t", "--template", dest='template', help="example: -t ShaderLoadInterfaceTemplate.h")
	parser.add_option("-s", "--stage", dest='stage', help="example: -s Vertex")
	parser.add_option("-b", "--binarylocation", dest='binarylocation', help="example: -b C:\\work\\some_vertex.vs.spv")
	parser.add_option("-x", "--support_path", dest='support_path', help="example: -x C:\\work\\some_vertex.vs.support")

	options, arguments = parser.parse_args()

	with open(options.input, 'r') as file:
		preprocessor = Preprocessor(file, options.define, options.include)
		lexical_analyzer = LexicalAnalyzer(preprocessor)
		syntax_analyzer = SyntaxAnalyzer(lexical_analyzer)
		codeGenerator = CodeGenerator(
			syntax_analyzer.root_level_declarations(),
			options.template,
			options.output,
			options.stage,
			options.binarylocation,
			options.support_path)

if __name__ == "__main__":
	main()
