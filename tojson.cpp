#include <fstream>
#include <unordered_map>
#include "tojson.hpp"

#define TABULATION L'\t'

struct WritingTagParameters {
	unsigned short level;
	bool isLastTag;
	bool writeTagName;
};

static void writeTagIn(
	std::wofstream& file, const Tag* const tag,
	const WritingTagParameters parameters
);
inline static void groupChildrenByName(
	const Tag* const tag,
	std::unordered_map<std::wstring, TagPtrSequence>& groups
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
	const TagPtrSequence& roots, const std::string& filename
) {
	std::wofstream file(filename);
	const Tag* lastRoot = !roots.empty() ? roots.back() : nullptr;

	file << "{\n";
	for (const Tag* root : roots)
		writeTagIn(file, root, WritingTagParameters{
			.level = 1,
			.isLastTag = root == lastRoot,
			.writeTagName = true
			}
		);

	file << "}";

	file.close();

	return ConvertingResult(
		ConvertingStatus::Success
	);
}

static void writeTagIn(
	std::wofstream& file, const Tag* const tag,
	const WritingTagParameters parameters
) {
	std::wstring ts(parameters.level, TABULATION);
	file << ts;

	if(parameters.writeTagName)
		file << L'\"' << tag->name << L"\" : ";

	if (tag->children.empty())
		file << L'\"' << tag->value << L'\"';

	else {
		Tag* lastChild = tag->children.back();
		std::unordered_map<std::wstring, TagPtrSequence> groupedChildren;

		groupChildrenByName(tag, groupedChildren);
		
		//haves not tag array
		if (groupedChildren.size() == tag->children.size()) {
			file << L"{\n";

			for (const Tag* child : tag->children)
				writeTagIn(file, child, WritingTagParameters{
					.level = static_cast<unsigned short>(parameters.level + 1),
					.isLastTag = child == lastChild,
					.writeTagName = true
					}
				);

			file << ts << L'}';
		}
		//haves children tags with equal tagName
		else if (groupedChildren.size() == 1 && profile.notNamesOneTagArray) {
			file << L"[\n";

			for (const Tag* child : tag->children)
				writeTagIn(file, child, WritingTagParameters{
					.level = static_cast<unsigned short>(parameters.level + 1),
					.isLastTag = child == lastChild,
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
				for (const Tag* child : children)
					writeTagIn(file, child,
						WritingTagParameters{
							.level = static_cast<unsigned short>(parameters.level + 2),
							.isLastTag = child == lastChild,
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

	if (!parameters.isLastTag)
		file << L',';

	file << L'\n';
}

inline static void groupChildrenByName(
	const Tag* const tag,
	std::unordered_map<std::wstring, TagPtrSequence>& groups
) {
	for (Tag* child : tag->children) {
		if (groups.contains(child->name))
			groups.at(child->name).push_back(child);
		else
			groups[child->name] = { child };
	}
}