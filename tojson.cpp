#include <fstream>
#include <unordered_map>
#include "tojson.hpp"

#define TABULATION L'\t'

struct WritingNodeParameters {
	unsigned short level;
	bool isLastNode;
	bool writeTagName;
};

static void writeNodeIn(
	std::wofstream& file, const Node* const node,
	const WritingNodeParameters parameters
);
inline static void groupChildrenByName(
	const Node* const node,
	std::unordered_map<std::wstring, NodePtrSequence>& groups
);

static ConvertingProfile profile = {
	.notNamesOneTagArray = true
};

ConvertingProfile* getConvertingProfile(void) {
	return &profile;
}

void setConvertingProfile(ConvertingProfile& newProfile) {
	profile = newProfile;
}

ConvertingResult convertToJson(
	const NodePtrSequence& roots, const std::string& filename
) {
	std::wofstream file(filename);
	const Node* lastRoot = !roots.empty() ? roots.back() : nullptr;

	file << "{\n";
	for (const Node* root : roots)
		writeNodeIn(file, root, WritingNodeParameters{
			.level = 1,
			.isLastNode = root == lastRoot,
			.writeTagName = true
			}
		);

	file << "}";

	file.close();

	return ConvertingResult(
		ConvertingStatus::Success
	);
}

static void writeNodeIn(
	std::wofstream& file, const Node* const node,
	const WritingNodeParameters parameters
) {
	std::wstring ts(parameters.level, TABULATION);
	file << ts;

	if(parameters.writeTagName)
		file << L'\"' << node->tagName << L"\" : ";

	if (node->children.empty())
		file << L'\"' << node->value << L'\"';

	else {
		Node* lastChild = node->children.back();
		std::unordered_map<std::wstring, NodePtrSequence> groupedChildren;

		groupChildrenByName(node, groupedChildren);
		
		//haves not tag array
		if (groupedChildren.size() == node->children.size()) {
			file << L"{\n";

			for (const Node* child : node->children)
				writeNodeIn(file, child, WritingNodeParameters{
					.level = static_cast<unsigned short>(parameters.level + 1),
					.isLastNode = child == lastChild,
					.writeTagName = true
					}
				);

			file << ts << L'}';
		}
		//haves children tags with equal tagName
		else if (groupedChildren.size() == 1 && profile.notNamesOneTagArray) {
			file << L"[\n";

			for (const Node* child : node->children)
				writeNodeIn(file, child, WritingNodeParameters{
					.level = static_cast<unsigned short>(parameters.level + 1),
					.isLastNode = child == lastChild,
					.writeTagName = false
					}
				);

			file << ts << L']';
		}
		else {
			file << L"{\n";

			size_t groupCount = groupedChildren.size();

			for (const auto& [childName, children] : groupedChildren) {
				file << ts << TABULATION << L'\"' << childName;
				if (children.size() != 1)
					file << L"\" : [\n";
				else
					file << L"\" : \n";

				lastChild = children.back();
				for (const Node* child : children)
					writeNodeIn(file, child,
						WritingNodeParameters{
							.level = static_cast<unsigned short>(parameters.level + 2),
							.isLastNode = child == lastChild,
							.writeTagName = false
						}
					);

				--groupCount;
				file << ts << TABULATION;
				if(children.size() != 1)
					file << L']';

				if (groupCount)
					file << L',';

				file << L'\n';
			}

			file << ts << L'}';
		}
	}

	if (!parameters.isLastNode)
		file << L',';

	file << L'\n';
}

inline static void groupChildrenByName(
	const Node* const node,
	std::unordered_map<std::wstring, NodePtrSequence>& groups
) {
	for (Node* child : node->children) {
		if (groups.contains(child->tagName))
			groups.at(child->tagName).push_back(child);
		else
			groups[child->tagName] = { child };
	}
}