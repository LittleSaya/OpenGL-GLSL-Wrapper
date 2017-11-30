#include "GLSLClassBuilder.h"

#include <limits>
#include <iostream>

const std::string GLSLClassBuilder::className("GLSL");

auto GLSLClassBuilder::buildClasses() -> bool
{
	for (std::size_t iSrc = 0; iSrc < GLSLSrcVec.size(); ++iSrc)
	{
		std::cout << "processing: " << GLSLSrcVec.at(iSrc).srcType << "::" << GLSLSrcVec.at(iSrc).srcName;
		std::size_t iSub = searchInSubClass(GLSLSrcVec.at(iSrc).srcType);
		if (std::numeric_limits<std::size_t>::max() == iSub)// not found
		{
			std::cout << " new type" << std::endl;
			subClassVec.emplace_back(GLSLSrcVec[iSrc].srcType, iSrc);
		}
		else// found
		{
			std::cout << " old type" << std::endl;
			subClassVec[iSub].addMem(iSrc);
		}
	}
	return true;
}

auto GLSLClassBuilder::buildBuffer() -> bool
{
	// 1. emplace preprocessor
	GLSL_h.emplace_back("#pragma once");
	GLSL_h.emplace_back("#include <vector>");
	GLSL_h.emplace_back("#include <string>");
	GLSL_h.emplace_back("");

	// 2. emplace main class header
	GLSL_h.emplace_back(std::string("class ") + className);
	GLSL_h.emplace_back("{");
	GLSL_h.emplace_back("public:");

	// 3. emplace subclasses
	for (const auto &subClass : subClassVec)
	{
		// 1. emplace subclass header
		GLSL_h.emplace_back(std::string("    class ") + subClass.getName());
		GLSL_h.emplace_back("    {");
		GLSL_h.emplace_back("    public:");

		// 2. emplace subclass members
		for (const auto iSrc : subClass.getVec())
		{
			GLSL_h.emplace_back(std::string("        static const std::vector<std::string> ") + GLSLSrcVec[iSrc].srcName + std::string(";"));
			
			// add a 'const char*' string
			GLSL_h.emplace_back(std::string("        static const char *") + GLSLSrcVec[iSrc].srcName + "_c;");
		}

		// 3. emplace subclass end
		GLSL_h.emplace_back("    };");
	}

	// 4. emplace main class end
	GLSL_h.emplace_back("};");
	GLSL_h.emplace_back("");

	// 5. emplace members' implementations
	GLSL_cpp.emplace_back(std::string("#include \"") + className + std::string(".h\""));
	GLSL_cpp.emplace_back("");
	for (const auto &subClass : subClassVec)
	{
		for (const auto iSrc : subClass.getVec())
		{
			// c++ code
			{
				// 1. emplace the member's name outside of the class
				GLSL_cpp.emplace_back(std::string("const std::vector<std::string> ") +
					className + std::string("::") + GLSLSrcVec[iSrc].srcType + std::string("::") + GLSLSrcVec[iSrc].srcName);

				// 2. emplace the beginning left brace
				GLSL_cpp.emplace_back("{");

				// 3. emplace the content of the source code
				for (const auto &line : GLSLSrcVec[iSrc].srcText)
				{
					GLSL_cpp.emplace_back(std::string("    \"") + line + std::string("\\n\","));
				}

				// 4. emplace the ending right brace
				GLSL_cpp.emplace_back("};");
				GLSL_cpp.emplace_back("");
			}
			// c code
			{
				// 1. emplace the member's name outside of the class
				GLSL_cpp.emplace_back(std::string("const char *") +
					className + std::string("::") + GLSLSrcVec[iSrc].srcType + std::string("::") + GLSLSrcVec[iSrc].srcName + "_c =");

				// 2. emplace the content of the source code
				for (const auto &line : GLSLSrcVec[iSrc].srcText)
				{
					GLSL_cpp.emplace_back(std::string("\"") + line + std::string("\\n\""));
				}

				// 3. emplace the ending semicolon
				GLSL_cpp.emplace_back(";");
				GLSL_cpp.emplace_back("");
			}
		}
	}
	return true;
}

auto GLSLClassBuilder::searchInSubClass(const std::string & cname) -> std::size_t
{
	for (std::size_t i = 0; i < subClassVec.size(); ++i)
	{
		if (0 == cname.compare(subClassVec.at(i).getName())) { return i; }
	}
	return std::numeric_limits<std::size_t>::max();
}
