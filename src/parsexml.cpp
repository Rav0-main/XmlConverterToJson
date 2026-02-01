#include <fstream>
#include <cctype>
#include "parsexml.hpp"

static const wchar_t START_TAG_NAME = L'<';
static const wchar_t END_TAG_NAME = L'>';
static const wchar_t CLOSING_TAG_NAME = L'/';

inline static void strip(std::wstring& str);
inline static void toWstringFrom(const std::string src, std::wstring& dist);
inline static std::wstring LineAndSymbolNumbersMsg(const size_t line, const size_t symbol);
inline static bool isInformationTag(const wchar_t firstSymbol);
inline static bool isStartOfTagName(const wchar_t symbol);
inline static bool isEndOfTagName(const wchar_t symbol);
inline static bool isClosingTagName(const wchar_t symbol);
inline static bool isTagAttributeSymbol(const wchar_t symbol);

ParsingStatus getXmlRootsOf(const std::string& filename, TagPtrSequence& roots) {
	std::wifstream file(filename);

	if (!file) {
		std::wstring wFilename;
		toWstringFrom(filename, wFilename);

		return ParsingStatus(
			ParsingStatusCode::FileNotExistsError,
			L"File: \"" + wFilename + L"\" not found."
		);
	}
	
	wchar_t symbol;
	symbol = file.get();

	bool inTagNameInit = false;
	bool inTagNameFoundSpace = false;
	bool inTagContent = false;
	bool inClosingTagName = false;
	bool inSingleTagName = false;
	bool inTagAttribute = false;

	bool foundOneValidTag = false;

	ParsingStatusCode statusCode = ParsingStatusCode::Success;
	std::wstring outMsg;
	
	size_t symbolNumber = 1;
	size_t currentLine = 1;

	Tag* currentTag;
	TagPtrSequence stack;
	std::wstring tagName;

	while (!file.fail()) {
		//<[/]...<
		if (isStartOfTagName(symbol) &&
			(inTagNameInit || inClosingTagName) && !inTagAttribute) {

			statusCode = ParsingStatusCode::WrongTagNameError;
			outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
				L"\nTag which starting on \"" + tagName + L"\" has not valid name.";
			break;
		}
		//<[/]abc ...
		else if (iswspace(symbol) && (inTagNameInit || inClosingTagName)) {
			inTagNameFoundSpace = true;

			if (tagName.empty()) {
				statusCode = ParsingStatusCode::WrongTagNameError;
				outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
					L"\nTag have space(s) before name.";
				break;
			}
		}
		//<
		else if (isStartOfTagName(symbol)) {
			inTagNameInit = true;
		}
		//[...] / [...]
		else if(isClosingTagName(symbol)) {
			//</...
			if (inTagNameInit && tagName.empty()) {
				inTagNameInit = false;
				inClosingTagName = true;
			}
			//<... /
			else if (inTagNameInit) {
				if (!inTagAttribute)
					inSingleTagName = true;
			}
			//</.../
			else if (inClosingTagName) {
				statusCode = ParsingStatusCode::WrongTagNameError;
				outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
					L"\nNot correct single tag name.";
				
				break;
			}
			else
				stack.back()->value.push_back(symbol);
		}
		//<...>
		else if (isEndOfTagName(symbol) && inTagNameInit) {
			inTagNameInit = false;
			inTagNameFoundSpace = false;
			inTagContent = !inSingleTagName;

			if (tagName.empty()) {
				statusCode = ParsingStatusCode::WrongTagNameError;
				outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
					L"\nEmpty tag name.";

				break;
			}

			else if (isInformationTag(tagName.front()) && !inTagAttribute) {
				foundOneValidTag = true;
				inTagContent = false;
			}

			else if (inTagAttribute) {
				statusCode = ParsingStatusCode::TagAttributeNotClosedError;
				outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
					L"\nAttribute of \"" + tagName + L"\" not closed.";

				break;
			}

			else if (inSingleTagName) {
				inSingleTagName = false;
				foundOneValidTag = true;
				if (!stack.empty())
					inTagContent = true;
			}

			else {
				foundOneValidTag = true;
				currentTag = new Tag;
				currentTag->name = tagName;

				if (!stack.empty()) {
					stack.back()->children.push_back(currentTag);
					stack.push_back(currentTag);
				}
				else {
					stack.push_back(currentTag);
					roots.push_back(currentTag);
				}
			}

			tagName.clear();
		}
		//</...>
		else if (isEndOfTagName(symbol) && inClosingTagName) {
			inTagNameFoundSpace = false;
			inTagContent = false;
			inClosingTagName = false;

			if (stack.empty()) {
				statusCode = ParsingStatusCode::WrongClosingTagNameError;
				outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
					L"Unknown closing tag name: \"" + tagName + L"\".";

				break;
			}
			else if (tagName == stack.back()->name) {
				strip(stack.back()->value);
				stack.pop_back();
				tagName.clear();

				if (!stack.empty())
					inTagContent = true;
			}
			else {
				statusCode = ParsingStatusCode::WrongClosingTagNameError;
				outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
					L"\nOpening tag name: \"" + stack.back()->name + \
					L"\" not equals closing: \"" + tagName + L"\".";

				break;
			}
		}
		//<[/]...
		else if ((inTagNameInit || inClosingTagName) && !inTagNameFoundSpace) {
			tagName.push_back(symbol);
		}
		//<... attr="...
		else if (inTagNameInit && isTagAttributeSymbol(symbol))
			inTagAttribute = !inTagAttribute;
		//skip tag attributes
		else if ((inTagNameInit || inClosingTagName) && inTagNameFoundSpace)
			;
		//<...>...
		else if (inTagContent) {
			stack.back()->value.push_back(symbol);
		}
		else if (iswspace(symbol))
			;
		//not excepted symbol in xml tree
		else {
			statusCode = ParsingStatusCode::FileIsNotXmlError;
			outMsg = L"File is not xml-format.\n" + \
							LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
							L"\nUnexpected symbol \"" + symbol + L"\".";
			break;
		}

		if (symbol == L'\n') {
			++currentLine;
			symbolNumber = 0;
		}
		else
			++symbolNumber;

		symbol = file.get();
	}

	file.close();

	if (statusCode == ParsingStatusCode::Success && file.eof()
		&& stack.empty() && foundOneValidTag)
		return ParsingStatus(
			ParsingStatusCode::Success
		);
	else {
		if (!stack.empty() && statusCode == ParsingStatusCode::Success) {
			statusCode = ParsingStatusCode::TagNameNotClosedError;
			outMsg = L"Tag name: \"" + stack.front()->name + L"\" not closed.";
		}
		else if (statusCode == ParsingStatusCode::Success && !foundOneValidTag) {
			statusCode = ParsingStatusCode::FileIsNotXmlError;
			outMsg = L"File is not xml-format or is empty file.";
		}
		else if (statusCode == ParsingStatusCode::Success && !file.eof()) {
			statusCode = ParsingStatusCode::UnknownError;
			outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
				L"An unknown error has occurred.";
		}

		if(!stack.empty())
			//dfs-free of current root
			freeTag(stack.front(), &(stack.front()));

		if (!roots.empty()) {
			roots.pop_back();

			//last root is freed (upper)
			for (Tag*& root : roots)
				freeTag(root, &root);

		}
		return ParsingStatus(
			statusCode,
			outMsg
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

static void toWstringFrom(const std::string src, std::wstring& dist) {
	dist.reserve(src.size());
	for (const char symb : src)
		dist.push_back(static_cast<wchar_t>(symb));
}

static std::wstring LineAndSymbolNumbersMsg(const size_t line, const size_t symbol) {
	return L"Line: " + std::to_wstring(line) + \
			 L", symbol: " + std::to_wstring(symbol) + L".";
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
	return symbol == CLOSING_TAG_NAME;
}

inline static bool isTagAttributeSymbol(const wchar_t symbol) {
	return symbol == L'\'' || symbol == L'\"';
}