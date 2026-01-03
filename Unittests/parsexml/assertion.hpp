#pragma once

#ifndef ASSERTION_HPP
#define ASSERTION_HPP

#include "..\\..\\parsexml.hpp"

bool assertEqualParsingResult(
	const ParsingResult valid, const ParsingResult result
);

void assertEqualRoots(
	const NodePtrSequence validRoots, const NodePtrSequence roots
);

void outputWrongVerdict(void);
void outputTrueVerdict(void);

#endif