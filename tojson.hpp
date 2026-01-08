#pragma once

#ifndef TOJSON_HPP

#define TOJSON_HPP
#include "node.hpp"

enum class ConvertingStatus {
	Success,
};

struct ConvertingResult {
	const ConvertingStatus status;
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
	1) If == true:
	JSON: 
	"list" : [
		"first",
		"second",
		"third"
	]
	2) Else ( == false):
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

void setConvertingProfile(ConvertingProfile& newProfile);
ConvertingProfile* getConvertingProfile(void);

ConvertingResult convertToJson(
	const NodePtrSequence& roots, const std::string& filename
);

#endif