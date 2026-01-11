#include <queue>
#include <iostream>
#include "tag.hpp"

#define EMPTY L"EMPTY"

static void outputASCIIOf(const std::wstring& str);

void freeTag(Tag* tag, Tag** tagPtr) {
	if (tag == nullptr)
		return;

	for (auto& child : tag->children)
		freeTag(child, &child);

	delete tag;
	*tagPtr = nullptr;
}

void outputTags(
	const TagPtrSequence& roots, const bool valueAsAscii
) {
	std::queue<const Tag*> queue;
	const Tag* tag;

	for (const Tag* root : roots) {
		queue.push(root);
		while (!queue.empty()) {
			tag = queue.front();
			queue.pop();

			std::wcout << L"* Tag: " << tag->name << std::endl;

			std::wcout << L"Value: ";
			if (tag->value.empty())
				std::wcout << EMPTY;
			else if (valueAsAscii)
				outputASCIIOf(tag->value);
			else
				std::wcout << tag->value;

			std::wcout << std::endl;

			std::wcout << "Childs: ";
			if (tag->children.empty())
				std::wcout << EMPTY;

			else
				for (const Tag* child : tag->children) {
					std::wcout << child->name << L", ";
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