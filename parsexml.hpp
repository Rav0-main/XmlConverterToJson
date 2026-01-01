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

#endif