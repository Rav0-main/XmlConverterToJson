#pragma once

#ifndef NODE_HPP

#define NODE_HPP

#include <string>
#include <vector>

struct Node {
	std::wstring tagName;
	std::wstring value;
	std::vector<Node*> children;
};

void freeNode(Node* node, Node** nodePtr);

void outputNodes(
	const std::vector<Node*>& nodes, const bool valueAsAscii
);

#endif