#pragma once

#ifndef PARSEXML_HPP

#define PARSEXML_HPP

#include <string>
#include "tag.hpp"

enum class ParsingStatusCode {
	Success,
	FileNotExistsError,
	WrongClosingTagNameError,
	WrongTagNameError,
	TagNameNotClosedError,
	TagAttributeNotClosedError,
	FileIsNotXmlError,
	UnknownError
};

struct ParsingStatus{
	const ParsingStatusCode code;
	/*
	* Information about ParsingStatusCode
	*/
	const std::wstring msg;
};

/*
* Parses xml file and places root nodes in roots
*/
ParsingStatus getXmlRootsOf(const std::string& filename, TagPtrSequence& roots);

#endif