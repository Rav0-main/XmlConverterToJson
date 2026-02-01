#include <iostream>
#include <string>
#include <vector>
#include "parsexml.hpp"
#include "assertion.hpp"

#define BOLD "\033[1m"
#define WHITE "\033[97m"
#define RED "\033[31m"
#define DEFAULT "\033[0m"

static const std::wstring DASH_LINE = L"------------------------------------------------------------------";

typedef bool (*Test) (void);

static bool testValidXmlFileWith1Root(void);
static bool testValidXmlFileWith3Roots(void);
static bool testValidXmlFileWithChaosFormat(void);
static bool testValidXmlFileWithDocTags(void);
static bool testValidXmlFileWithTagAttributes(void);
static bool testNotExistFile(void);
static bool testWrongClosingTagNameWith1Root(void);
static bool testWrongClosingTagNameWith4Roots(void);
static bool testInTagNameStartingNewTagName(void);
static bool testNotXmlFormatFile(void);
static bool testEmptyTagNameInXml(void);
static bool testTagNameHasSpacesBeforeName(void);
static bool testValidXmlWithSingleTags(void);
static bool testNotCorrectSingleTag(void);
static bool testNotClosedTagAttribute(void);

inline static void outputTestname(const std::wstring& testname);
inline static void outputDashLine(void);

int main(void) {
	const Test tests[] = {
		testValidXmlFileWith1Root,
		testValidXmlFileWith3Roots,
		testValidXmlFileWithChaosFormat,
		testValidXmlFileWithDocTags,
		testValidXmlFileWithTagAttributes,
		testNotExistFile,
		testWrongClosingTagNameWith1Root,
		testWrongClosingTagNameWith4Roots,
		testInTagNameStartingNewTagName,
		testNotXmlFormatFile,
		testEmptyTagNameInXml,
		testTagNameHasSpacesBeforeName,
		testValidXmlWithSingleTags,
		testNotCorrectSingleTag,
		testNotClosedTagAttribute
	};
	const size_t size = sizeof(tests) / sizeof(tests[0]);
	std::vector<size_t> wrongTests;
	
	for (size_t i = 0; i != size; ++i) {
		std::cout << "N " << i + 1 << ". ";
		if (!tests[i]())
			wrongTests.push_back(i+1);

		outputDashLine();
	}

	std::cout << "RESULT: " BOLD WHITE << size - wrongTests.size() 
				  << DEFAULT "/" BOLD WHITE << size << DEFAULT << std::endl;

	if (wrongTests.empty())
		std::cout << "OK.";

	else {
		std::cout << "Wrong tests: ";
		for (size_t i = 0; i < wrongTests.size()-1; ++i)
			std::cout << RED << wrongTests[i] << DEFAULT ", ";
		
		std::cout << RED << wrongTests[wrongTests.size() - 1] << DEFAULT;
	}

	std::cout << std::endl;
	return 0;
}

static bool testValidXmlFileWith1Root(void) {
	const std::string filename = "valid_test_1.xml";
	const std::wstring testname = L"Test valid xml file with 1 root";

	outputTestname(testname);

	Tag* root = new Tag;
	root->name = L"m1";

	Tag* empty1 = new Tag;
	empty1->name = L"empty1";

	Tag* t1 = new Tag;
	t1->name = L"t1";

	Tag* t1_1 = new Tag;
	t1_1->name = L"t1.1";
	t1_1->value = L"tag 1.1";

	Tag* t1_2 = new Tag;
	t1_2->name = L"t1.2";
	t1_2->value = L"tag 1.2";

	Tag* t1_3 = new Tag;
	t1_3->name = L"t1.3";
	t1_3->value = L"tag 1.3";

	Tag* empty2 = new Tag;
	empty2->name = L"empty2";

	t1->children = { t1_1, t1_2, t1_3, empty2 };

	Tag* t2 = new Tag;
	t2->name = L"t2";

	Tag* t2_1 = new Tag;
	t2_1->name = L"t2.1";
	
	Tag* t2_1_1 = new Tag;
	t2_1_1->name = L"t2.1.1";

	Tag* t2_1_1_1 = new Tag;
	t2_1_1_1->name = L"t2.1.1.1";
	t2_1_1_1->value = L"tag 2.1.1.1";

	Tag* empty3 = new Tag;
	empty3->name = L"empty3";

	t2_1_1->children = { t2_1_1_1, empty3 };

	Tag* t2_1_2 = new Tag;
	t2_1_2->name = L"t2.1.2";
	t2_1_2->value = L"tag 2.1.2";

	t2_1->children = { t2_1_1, t2_1_2 };

	Tag* t2_2 = new Tag;
	t2_2->name = L"t2.2";
	t2_2->value = L"tag 2.2";

	Tag* t2_3 = new Tag;
	t2_3->name = L"t2.3";
	t2_3->value = L"tag 2.3";

	t2->children = { t2_1, t2_2, t2_3 };

	Tag* empty4 = new Tag;
	empty4->name = L"empty4";

	root->children = { empty1, t1, t2, empty4 };

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	bool result;
	if (assertEqualParsingStatusCode(ParsingStatusCode::Success, status.code))
		result = assertEqualRoots({ root }, roots);
	else
		result = false;

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);

	return true;
}

static bool testValidXmlFileWith3Roots(void) {
	const std::string filename = "valid_test_2.xml";
	const std::wstring testname = L"Test valid xml file with 3 roots";

	outputTestname(testname);

	Tag* root1 = new Tag;
	root1->name = L"m1";

	Tag* m1t1 = new Tag;
	m1t1->name = L"t1";
	m1t1->value = L"hello";

	Tag* m1t2 = new Tag;
	m1t2->name = L"t2";

	Tag* m1t2_1 = new Tag;
	m1t2_1->name = L"t2.1";
	m1t2_1->value = L"world";

	Tag* m1t2_2 = new Tag;
	m1t2_2->name = L"t2.2";
	m1t2_2->value = L"guys";

	m1t2->children = { m1t2_1, m1t2_2 };

	root1->children = { m1t1, m1t2 };

	Tag* root2 = new Tag;
	root2->name = L"m2";

	Tag* empty = new Tag;
	empty->name = L"empty";

	Tag* m2t1 = new Tag;
	m2t1->name = L"t1";
	m2t1->value = L"hello world!!!";

	root2->children = { empty, m2t1 };

	Tag* root3 = new Tag;
	root3->name = L"m3";

	Tag* m3t1 = new Tag;
	m3t1->name = L"t1";
	m3t1->value = L"test";

	Tag* m3t2 = new Tag;
	m3t2->name = L"t2";
	m3t2->value = L"stack";

	empty = new Tag;
	empty->name = L"empty";

	root3->children = { empty, m3t1, m3t2 };

	 TagPtrSequence answer = {
		root1, root2, root3
	};

	 TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	bool result;
	if (assertEqualParsingStatusCode(ParsingStatusCode::Success, status.code))
		result = assertEqualRoots(answer, roots);
	else
		result = false;

	for (auto& tag : answer)
		freeTag(tag, &tag);

	for (auto& tag : roots)
		freeTag(tag, &tag);

	return result;
}

static bool testValidXmlFileWithChaosFormat(void) {
	const std::string filename = "valid_test_3.xml";
	const std::wstring testname = L"Test valid xml file with chaos format";

	outputTestname(testname);

	Tag* root1 = new Tag;
	root1->name = L"m1";

	Tag* t1 = new Tag;
	t1->name = L"t1";
	t1->value = L"tag 1";

	Tag* t2 = new Tag;
	t2->name = L"t2";

	Tag* t2_1 = new Tag;
	t2_1->name = L"t2.1";
	t2_1->value = L"tag 2.1";

	Tag* t2_2 = new Tag;
	t2_2->name = L"t2.2";
	t2_2->value = L"tag 2.2";

	t2->children = { t2_1, t2_2 };

	Tag* t3 = new Tag;
	t3->name = L"t3";
	t3->value = L"tag 3";

	root1->children = { t1, t2, t3 };

	Tag* root2 = new Tag;
	root2->name = L"m2";

	Tag* empty = new Tag;
	empty->name = L"empty";

	t1 = new Tag;
	t1->name = L"t1";
	t1->value = L"tag 1";

	root2->children = { empty, t1 };

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	TagPtrSequence answer = { root1, root2 };

	bool result;
	if (assertEqualParsingStatusCode(ParsingStatusCode::Success, status.code))
		result = assertEqualRoots(answer, roots);
	else
		result = false;

	for (auto& tag : answer)
		freeTag(tag, &tag);

	for (auto& tag : roots)
		freeTag(tag, &tag);

	return result;
}

static bool testValidXmlFileWithDocTags(void) {
	const std::string filename = "with_doc_tags.xml";
	const std::wstring testname = L"Test xml-file with documentation tags";

	outputTestname(testname);

	Tag* root = new Tag;
	root->name = L"main";

	Tag* t1 = new Tag;
	t1->name = L"t1";

	Tag* t1_1 = new Tag;
	t1_1->name = L"t1.1";
	t1_1->value = L"tag 1.1";

	Tag* empty = new Tag;
	empty->name = L"empty";

	t1->children = { t1_1, empty };

	Tag* t2 = new Tag;
	t2->name = L"t2";
	t2->value = L"tag 2";

	empty = new Tag;
	empty->name = L"empty";

	root->children = { t1, t2, empty };

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	bool result;
	if (assertEqualParsingStatusCode(ParsingStatusCode::Success, status.code))
		result = assertEqualRoots({ root }, roots);
	else
		result = false;

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);

	return result;
}

static bool testValidXmlFileWithTagAttributes(void) {
	const std::string filename = "tags_with_attrs.xml";
	const std::wstring testname = L"Test tags which have attributes";

	outputTestname(testname);

	Tag* root = new Tag;
	root->name = L"body";

	Tag* t1 = new Tag;
	t1->name = L"t1";
	t1->value = L"tag 1";

	Tag* t2 = new Tag;
	t2->name = L"t2";

	Tag* t2_1 = new Tag;
	t2_1->name = L"t2.1";
	t2_1->value = L"tag 2.1";

	Tag* empty = new Tag;
	empty->name = L"empty";

	Tag* t2_2 = new Tag;
	t2_2->name = L"t2.2";
	t2_2->value = L"tag 2.2";

	t2->children = { t2_1, empty, t2_2 };

	root->children = { t1, t2 };

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	bool result;
	if (assertEqualParsingStatusCode(ParsingStatusCode::Success, status.code))
		result = assertEqualRoots({ root }, roots);
	else
		result = false;

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);

	return result;
}

static bool testNotExistFile(void) {
	const std::string filename = "not_exist_file.xml";
	const std::wstring testname = L"Test not exist file";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::FileNotExistsError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testWrongClosingTagNameWith1Root(void) {
	const std::string filename = "wrong_closing_tag_name_1_tree.xml";
	const std::wstring testname = L"Test wrong closing tag name with 1 root";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::WrongClosingTagNameError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testWrongClosingTagNameWith4Roots(void) {
	const std::string filename = "wrong_closing_tag_name_4_trees.xml";
	const std::wstring testname = L"Test wrong closing tag name with 4 roots";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::WrongClosingTagNameError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testInTagNameStartingNewTagName(void) {
	const std::string filename = "wrong_tag_name.xml";
	const std::wstring testname = L"Test wrong tag name, in him starting new tag";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::WrongTagNameError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testNotXmlFormatFile(void) {
	const std::string filename = "text.txt";
	const std::wstring testname = L"Test not xml file";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatusCode(ParsingStatusCode::FileIsNotXmlError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testEmptyTagNameInXml(void) {
	const std::string filename = "with_empty_tag_name.xml";
	const std::wstring testname = L"Test empty tag name";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatusCode(ParsingStatusCode::WrongTagNameError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testTagNameHasSpacesBeforeName(void) {
	const std::string filename = "spaces_before_tag_name.xml";
	const std::wstring testname = L"Test tag name has spaces before name";
	
	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::WrongTagNameError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testValidXmlWithSingleTags(void) {
	const std::string filename = "with_single_tags.xml";
	const std::wstring testname = L"Test valid xml file with single tags";

	outputTestname(testname);

	Tag* root = new Tag;
	root->name = L"root";

	Tag* tag1 = new Tag;
	tag1->name = L"tag1";

	Tag* tag2 = new Tag;
	tag2->name = L"tag2";
	tag2->value = L"hello";

	root->children = { tag1, tag2 };

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	bool result;
	if (assertEqualParsingStatusCode(ParsingStatusCode::Success, status.code))
		result = assertEqualRoots({ root }, roots);
	else
		result = false;

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);

	return result;
}

static bool testNotCorrectSingleTag(void) {
	const std::string filename = "not_correct_single_tag.xml";
	const std::wstring testname = L"Test not correct single tag";

	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::WrongTagNameError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;
}

static bool testNotClosedTagAttribute(void) {
	const std::string filename = "not_closed_attribute.xml";
	const std::wstring testname = L"Test not closed tag attribute";
	
	outputTestname(testname);

	TagPtrSequence roots;
	const ParsingStatus status = getXmlRootsOf(filename, roots);

	if (assertEqualParsingStatusCode(ParsingStatusCode::TagAttributeNotClosedError, status.code)) {
		outputTrueVerdict();
		return true;
	}
	else
		return false;

}

static void outputTestname(const std::wstring& testname) {
	std::wcout << testname << L":" << std::endl;
}

static void outputDashLine(void) {
	std::wcout << std::endl << DASH_LINE << std::endl;
}