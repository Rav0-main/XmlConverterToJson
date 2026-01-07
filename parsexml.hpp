#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <string>
#include "node.hpp"

enum class ParsingStatus {
	Success,
	FileNotExistsError,
	WrongClosingTagNameError,
	WrongTagNameError,
	TagNameNotClosedError,
	UnknownError
};

struct ParsedXml {
	const ParsingStatus status;
};

ParsedXml getXmlRootsOf(const std::string& filename, NodePtrSequence& roots);

#endif