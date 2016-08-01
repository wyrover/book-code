import sublime, sublime_plugin
import re
import uuid

def capitalizeFirstChar(str):
	return str[0:1].upper() + str[1:]

def prepareValue(value):
	if value == "self":
		return "this"
	return value

def convertMethodNameAndParams(source, join_name):
	reg = re.compile('([\S^\:]+)\s*\:\s*([\S^\:]+)\s*')
	f = reg.findall(source)
	if f is None:
		return None
	funcname = ""
	params = ""
	for item in f:
		name = item[0]
		value = item[1]
		if join_name:
			if funcname != "":
				name = capitalizeFirstChar(name)
			funcname += name
		else:
			if funcname == "":
				funcname = name
		if params != "":
			params += ", "
		params += prepareValue(value)
	return "{0}({1})".format(funcname, params)

def prepareObjectNameForCall(name):
	static = False
	if not "." in name and not "->" in name and not ":" in name and name[0:1].isupper():
		static = True
	if name == "self":
		name = "this"
	if static:
		return name + "::"
	else:
		return name + "->"

def covnertMethodCallBodyWithParams(source, join_name):
	reg = re.compile('^\s*(\S+)\s+(([\S^\:]+\s*\:\s*[\S^\:]+\s*)+)\s*$')
	m = reg.match(source)
	if m is None:
		return None
	objectName = m.group(1)
	nameAndParams = m.group(2)
	call = convertMethodNameAndParams(nameAndParams, join_name)
	if call is None:
		return None
	return "{0}{1}".format(prepareObjectNameForCall(objectName), call)

def covnertMethodCallBodyWithoutParams(source):
	reg = re.compile('^\s*(\S+)\s+(\w+)\s*$')
	m = reg.match(source)
	if m is None:
		return None
	objectName = m.group(1)
	methodName = m.group(2)
	return "{0}{1}()".format(prepareObjectNameForCall(objectName), methodName)

def convertMethodCall(source, join_name):
	replaces = []
	key_index = 0

	f = re.compile('\[([^\[\]]+)\]').findall(source)
	print f
	while f != []:
		for item in f:
			call = covnertMethodCallBodyWithParams(item, join_name)
			if call is None:
				call = covnertMethodCallBodyWithoutParams(item)
			if not call is None:
				key_value = "replace_key_123123123123_" + str(key_index)
				key_index += 1
				replaces.append(dict(key = key_value, value = call))
				source = source.replace("[" + item + "]", key_value + " ")
		f = re.compile('\[([^\[\]]+)\]').findall(source)
	replaces.append(dict(value = source))

	for i in range(len(replaces) - 1):
		key = replaces[i]["key"]
		value = replaces[i]["value"]
		for j in range(i, len(replaces)):
			replaces[j]["value"] = replaces[j]["value"].replace(key, value)

	result = replaces[len(replaces) - 1]["value"];
	result = result.replace(" ;", ";")
	return result;

class ConvertMethodCallCommand(sublime_plugin.TextCommand):
	def run(self, edit):
		for region in self.view.sel():  
			if not region.empty(): 
				source = self.view.substr(region) 
				result = convertMethodCall(source, True)
            	if not result is None:
            		self.view.replace(edit, region, result)

class ConvertMethodCallNoJoinCommand(sublime_plugin.TextCommand):
	def run(self, edit):
		for region in self.view.sel():  
			if not region.empty(): 
				source = self.view.substr(region) 
				result = convertMethodCall(source, False)
            	if not result is None:
            		self.view.replace(edit, region, result)            		