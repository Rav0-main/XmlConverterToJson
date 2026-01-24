#include <fstream>
#include <cctype>
#include "parsexml.hpp"

inline static const wchar_t START_TAG_NAME = L'<';
inline static const wchar_t END_TAG_NAME = L'>';
inline static const wchar_t CLOSING_TAG_NAME = L'/';

static void strip(std::wstring& str);
inline void toWstringFrom(const std::string src, std::wstring& dist);
inline std::wstring LineAndSymbolNumbersMsg(const size_t line, const size_t symbol);
inline static bool isInformationTag(const wchar_t firstSymbol);
inline static bool isStartOfTagName(const wchar_t symbol);
inline static bool isEndOfTagName(const wchar_t symbol);
inline static bool isClosingTagName(const wchar_t symbol);

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
			(inTagNameInit || inClosingTagName)) {

			statusCode = ParsingStatusCode::WrongTagNameError;
			outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
				L"\nTag starting with \"" + tagName + L"\" have not valid name.";
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
			inTagContent = false;
			inTagNameInit = false;
			inClosingTagName = false;

			if (tagName == stack.back()->name && !inTagNameFoundSpace) {
				strip(stack.back()->value);
				stack.pop_back();
				tagName.clear();
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
			outMsg = LineAndSymbolNumbersMsg(currentLine, symbolNumber) + \
						   L"\n. Symbol \"" + symbol + L"\" not correct for xml-format.";
			break;
		}

		if (symbol == L'\n')
			++currentLine;
		
		++symbolNumber;
		symbol = file.get();
	}

	file.close();

	if (statusCode == ParsingStatusCode::Success && file.eof() && stack.empty())
		return ParsingStatus( 
			ParsingStatusCode::Success
		);
	else {
		if (!stack.empty() && statusCode == ParsingStatusCode::Success) {
			statusCode = ParsingStatusCode::TagNameNotClosedError;
			outMsg = L"Tag name: \"" + stack.front()->name + L"\" not closed.";
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

inline void toWstringFrom(const std::string src, std::wstring& dist) {
	dist.reserve(src.size());
	for (const char symb : src)
		dist.push_back(static_cast<wchar_t>(symb));
}

inline std::wstring LineAndSymbolNumbersMsg(const size_t line, const size_t symbol) {
	return L"Line: " + std::to_wstring(line) + \
			 L", symbol: " + std::to_wstring(symbol) + L".";
}

inline static bool isInformationTag(const wchar_t firstSymbol) {
	return firstSymbol == L'?' || firstSymbol == L'!';
}

inline static bool isStartOfTagName(const wchar_t symbol) {
	return symbol == START_TAG_NAME;
}

inline static bool isEndOfTagName(const wchar_t symbol) {
	return symbol == END_TAG_NAME;
}

inline static bool isClosingTagName(const wchar_t symbol) {
	return symbol == CLOSING_TAG_NAME;
}