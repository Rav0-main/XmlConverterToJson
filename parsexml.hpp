#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <tuple>
#include <vector>
#include <string>

struct Node {
	std::string tagName;
	std::string value;
	std::vector<Node*> children;
};

enum class ParsingResult {
	Success,
	FileNotFound,
	UnknownError
};

typedef std::tuple<std::vector<Node*>, ParsingResult> ParsedXml;

ParsedXml getXmlTreesOf(const std::string& filename);

void freeNode(Node* node, Node** nodePtr);

void outputRoots(
	const std::vector<Node*>& roots, const bool valueAsAscii
);
#endif