import sublime, sublime_plugin
import re

def importToIncludeLine(str, cotnext):
	reg = re.compile('^#import (.+)')
	m = reg.match(str)
	if m is None:
		return None
	name = m.group(1)
	if name == "<Foundation/Foundation.h>":
		return ""
	return "#include " + name

def addHeaderMacro(edit, view):
	filename = view.file_name()
	if filename is None:
		filename = "not_saved.h"
	else:
		filename = filename.split("\\").pop()

	key = "__" + filename.upper().replace(".", "_") + "_"
	view.insert(edit, 0, "#ifndef {0}\n#define {0}\n\n".format(key));
	view.insert(edit, view.size(), "\n\n#endif /*{0}*/".format(key));	

def stipStartingComments(edit, view):
	line = view.full_line(0)
	while view.substr(line).startswith("//"):
		view.erase(edit, line)
		line = view.full_line(0)

def convertProperty(source, context):
	reg = re.compile('^@property\s*\((.+)\)\s+(\S+)\s+(\w+);')
	m = reg.match(source)
	if m is None:
		return None
	params = m.group(1)
	datatype = m.group(2)
	name = m.group(3)
	readonly = False
	retain = False
	for param in params.split(","):
		p = param.strip().lower()
		if (p == "readonly"):
			readonly = True
		if (p == "retain"):
			retain = True
	macroname = "CC_SYNTHESIZE"

	if readonly:
		macroname = "CC_SYNTHESIZE_READONLY"
	if retain:
		macroname = "CC_SYNTHESIZE_RETAIN"
	return "	{0}({1}, {2}, {3});".format(macroname, datatype, name, name.capitalize())

def covnertClass(source, context):
	reg = re.compile('^@class\s+(\S+);')
	m = reg.match(source)	
	if m is None:
		return None
	classname = m.group(1)
	return "class {0};".format(classname)

def convertInterface(source, context):
	reg = re.compile('^@interface\s+(\w+)\s*\:\s*(\w*)(\<(\S+)\>)?\w*{?')
	m = reg.match(source)	
	if m is None:
		return None
	classname = m.group(1)
	parent = m.group(2)
	interfaces = m.group(4)
	parents = "	: public {0}".format(parent)
	if not interfaces is None:
		for interface in interfaces.split(","):
			s = interface.strip();
			parents = parents + ",\n	  public {0}".format(s)
	result = "class {0}\n{1} {{".format(classname, parents)
	if source.endswith("{"):
		result = result + "\nprotected:"
	return result

def proccessHeaderBrackets(source, context):
	if (source.strip() == "{"):
		return "protected:"
	elif (source.strip() == "}"):
		return "public:"
	elif (source.strip() == "@end"):
		return "};"
	return None

def convertDataType(datatype, funcname):
	if datatype == "BOOL":
		return "bool"
	if datatype == "id":
		if (not funcname is None and funcname.startswith("init")):
			return "bool"
		else:
			return "CCObject*"
	return datatype;

def convertMethodNoParam(line, classname):
	reg = re.compile('^([\+\-])\s*\((\S+)\)\s*(\w+)\s*\;?$')
	m = reg.match(line)	
	if m is None:
		return None	
	mark = m.group(1)
	resulttype = m.group(2)
	name = m.group(3)
	resulttype = convertDataType(resulttype, name)
	if not classname is None:
		name = "{0}::{1}".format(classname, name)
	result = "{0} {1}()".format(resulttype, name)
	if mark == "+":
		result = "static " + result;
	return result;

def convertMethodParam(line, classname, join_name):
	paramRegExp = '\s*(\w+)\s*\:\s*\((\S+)\)\s*(\w+)\s*'
	fullreg = re.compile('^([\+\-])\s*\((\S+)\)\s*(' + paramRegExp + ')+\;?$')
	m = fullreg.match(line)	
	if m is None:
		return None	
	mark = m.group(1)
	resulttype = m.group(2)
	funcname = ""
	funcparams = ""

	paramreg = re.compile(paramRegExp)
	f = paramreg.findall(line)
	for match in f:
		func = match[0]
		if join_name:
			if funcname != "":
				#funcname += "And"
				func = func.capitalize()
			funcname += func
		else:
			if funcname == "":
				funcname = func
		datatype = match[1]
		paramname = match[2]
		datatype = convertDataType(datatype, None)
		if funcparams != "":
			funcparams += ", "
		funcparams += "{0} {1}".format(datatype, paramname)

	resulttype = convertDataType(resulttype, funcname)
	if not classname is None:
		funcname = "{0}::{1}".format(classname, funcname)

	result = "{0} {1}({2})".format(resulttype, funcname, funcparams)
	if mark == "+":
		result = "static " + result;
	return result;

def convertMethodDeclaration(line, context):
	result = convertMethodNoParam(line, None)
	if not result is None:
		return "	" + result + ";"
	result = convertMethodParam(line, None, context.get("join_name"))
	if not result is None:
		return "	" + result + ";"
	return None

def convertMethodImplementation(line, context):
	result = convertMethodNoParam(line, context.get("classname"))
	if not result is None:
		return result
	result = convertMethodParam(line, context.get("classname"), context.get("join_name"))
	if not result is None:
		return result
	return None

def convertImplementation(line, context):
	if (line.strip() == "@end"):
		return ""

	reg = re.compile('^@implementation\s+(\w+)')
	m = reg.match(line)	
	if m is None:
		return None
	classname = m.group(1)	
	context["classname"] = classname
	return "// implementation {0}".format(classname)

def convertSynthesize(line, context):
	if (line.strip().startswith("@synthesize")):
		return "// " + line
	return None

def proccessLines(lines, transformations, context):
	result = ""
	for line in lines.split("\n"):
		if line.strip() == "":
			result = result + line + "\n"
		else:
			lineResult = line
			for transformation in transformations:
				s = transformation(lineResult, context)
				if not s is None:
					lineResult = s;
			if (lineResult is None):
					result = result + line + "\n"
			else:
				if (lineResult != ""):
					result = result + lineResult + "\n"
	return result	

class ConvertHeaderCommand(sublime_plugin.TextCommand):
	def run(self, edit):
		stipStartingComments(edit, self.view)
		region = sublime.Region(0, self.view.size())
		s = self.view.substr(region)
		context = dict()
		r = proccessLines(s, [convertProperty, importToIncludeLine, covnertClass, convertInterface, proccessHeaderBrackets, convertMethodDeclaration], context)
		if not r is None:
			if r == "":
				self.view.erase(edit, region)
			else:
				self.view.replace(edit, region, r)				
		addHeaderMacro(edit, self.view)

class ConvertImplementationCommand(sublime_plugin.TextCommand):
	def run(self, edit):
		stipStartingComments(edit, self.view)
		region = sublime.Region(0, self.view.size())
		s = self.view.substr(region)
		context = dict()
		r = proccessLines(s, [importToIncludeLine, convertImplementation, convertSynthesize, convertMethodImplementation], context)
		if not r is None:
			if r == "":
				self.view.erase(edit, region)
			else:
				self.view.replace(edit, region, r)

class ConvertMethodsCommand(sublime_plugin.TextCommand):
	def run(self, edit):
		for region in self.view.sel():  
			if not region.empty(): 
				s = self.view.substr(region)
				context = dict()
				r = proccessLines(s, [convertProperty, convertMethodDeclaration], context)
				if not r is None:
					if r == "":
						self.view.erase(edit, region)
					else:
						self.view.replace(edit, region, r)				