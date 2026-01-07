#pragma once

#ifndef ASSERTION_HPP
#define ASSERTION_HPP

#include "..\\..\\parsexml.hpp"

bool assertEqualParsingStatus(
	const ParsingStatus valid, const ParsingStatus result
);

void assertEqualRoots(
	const NodePtrSequence validRoots, const NodePtrSequence roots
);

void outputWrongVerdict(void);
void outputTrueVerdict(void);

#endif