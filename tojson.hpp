#pragma once

#ifndef TOJSON_HPP

#define TOJSON_HPP
#include "node.hpp"

void convertToJson(
	const NodePtrSequence& roots, const std::string& filename
);

#endif