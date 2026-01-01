#pragma once

#ifndef TOJSON_HPP

#define TOJSON_HPP
#include "node.hpp"

void convertToJson(
	const std::vector<Node*>& trees, const std::string& filename
);

#endif