#pragma once
#include <vector>
#include <string>

struct GLSLSrc
{
	std::string srcName;
	std::string srcType;
	std::vector<std::string> srcText;

	GLSLSrc(const std::string &name, const std::string &type, const std::vector<std::string> &text) :
		srcName(name), srcType(type), srcText(text)
	{

	}
};