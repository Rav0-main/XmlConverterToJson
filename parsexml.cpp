#include <fstream>
#include <cctype>
#include "parsexml.hpp"

#define START_TAG_NAME L'<'
#define END_TAG_NAME L'>'
#define CLOSE_TAG_NAME L'/'

static void strip(std::wstring& str);
static bool isInformationTag(const wchar_t firstSymbol);
static bool isStartOfTagName(const wchar_t symbol);
static bool isEndOfTagName(const wchar_t symbol);
static bool isClosingTagName(const wchar_t symbol);

ParsedXml getXmlRootsOf(const std::string& filename, NodePtrSequence& roots) {
	std::wifstream file(filename);

	if (!file)
		return ParsedXml(
			ParsingStatus::FileNotExistsError
		);
	
	wchar_t symbol;
	symbol = file.get();

	bool inTagNameInit = false;
	bool inTagNameFoundSpace = false;
	bool inTagContent = false;
	bool inClosingTagName = false;
	ParsingStatus result = ParsingStatus::Success;

	Node* node;
	NodePtrSequence stack;
	std::wstring tagName;

	while (!file.fail()) {
		//<[/]...<
		if (isStartOfTagName(symbol) &&
			(inTagNameInit || inClosingTagName)) {

			result = ParsingStatus::WrongTagNameError;
			break;
		}
		//<[/]abc ...
		else if (iswspace(symbol) && (inTagNameInit || inClosingTagName)) {
			inTagNameFoundSpace = true;
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
			inTagNameFoundSpace = false;
			inTagContent = true;

			if (tagName.empty() || isInformationTag(tagName.front()))
				inTagContent = false;
			else {
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
			}

			tagName.clear();
		}
		//</...>
		else if (isEndOfTagName(symbol) && inClosingTagName) {
			inTagContent = false;
			inTagNameInit = false;
			inClosingTagName = false;

			if (tagName == stack.back()->tagName && !inTagNameFoundSpace) {
				strip(stack.back()->value);
				stack.pop_back();
				tagName.clear();
			}
			else {
				result = ParsingStatus::WrongClosingTagNameError;
				break;
			}
		}
		//<[/]...
		else if ((inTagNameInit || inClosingTagName) && !inTagNameFoundSpace) {
			tagName.push_back(symbol);
		}
		else if ((inTagNameInit || inClosingTagName) && inTagNameFoundSpace)
			;
		//<...>...
		else if (inTagContent) {
			stack.back()->value.push_back(symbol);
		}

		symbol = file.get();
	}

	file.close();

	if (!int(result) && file.eof() && stack.empty())
		return ParsedXml( 
			ParsingStatus::Success
		);
	else {
		if (!stack.empty() && !int(result))
			result = ParsingStatus::TagNameNotClosedError;

		if(!stack.empty())
			//dfs-free of current root
			freeNode(stack.front(), &(stack.front()));

		roots.pop_back();
		if (!roots.empty()) {

			//last root is freed (upper)
			for (Node*& root : roots)
				freeNode(root, &root);

		}
		return ParsedXml(
			result
		);
	}
}

static void strip(std::wstring& str) {
	const size_t len = str.length();
	size_t left = 0;
	while (left < len && iswspace(str[left]))
		++left;

	if (left == len) {
		str.clear();
		return;
	}

	size_t right = len - 1;
	while (iswspace(str[right]))
		--right;

	str = str.substr(left, right - left + 1);
}

static bool isInformationTag(const wchar_t firstSymbol) {
	return firstSymbol == L'?' || firstSymbol == L'!';
}

static bool isStartOfTagName(const wchar_t symbol) {
	return symbol == START_TAG_NAME;
}

static bool isEndOfTagName(const wchar_t symbol) {
	return symbol == END_TAG_NAME;
}

static bool isClosingTagName(const wchar_t symbol) {
	return symbol == CLOSE_TAG_NAME;
}