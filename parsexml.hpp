#pragma once
#ifndef tuple
#include <tuple>
#endif

#ifndef vector
#include <vector>
#endif

#ifndef string
#include <string>
#endif

struct Node {
	std::string tagName;
	std::string value;
	std::vector<Node*> childs;
};

enum class ParsingResult {
	Success,
	FileNotFound,
	UnknownError
};

typedef std::tuple<std::vector<Node*>, ParsingResult> ParsedXml;

ParsedXml getXmlTreesOf(const std::string filename);

void outputRoots(const std::vector<Node*> roots);