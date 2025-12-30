#include <iostream>
#include <queue>
#include <string>
#include "assertion.hpp"

#define OK_LINE "OK"
#define WRONG_LINE "ERROR"
#define VERDICT "Verdict"
#define EMPTY_LINE "EMPTY"

static bool checkAtEqualNodes(
	const Node* const validTree, const Node* const tree
);
static void outputTagNamesOfNodeVector(const std::vector<Node*> vect);

bool assertEqualParsingResult(
	const ParsingResult valid, const ParsingResult result
) {
	if (valid != result) {
		std::cout << "Wrong code of parsing result: " << int(result) << std::endl;
		std::cout << "Excepted: " << int(valid) << std::endl;
		std::cout << VERDICT << ": " WRONG_LINE << std::endl;
		return false;
	}

	return true;
}

void assertEqualTrees(
	const std::vector<Node*> validTrees, const std::vector<Node*> trees
) {
	if (validTrees.size() == trees.size()) {
		bool assertion = true;
		bool isEqualTree = true;
		for (int i = 0; i < validTrees.size(); ++i) {
			isEqualTree = checkAtEqualNodes(validTrees[i], trees[i]);
			assertion = isEqualTree && assertion;
			if (!isEqualTree)
				std::cout << std::endl;
		}
		if (assertion)
			std::cout << VERDICT << ": " OK_LINE;
		else
			std::cout << VERDICT << ": " WRONG_LINE;

		std::cout << std::endl;
	}
	else {
		std::cout << "Wrong tree count: " << trees.size() << std::endl;
		std::cout << "But excepted: " << validTrees.size() << std::endl;
		std::cout << VERDICT << ": " << WRONG_LINE;
	}
}

static bool checkAtEqualNodes(
	const Node* const validTree, const Node* const tree
) {
	std::queue<const Node*> validQueue;
	std::queue<const Node*> queue;

	std::queue<std::string> paths;
	paths.push("");

	validQueue.push(validTree);
	queue.push(tree);

	bool run = true;
	const Node* validNode;
	const Node* node;
	while (run && !validQueue.empty() && !queue.empty()) {
		validNode = validQueue.front();
		validQueue.pop();

		node = queue.front();
		queue.pop();

		if (validNode->tagName != node->tagName) {
			std::cout << "Node with path: \"" << paths.front() << "/"
				<< node->tagName
				<< "\" not equal tagNames!" << std::endl;

			std::cout << "Truth tagName: " << validNode->tagName << "\n";
			std::cout << "But taken: " << node->tagName << "\n";
			return false;
		}
		else if (validNode->value != node->value) {
			std::cout << "Node with path: \"" << paths.front() << "/"
				<< node->tagName
				<< "\" not equal values!" << std::endl;

			std::cout << "Truth value: " << validNode->value << "\n";
			std::cout << "But taken: " << node->value << "\n";
			return false;
		}
		else if (validNode->children.size() != node->children.size()) {
			std::cout << "Node with path: \"" << paths.front() << "/"
				<< node->tagName
				<< "\" not equal children size!" << std::endl;

			std::cout << "Truth value: " << validNode->children.size() << "\n";
			std::cout << "But taken: " << node->children.size() << "\n";

			std::cout << "Valid children: ";
			outputTagNamesOfNodeVector(validNode->children);
			std::cout << "But taken: ";
			outputTagNamesOfNodeVector(node->children);
			return false;
		}

		for (int i = 0; i < validNode->children.size(); ++i) {
			paths.push(paths.front() + "/" + validNode->tagName);
			validQueue.push(validNode->children[i]);
			queue.push(node->children[i]);
		}

		paths.pop();
	}

	return true;
}

static void outputTagNamesOfNodeVector(const std::vector<Node*> vect) {
	if (vect.size())
		for (const Node* node : vect)
			std::cout << node->tagName << ", ";
	else
		std::cout << EMPTY_LINE;

	std::cout << std::endl;
}