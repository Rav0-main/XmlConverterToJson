#include <fstream>
#include <unordered_map>
#include "tojson.hpp"

#define TABULATION L'\t'

static ConvertingProfile profile = {
	.notNeedNamingOneTaggedArray = true
};

ConvertingProfile* getConvertingProfile(void) {
	return &profile;
}

void setConvertingProfile(ConvertingProfile& newProfile) {
	profile = newProfile;
}

static void writeNodeIn(
	std::wofstream& file, const Node* const node,
	const unsigned short level, const bool isLastNode,
	const bool needWriteTagName
);

ConvertingResult convertToJson(
	const NodePtrSequence& roots, const std::string& filename
) {
	std::wofstream file(filename);
	const Node* lastRoot = !roots.empty() ? roots.back() : nullptr;

	file << "{\n";
	for (const Node* root : roots)
		writeNodeIn(file, root, 1, root == lastRoot, true);

	file << "}";

	file.close();

	return ConvertingResult(
		ConvertingStatus::Success
	);
}

static void writeNodeIn(
	std::wofstream& file, const Node* const node,
	const unsigned short level, const bool isLastNode,
	const bool needWriteTagName
) {

	std::wstring ts(level, TABULATION);
	file << ts;
	if(needWriteTagName)
		file << L'\"' << node->tagName << L"\" : ";

	if (node->children.empty()) {
		file << L'\"' << node->value << L'\"';

		if (!isLastNode)
			file << L',';
		file << L'\n';
	}
	else {
		Node* lastChild = node->children.back();
		std::unordered_map<std::wstring, NodePtrSequence> groupedChildren;

		for (Node* child : node->children) {
			if (groupedChildren.contains(child->tagName))
				groupedChildren.at(child->tagName).push_back(child);
			else
				groupedChildren[child->tagName] = { child };
		}

		//not tag array
		if (groupedChildren.size() == node->children.size()) {
			file << L"{\n";
			for (const Node* child : node->children)
				writeNodeIn(file, child, level + 1, child == lastChild, true);

			file << ts << L'}';
			if (!isLastNode)
				file << L',';
			file << L'\n';
		}
		//in tag have equal child tags
		else if (groupedChildren.size() == 1 && profile.notNeedNamingOneTaggedArray) {
			file << L"[\n";
			for (const Node* child : node->children)
				writeNodeIn(file, child, level + 1, child == lastChild, false);

			file << ts << L']';
			if (!isLastNode)
				file << L',';
			file << L'\n';
		}
		else {
			file << L"{\n";
			size_t groupCount = groupedChildren.size();

			for (const auto& [tagName, children] : groupedChildren) {
				file << ts << TABULATION << L'\"' << tagName << L"\" : [\n";

				lastChild = children.back();
				for(const Node* child : children)
					writeNodeIn(file, child, level + 2, child == lastChild, false);

				--groupCount;
				file << ts << TABULATION << L']';
				if (groupCount)
					file << L',';

				file << L'\n';
			}

			file << ts << L'}';
			if (!isLastNode)
				file << L',';
			file << L'\n';
		}
	}
}