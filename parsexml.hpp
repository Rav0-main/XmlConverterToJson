#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <tuple>
#include <vector>
#include <string>
#include "node.hpp"

enum class ParsingResult {
	Success,
	FileNotExistsError,
	WrongClosingTagNameError,
	WrongTagNameError,
	UnknownError
};

typedef std::tuple<std::vector<Node*>, ParsingResult> ParsedXml;

ParsedXml getXmlTreesOf(const std::string& filename);

//всё что связано с node вынести в другой файл
void freeNode(Node* node, Node** nodePtr);

void outputTrees(
	const std::vector<Node*>& roots, const bool valueAsAscii
);

#endif