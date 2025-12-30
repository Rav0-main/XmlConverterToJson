#pragma once

#ifndef ASSERTION_HPP
#define ASSERTION_HPP

#include "..\\..\\parsexml.hpp"

bool assertEqualParsingResult(
	const ParsingResult valid, const ParsingResult result
);

void assertEqualTrees(
	const std::vector<Node*> validTrees, const std::vector<Node*> trees
);

#endif