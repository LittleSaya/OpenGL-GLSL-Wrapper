#pragma once
#include <vector>
#include <string>

class SubClass
{
private:
	std::string subClassName;
	std::vector<std::size_t> memberVec;

public:
	SubClass(const std::string &p_subClassName, const std::size_t initMember):
		subClassName(p_subClassName), memberVec(1, initMember)
	{ }

	auto getName() const		-> const std::string&				{ return subClassName; }
	auto getVec() const			-> const std::vector<std::size_t>&	{ return memberVec; }
	auto addMem(std::size_t i)	-> void								{ memberVec.emplace_back(i); }
};