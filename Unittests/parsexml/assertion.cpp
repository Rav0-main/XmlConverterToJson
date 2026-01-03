#include <iostream>
#include <queue>
#include <string>
#include "assertion.hpp"

#define OK_LINE L"OK"
#define WRONG_LINE L"ERROR"
#define VERDICT L"Verdict"
#define EMPTY_LINE L"EMPTY"

static bool checkAtEqualNodes(
	const Node* const validNode, const Node* const node
);
static void outputTagNamesOfNodeVector(const NodePtrSequence& vect);

bool assertEqualParsingResult(
	const ParsingResult valid, const ParsingResult result
) {
	if (valid != result) {
		std::wcout << L"Wrong code of parsing result: " << int(result) << std::endl;
		std::wcout << L"Excepted: " << int(valid) << std::endl;
		outputWrongVerdict();
		return false;
	}

	return true;
}

void assertEqualRoots(
	const NodePtrSequence validRoots, const NodePtrSequence roots
) {
	if (validRoots.size() == roots.size()) {
		bool assertion = true;
		bool isEqualRoot = true;
		for (int i = 0; i < validRoots.size(); ++i) {
			isEqualRoot = checkAtEqualNodes(validRoots[i], roots[i]);
			assertion = isEqualRoot && assertion;
			if (!isEqualRoot)
				std::wcout << std::endl;
		}
		if (assertion)
			outputTrueVerdict();
		else
			outputWrongVerdict();
	}
	else {
		std::wcout << L"Wrong tree count: " << roots.size() << std::endl;
		std::wcout << L"But excepted: " << validRoots.size() << std::endl;
		outputWrongVerdict();
	}
}

static bool checkAtEqualNodes(
	const Node* const validRoot, const Node* const root
) {
	std::queue<const Node*> validQueue;
	std::queue<const Node*> queue;

	std::queue<std::wstring> paths;
	paths.push(L"");

	validQueue.push(validRoot);
	queue.push(root);

	bool run = true;
	const Node* validNode;
	const Node* node;
	while (run && !validQueue.empty() && !queue.empty()) {
		validNode = validQueue.front();
		validQueue.pop();

		node = queue.front();
		queue.pop();

		if (validNode->tagName != node->tagName) {
			std::wcout << L"Node with path: \"" << paths.front() << L"/"
				<< node->tagName
				<< L"\" not equal tagNames!" << std::endl;

			std::wcout << L"Truth tagName: " << validNode->tagName << L"\n";
			std::wcout << L"But taken: " << node->tagName << L"\n";
			return false;
		}
		else if (validNode->value != node->value) {
			std::wcout << L"Node with path: \"" << paths.front() << L"/"
				<< node->tagName
				<< L"\" not equal values!" << std::endl;

			std::wcout << L"Truth value: " << validNode->value << L"\n";
			std::wcout << L"But taken: " << node->value << L"\n";
			return false;
		}
		else if (validNode->children.size() != node->children.size()) {
			std::wcout << L"Node with path: \"" << paths.front() << L"/"
				<< node->tagName
				<< L"\" not equal children size!" << std::endl;

			std::wcout << L"Truth value: " << validNode->children.size() << L"\n";
			std::wcout << L"But taken: " << node->children.size() << L"\n";

			std::wcout << L"Valid children: ";
			outputTagNamesOfNodeVector(validNode->children);
			std::wcout << L"But taken: ";
			outputTagNamesOfNodeVector(node->children);
			return false;
		}

		for (int i = 0; i < validNode->children.size(); ++i) {
			paths.push(paths.front() + L"/" + validNode->tagName);
			validQueue.push(validNode->children[i]);
			queue.push(node->children[i]);
		}

		paths.pop();
	}

	return true;
}

static void outputTagNamesOfNodeVector(const NodePtrSequence& vect) {
	if (vect.size())
		for (const Node* node : vect)
			std::wcout << node->tagName << L", ";
	else
		std::wcout << EMPTY_LINE;

	std::wcout << std::endl;
}

void outputWrongVerdict(void) {
	std::wcout << VERDICT << L": " << WRONG_LINE << std::endl;
}

void outputTrueVerdict(void) {
	std::wcout << VERDICT << L": " << OK_LINE << std::endl;
}