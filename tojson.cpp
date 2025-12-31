#include <fstream>
#include "tojson.hpp"

#define TABULATION '\t'

static void writeNodeIn(
	std::ofstream& file, const Node* const node,
	const unsigned short level, const bool isLastNode
);

void convertToJson(
	const std::string& filename, const std::vector<Node*>& trees
) {
	std::ofstream file(filename);
	const Node* lastRoot = trees.back();

	file << "{\n";
	for (const Node* root : trees)
		writeNodeIn(file, root, 1, root == lastRoot);

	file << "}";

	file.close();
}

static void writeNodeIn(
	std::ofstream& file, const Node* const node,
	const unsigned short level, const bool isLastNode
) {

	std::string ts(level, TABULATION);
	file << ts << '\"' << node->tagName << "\" : ";

	if (node->children.empty()) {
		file << '\"' << node->value << '\"';

		if (!isLastNode)
			file << ',';
		file << '\n';
	}
	else {
		file << "{\n";

		const Node* lastChild = node->children.back();
		for (const Node* child : node->children)
			writeNodeIn(file, child, level + 1, child == lastChild);

		file << ts << '}';
		if (!isLastNode)
			file << ',';
		file << '\n';
	}
}