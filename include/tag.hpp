#pragma once

#ifndef TAG_HPP

#define TAG_HPP

#include <string>
#include <deque>

struct Tag {
	std::wstring name;
	std::wstring value;
	std::deque<Tag*> children;
};

typedef std::deque<Tag*> TagPtrSequence;

void freeTag(Tag* node, Tag** nodePtr);

void outputTags(
	const TagPtrSequence& nodes, const bool valueAsAscii = false
);

#endif