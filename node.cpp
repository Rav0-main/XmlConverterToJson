#include <queue>
#include <iostream>
#include <vector>
#include "node.hpp"

#define EMPTY "EMPTY"

static void outputASCIIOf(const std::string& str);

void freeNode(Node* node, Node** nodePtr) {
	if (node == nullptr)
		return;

	for (auto& child : node->children)
		freeNode(child, &child);

	delete node;
	*nodePtr = nullptr;
}

void outputNodes(
	const std::vector<Node*>& roots, const bool valueAsAscii
) {
	std::queue<const Node*> queue;
	const Node* node;

	for (const Node* root : roots) {
		queue.push(root);
		while (!queue.empty()) {
			node = queue.front();
			queue.pop();

			std::cout << "* Node: " << node->tagName << std::endl;

			std::cout << "Value: ";
			if (node->value.empty())
				std::cout << EMPTY;
			else if (valueAsAscii)
				outputASCIIOf(node->value);
			else
				std::cout << node->value;

			std::cout << std::endl;

			std::cout << "Childs: ";
			if (node->children.empty())
				std::cout << EMPTY;

			else
				for (const Node* child : node->children) {
					std::cout << child->tagName << ", ";
					queue.push(child);
				}

			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

static void outputASCIIOf(const std::string& str) {
	std::cout << std::hex;
	for (const int symb : str)
		std::cout << "0x" << symb << " ";

	std::cout << std::dec;
}