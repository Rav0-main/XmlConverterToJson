#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <string>
#include "node.hpp"

enum class ParsingResult {
	Success,
	FileNotExistsError,
	WrongClosingTagNameError,
	WrongTagNameError,
	TagNameNotClosedError,
	UnknownError
};

struct ParsedXml {
	const ParsingResult result;
};

ParsedXml getXmlRootsOf(const std::string& filename, NodePtrSequence& roots);

#endif