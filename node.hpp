#pragma once

#ifndef NODE_HPP

#define NODE_HPP

#include <string>
#include <deque>

struct Node {
	std::wstring tagName;
	std::wstring value;
	std::deque<Node*> children;
};

typedef std::deque<Node*> NodePtrSequence;

void freeNode(Node* node, Node** nodePtr);

void outputNodes(
	const NodePtrSequence& nodes, const bool valueAsAscii
);

#endif