#include <iostream>
#include <queue>
#include <string>
#include "assertion.hpp"

static const std::wstring OK_LINE = L"OK";
static const std::wstring WRONG_LINE = L"ERROR";
static const std::wstring VERDICT = L"Verdict";
static const std::wstring EMPTY_LINE = L"EMPTY";

static bool checkAtEqualTags(
	const Tag* const validTag, const Tag* const tag
);
inline static void outputTagNamesOf(const TagPtrSequence& vect);

bool assertEqualParsingStatusCode(
	const ParsingStatusCode valid, const ParsingStatusCode result
) {
	if (valid != result) {
		std::wcout << L"Wrong code of parsing result: " << int(result) << std::endl;
		std::wcout << L"Excepted: " << int(valid) << std::endl;
		outputWrongVerdict();
		return false;
	}

	return true;
}

bool assertEqualRoots(
	const TagPtrSequence validRoots, const TagPtrSequence roots
) {
	if (validRoots.size() == roots.size()) {
		bool assertion = true;
		bool isEqualRoot = true;
		for (int i = 0; i < validRoots.size(); ++i) {
			isEqualRoot = checkAtEqualTags(validRoots[i], roots[i]);
			assertion = isEqualRoot && assertion;
			if (!isEqualRoot)
				std::wcout << std::endl;
		}
		if (assertion)
			outputTrueVerdict();
		else
			outputWrongVerdict();
	
		return assertion;
	}
	else {
		std::wcout << L"Wrong tree count: " << roots.size() << std::endl;
		std::wcout << L"But excepted: " << validRoots.size() << std::endl;
		outputWrongVerdict();

		return false;
	}
}

static bool checkAtEqualTags(
	const Tag* const validRoot, const Tag* const root
) {
	std::queue<const Tag*> validQueue;
	std::queue<const Tag*> queue;

	std::queue<std::wstring> paths;
	paths.push(L"");

	validQueue.push(validRoot);
	queue.push(root);

	bool run = true;
	const Tag* validTag;
	const Tag* tag;
	while (run && !validQueue.empty() && !queue.empty()) {
		validTag = validQueue.front();
		validQueue.pop();

		tag = queue.front();
		queue.pop();

		if (validTag->name != tag->name) {
			std::wcout << L"Tag with path: \"" << paths.front() << L"/"
				<< tag->name
				<< L"\" not equal tagNames!" << std::endl;

			std::wcout << L"Truth tagName: " << validTag->name << L"\n";
			std::wcout << L"But taken: " << tag->name << L"\n";
			return false;
		}
		else if (validTag->value != tag->value) {
			std::wcout << L"Tag with path: \"" << paths.front() << L"/"
				<< tag->name
				<< L"\" not equal values!" << std::endl;

			std::wcout << L"Truth value: " << validTag->value << L"\n";
			std::wcout << L"But taken: " << tag->value << L"\n";
			return false;
		}
		else if (validTag->children.size() != tag->children.size()) {
			std::wcout << L"Tag with path: \"" << paths.front() << L"/"
				<< tag->name
				<< L"\" not equal children size!" << std::endl;

			std::wcout << L"Truth value: " << validTag->children.size() << L"\n";
			std::wcout << L"But taken: " << tag->children.size() << L"\n";

			std::wcout << L"Valid children: ";
			outputTagNamesOf(validTag->children);
			std::wcout << L"But taken: ";
			outputTagNamesOf(tag->children);
			return false;
		}

		for (int i = 0; i < validTag->children.size(); ++i) {
			paths.push(paths.front() + L"/" + validTag->name);
			validQueue.push(validTag->children[i]);
			queue.push(tag->children[i]);
		}

		paths.pop();
	}

	return true;
}

static void outputTagNamesOf(const TagPtrSequence& vect) {
	if (vect.size())
		for (const Tag* tag : vect)
			std::wcout << tag->name << L", ";
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