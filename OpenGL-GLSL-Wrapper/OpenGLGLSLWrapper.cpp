#include "GLSLFinder.h"
#include "GLSLClassBuilder.h"

#include <iostream>
#include <vector>
#include <fstream>

// 把当前文件夹下的所有GLSL代码全部封装在一个类里

bool writeFile(const std::string &filename, const std::vector<std::string> &content);

int main(int argc, char **argv)
{
	// search
	GLSLFinder glslFinder;
	if (!glslFinder.find())
	{
		std::cout << "fail: \"glslFinder.find() == false\"" << std::endl;
		return 0;
	}
	// build the class
	GLSLClassBuilder glslBuilder(glslFinder.getSrc());
	if (!glslBuilder.build())
	{
		std::cout << "fail: \"glslBuilder.build() == false\"" << std::endl;
		return 0;
	}
	// write out
	if (!writeFile(GLSLClassBuilder::className + std::string(".h"), glslBuilder.getH()) ||
		!writeFile(GLSLClassBuilder::className + std::string(".cpp"), glslBuilder.getCpp()))
	{
		std::cout << "fail: \"writeFile(...) == false\"" << std::endl;
		return 0;
	}
	return 0;
}

bool writeFile(const std::string & filename, const std::vector<std::string>& content)
{
	std::ofstream fout(filename);
	if (!fout.is_open())
	{
		std::cout << "fail to open file: \"" << filename << "\"" << std::endl;
		return false;
	}
	for (const auto &line : content)
	{
		fout << line << '\n';
	}
	fout << std::flush;
	fout.close();
	return true;
}
