#include <fstream>
#include <cctype>
#include <iostream>
#include <queue>
#include "parsexml.hpp"

#define START_TAG_NAME '<'
#define END_TAG_NAME '>'
#define CLOSE_TAG_NAME '/'
#define EMPTY "EMPTY"

static void strip(std::string& str);
static bool isStartOfTagName(const char symbol);
static bool isEndOfTagName(const char symbol);
static bool isCloseOfTagName(const char symbol);
static void outputASCIIOf(const std::string& str);

ParsedXml getXmlTreesOf(const std::string& filename) {
	std::ifstream file(filename);
	if (!file)
		return ParsedXml(
			std::vector<Node*>(), ParsingResult::FileNotFound
		);

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

	while (!file.fail()) {
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
				stack.back()->children.push_back(node);
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

			strip(stack.back()->value);
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

	if (successParsed && file.eof())
		return ParsedXml( 
			{ roots, ParsingResult::Success }
		);
	else {
		for (Node* node : stack)
			freeNode(node, &node);

		for (Node* node : roots)
			freeNode(node, &node);

		return ParsedXml(
			{ std::vector<Node*>(), ParsingResult::UnknownError}
		);
	}
}

static void strip(std::string& str) {
	const size_t len = str.length();
	size_t left = 0;
	while (left < len && isspace(str[left]))
		++left;

	if (left == len) {
		str.clear();
		return;
	}

	size_t right = len - 1;
	while (isspace(str[right]))
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

void freeNode(Node* node, Node** nodePtr) {
	for (auto &child : node->children)
		freeNode(child, &child);

	delete node;
	*nodePtr = nullptr;
}

void outputTrees(
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