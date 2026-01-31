#pragma once

#ifndef TOJSON_HPP

#define TOJSON_HPP

#include "tag.hpp"

enum class ConvertingStatusCode {
	Success,
	NotOpenedFileError,
	UnknownError
};

struct ConvertingStatus {
	const ConvertingStatusCode code;
	const std::string msg;
};

struct ConvertingProfile {
	/*
	Example:
	XML:
		<list>
			<el>first</el>
			<el>second</el>
			<el>third</el>
		</list>
	1) if == true:
	JSON: 
	"list" : [
		"first",
		"second",
		"third"
	]
	2) else ( == false):
	JSON:
	"list" : {
		"el" : [
				"first",
				"second",
				"third"
		]
	}
	*/
	bool notNamesOneTagArray;
};

inline void setConvertingProfile(ConvertingProfile& newProfile);
inline ConvertingProfile* getConvertingProfile(void);

ConvertingStatus convertToJson(
	const TagPtrSequence& roots, const std::string& filename
);

#endif