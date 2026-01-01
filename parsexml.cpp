#include <fstream>
#include <cctype>
#include <iostream>
#include <queue>
#include "parsexml.hpp"

#define START_TAG_NAME '<'
#define END_TAG_NAME '>'
#define CLOSE_TAG_NAME '/'

static void strip(std::string& str);
static bool isStartOfTagName(const char symbol);
static bool isEndOfTagName(const char symbol);
static bool isClosingTagName(const char symbol);

ParsedXml getXmlTreesOf(const std::string& filename) {
	std::ifstream file(filename);
	if (!file)
		return ParsedXml(
			std::vector<Node*>(), ParsingResult::FileNotExistsError
		);

	char symbol;
	symbol = file.get();

	bool inTagNameInit = false;
	bool inTagContent = false;
	bool inClosingTagName = false;
	ParsingResult result = ParsingResult::Success;

	Node* node;
	std::vector<Node*> stack;
	std::vector<Node*> roots;
	std::string tagName;

	while (!file.fail()) {
		if (isspace(symbol) && inTagNameInit) {
			result = ParsingResult::UnknownError;
			break;
		}
		//<[/]...<
		else if (isStartOfTagName(symbol) && 
			(inTagNameInit || inClosingTagName)) {

			result = ParsingResult::WrongTagNameError;
			break;
		}
		//<
		else if (isStartOfTagName(symbol)) {
			inTagNameInit = true;
		}
		//</
		else if (isClosingTagName(symbol) && inTagNameInit) {
			inTagNameInit = false;
			inClosingTagName = true;
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
		else if (isEndOfTagName(symbol) && inClosingTagName) {
			inTagContent = false;
			inTagNameInit = false;
			inClosingTagName = false;

			if (tagName == stack.back()->tagName) {
				strip(stack.back()->value);
				stack.pop_back();
				tagName.clear();
			}
			else {
				result = ParsingResult::WrongClosingTagNameError;
				break;
			}
		}
		//<...
		else if (inTagNameInit || inClosingTagName) {
			tagName.push_back(symbol);
		}
		//<...>...
		else if (inTagContent) {
			stack.back()->value.push_back(symbol);
		}

		symbol = file.get();
	}

	file.close();

	if (!int(result) && file.eof())
		return ParsedXml( 
			{ roots, ParsingResult::Success }
		);
	else {
		//dfs-free of current root
		freeNode(stack.front(), &(stack.front()));

		//last root is freed (upper)
		for (int i = 0; i < roots.size() - 1; ++i)
			freeNode(roots[i], &roots[i]);

		return ParsedXml(
			{ std::vector<Node*>(), result}
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

static bool isClosingTagName(const char symbol) {
	return symbol == CLOSE_TAG_NAME;
}