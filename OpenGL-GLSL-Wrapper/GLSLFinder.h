#pragma once
#include "GLSLSrc.h"

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <cctype>

#include <io.h>
#include <direct.h>

// a functor which finds and returns every GLSL source code in the CWD
class GLSLFinder
{
public:
	static const std::string targetFileType;

private:
	std::vector<std::string> fileList;
	std::vector<std::pair<std::string, std::string>> nameTypeList;
	std::vector<std::vector<std::string>> sourceCodeList;
	std::vector<GLSLSrc> GLSLSrcVec;

public:
	auto find() -> bool;
	std::vector<GLSLSrc> &getSrc() { return GLSLSrcVec; }

private:
	// fill the 'fileList'
	auto findFiles() -> void;

	// fill the 'nameTypeList'
	auto extractNameType() -> void;

	// fill the 'sourceCodeList', read files
	// on failure:
	//		1. sourceCodeList.clear() will be called
	//		2. the name of the file on which errors occured will be returned
	auto readSourceCode() -> std::string;

	// make 'str' a valid class name
	auto nameFix(std::string &str) -> void;
};
