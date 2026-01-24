#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <string>
#include "tag.hpp"

enum ParsingStatusCode {
	Success,
	FileNotExistsError,
	WrongClosingTagNameError,
	WrongTagNameError,
	TagNameNotClosedError,
	FileIsNotXmlError,
	UnknownError
};

struct ParsingStatus {
	const ParsingStatusCode code;
	const std::wstring msg;
};

ParsingStatus getXmlRootsOf(const std::string& filename, TagPtrSequence& roots);

#endif