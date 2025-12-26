#include "parsexml.hpp"

#ifndef fstream
#include <fstream>
#endif

#ifndef cctype
#include <cctype>
#endif

#ifndef iostream
#include <iostream>
#endif

#ifndef queue
#include <queue>
#endif

#define START_TAG_NAME '<'
#define END_TAG_NAME '>'
#define CLOSE_TAG_NAME '/'

static void deleteAllSpaceSymbolsIn(std::string& str);
static bool isStartOfTagName(const char symbol);
static bool isEndOfTagName(const char symbol);
static bool isCloseOfTagName(const char symbol);

ParsedXml getXmlTreesOf(const std::string filename) {
	std::ifstream file(filename);
	if (!file)
		return { {}, ParsingResult::FileNotFound };

	char symbol;
	file.get(symbol);

	bool inTagNameInit = false;
	bool inTagContent = false;
	bool inTagNameClose = false;
	bool successParsed = true;
	Node* node;
	std::vector<Node*> stack;
	std::vector<Node*> roots;
	std::string tagName;

	while (!file.eof()) {
		//in tag found space symbol
		if (isspace(symbol) && inTagNameInit) {
			successParsed = false;
			break;
		}
		//in tag found start of new tag
		else if (isStartOfTagName(symbol) && inTagNameInit) {
			successParsed = false;
			break;
		}
		//<
		else if (isStartOfTagName(symbol) && !inTagNameInit) {
			inTagNameInit = true;
		}
		//</
		else if (isCloseOfTagName(symbol) && inTagNameInit) {
			inTagNameInit = false;
			inTagNameClose = true;
		}
		//<...>
		else if (isEndOfTagName(symbol) && inTagNameInit) {
			inTagNameInit = false;
			inTagContent = true;
			node = new Node;
			node->tagName = tagName;
			if (!stack.empty()) {
				stack.back()->childs.push_back(node);
				stack.push_back(node);
			}
			else {
				stack.push_back(node);
				roots.push_back(node);
			}

			tagName.clear();
		}
		//</...>
		else if (isEndOfTagName(symbol) && inTagNameClose) {
			inTagContent = false;
			inTagNameInit = false;
			inTagNameClose = false;

			deleteAllSpaceSymbolsIn(stack.back()->value);
			stack.pop_back();
		}
		else if (inTagNameClose) {
			;
		}
		else if (inTagNameInit && !inTagNameClose) {
			tagName.push_back(symbol);
		}
		else if (inTagContent) {
			stack.back()->value.push_back(symbol);
		}

		file.get(symbol);
	}

	file.close();

	if (successParsed)
		return { roots, ParsingResult::Success };
	else
		return { {}, ParsingResult::UnknownError };
}

static void deleteAllSpaceSymbolsIn(std::string& str) {
	const size_t len = str.length();
	size_t left = 0;
	while (left < len && isspace(str[left]))
		++left;

	if (left == len)
		return;

	long long right = len - 1;
	while (right > 0 && isspace(str[right]))
		--right;

	str = str.substr(left, right - left + 1);
}

static bool isStartOfTagName(const char symbol) {
	return symbol == START_TAG_NAME;
}

static bool isEndOfTagName(const char symbol) {
	return symbol == END_TAG_NAME;
}

static bool isCloseOfTagName(const char symbol) {
	return symbol == CLOSE_TAG_NAME;
}

void outputRoots(const std::vector<Node*> roots) {
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
				std::cout << "Empty";
			else
				std::cout << node->value;

			std::cout << std::endl;

			std::cout << "Childs: ";
			for (Node* childNode : node->childs) {
				std::cout << childNode->tagName << ", ";
				queue.push(childNode);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}