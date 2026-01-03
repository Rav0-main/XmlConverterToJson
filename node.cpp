#include <queue>
#include <iostream>
#include <vector>
#include "node.hpp"

#define EMPTY L"EMPTY"

static void outputASCIIOf(const std::wstring& str);

void freeNode(Node* node, Node** nodePtr) {
	if (node == nullptr)
		return;

	for (auto& child : node->children)
		freeNode(child, &child);

	delete node;
	*nodePtr = nullptr;
}

void outputNodes(
	const NodePtrSequence& roots, const bool valueAsAscii
) {
	std::queue<const Node*> queue;
	const Node* node;

	for (const Node* root : roots) {
		queue.push(root);
		while (!queue.empty()) {
			node = queue.front();
			queue.pop();

			std::wcout << L"* Node: " << node->tagName << std::endl;

			std::wcout << L"Value: ";
			if (node->value.empty())
				std::wcout << EMPTY;
			else if (valueAsAscii)
				outputASCIIOf(node->value);
			else
				std::wcout << node->value;

			std::wcout << std::endl;

			std::wcout << "Childs: ";
			if (node->children.empty())
				std::wcout << EMPTY;

			else
				for (const Node* child : node->children) {
					std::wcout << child->tagName << L", ";
					queue.push(child);
				}

			std::wcout << std::endl;
		}
		std::wcout << std::endl;
	}
}

static void outputASCIIOf(const std::wstring& str) {
	std::wcout << std::hex;
	for (const int symb : str)
		std::wcout << "0x" << symb << " ";

	std::wcout << std::dec;
}