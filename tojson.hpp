#pragma once

#ifndef TOJSON_HPP

#define TOJSON_HPP
#include "node.hpp"

void convertToJson(
	const std::string& filename, const std::vector<Node*>& trees
);

#endif