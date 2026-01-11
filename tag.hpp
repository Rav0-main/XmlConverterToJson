#pragma once

#ifndef NODE_HPP

#define NODE_HPP

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
	const TagPtrSequence& nodes, const bool valueAsAscii
);

#endif