#include <fstream>
#include "tojson.hpp"

#define TABULATION L'\t'

static void writeNodeIn(
	std::wofstream& file, const Node* const node,
	const unsigned short level, const bool isLastNode
);

void convertToJson(
	const std::vector<Node*>& trees, const std::string& filename
) {
	std::wofstream file(filename);
	const Node* lastRoot = !trees.empty() ? trees.back() : nullptr;

	file << "{\n";
	for (const Node* root : trees)
		writeNodeIn(file, root, 1, root == lastRoot);

	file << "}";

	file.close();
}

static void writeNodeIn(
	std::wofstream& file, const Node* const node,
	const unsigned short level, const bool isLastNode
) {

	std::wstring ts(level, TABULATION);
	file << ts << L'\"' << node->tagName << L"\" : ";

	if (node->children.empty()) {
		file << L'\"' << node->value << L'\"';

		if (!isLastNode)
			file << L',';
		file << L'\n';
	}
	else {
		file << L"{\n";

		const Node* lastChild = node->children.back();
		for (const Node* child : node->children)
			writeNodeIn(file, child, level + 1, child == lastChild);

		file << ts << L'}';
		if (!isLastNode)
			file << L',';
		file << L'\n';
	}
}