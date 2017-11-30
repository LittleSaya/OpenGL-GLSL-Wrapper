#pragma once
#include "GLSLSrc.h"
#include "SubClass.h"

#include <vector>
#include <string>

class GLSLClassBuilder
{
public:
	static const std::string className;

private:
	const std::vector<GLSLSrc> &GLSLSrcVec;
	std::vector<SubClass> subClassVec;
	std::vector<std::string> GLSL_h;
	std::vector<std::string> GLSL_cpp;

public:
	GLSLClassBuilder(const std::vector<GLSLSrc> &p_GLSLSrcVec): GLSLSrcVec(p_GLSLSrcVec)
	{ }

public:
	auto build()			-> bool								{ return (buildClasses() && buildBuffer()); }
	auto getH()				-> const std::vector<std::string>&	{ return GLSL_h; }
	auto getCpp()			-> const std::vector<std::string>&	{ return GLSL_cpp; }

private:
	auto buildClasses()		-> bool;
	auto buildBuffer()		-> bool;

	// returns std::numeric_limits<std::size_t>::max() on failure
	auto searchInSubClass(const std::string &cname)	->	std::size_t;
};
