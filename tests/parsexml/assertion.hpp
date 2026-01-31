#pragma once

#ifndef ASSERTION_HPP

#define ASSERTION_HPP

#include "parsexml.hpp"

bool assertEqualParsingStatusCode(
	const ParsingStatusCode valid, const ParsingStatusCode result
);

void assertEqualRoots(
	const TagPtrSequence validRoots, const TagPtrSequence roots
);

inline void outputWrongVerdict(void);
void outputTrueVerdict(void);

#endif