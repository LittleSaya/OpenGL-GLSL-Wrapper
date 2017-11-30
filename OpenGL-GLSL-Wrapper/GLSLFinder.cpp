#include "GLSLFinder.h"

#include <string>

const std::string GLSLFinder::targetFileType("?*.?*.glsl");

auto GLSLFinder::find() -> bool
{
	findFiles();
	if (fileList.size() == 0)
	{
		std::cout << "GLSLFinder: find no file" << std::endl;
		return false;
	}
	extractNameType();
	std::string errFile = readSourceCode();
	if (sourceCodeList.size() == 0)
	{
		std::cout << "GLSLFinder: error when reading file: \"" + errFile + "\"" << std::endl;
		return false;
	}
	// fill the GLSLSrcVec
	for (std::size_t i = 0; i < nameTypeList.size(); ++i)
	{
		GLSLSrcVec.emplace_back(nameTypeList[i].first, nameTypeList[i].second, sourceCodeList[i]);
	}
	return true;
}

auto GLSLFinder::findFiles() -> void
{
	// create search target
	char cwd[_MAX_PATH] = { '\0' };
	_getcwd(cwd, _MAX_PATH);
	std::string target(cwd);
	if (target.back() != '\\') { target.append("\\"); }
	target.append(targetFileType);

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(target.data(), &findData);
	if (-1 == handle)
	{
		std::cout << "fail to find first file: \"" << target << "\"" << std::endl;
	}
	else
	{
		do
		{
			auto attrib = findData.attrib;
			if (!(_A_SYSTEM & attrib || _A_SUBDIR & attrib))
			{
				fileList.emplace_back(findData.name);
			}
		} while (_findnext(handle, &findData) == 0);
		_findclose(handle);
	}
}

auto GLSLFinder::extractNameType() -> void
{
	for (const auto &file : fileList)
	{
		// if file is 'abc.xy.glsl'
		// name = abc, type = xy
		std::size_t dotIdxR = file.rfind('.');
		std::size_t dotIdxL = file.rfind('.', dotIdxR - 1);
		std::string name(file.substr(0, dotIdxL));
		std::string type(file.substr(dotIdxL + 1, dotIdxR - dotIdxL - 1));
		// fix the 'name' and the 'type'
		nameFix(name);
		nameFix(type);
		for (auto &ch : type) { ch = std::toupper(ch); }
		// emplace a name-type pair
		nameTypeList.emplace_back(name, type);
		std::cout << "dotIdxL=" << dotIdxL << ", dotIdxR=" << dotIdxR << std::endl;
		std::cout << "extract " << file << " -> name=" << name << ", type=" << type << std::endl;
	}
}

inline auto GLSLFinder::readSourceCode() -> std::string
{
	for (const auto &file : fileList)
	{
		std::ifstream fin(file);
		if (!fin.is_open())
		{
			sourceCodeList.clear();
			return file;
		}
		std::vector<std::string> text;
		std::string line;
		while (std::getline(fin, line)) { text.emplace_back(line); }
		sourceCodeList.emplace_back(text);
		fin.close();
	}
	return std::string();
}

auto GLSLFinder::nameFix(std::string &str) -> void
{
	if (str.size() == 0 || (!std::isalpha(str[0]) && '_' != str[0])) { str.insert(0, 1, '_'); }// name starts with a letter or an underline
	for (auto &ch : str) { if (!std::isdigit(ch) && !std::isalpha(ch) && '_' != ch) { ch = '_'; } }// only 0~9, a~z, A~Z, and '_' are valid
}
