#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <string>
#include "tag.hpp"

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

ParsedXml getXmlRootsOf(const std::string& filename, TagPtrSequence& roots);

#endif