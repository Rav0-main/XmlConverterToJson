#pragma once

#ifndef ASSERTION_HPP
#define ASSERTION_HPP

#include "..\\..\\parsexml.hpp"

bool assertEqualParsingStatus(
	const ParsingStatus valid, const ParsingStatus result
);

void assertEqualRoots(
	const TagPtrSequence validRoots, const TagPtrSequence roots
);

void outputWrongVerdict(void);
void outputTrueVerdict(void);

#endif