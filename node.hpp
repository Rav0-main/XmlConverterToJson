#pragma once

#ifndef NODE_HPP

#define NODE_HPP

#include <string>
#include <vector>

struct Node {
	std::string tagName;
	std::string value;
	std::vector<Node*> children;
};

#endif