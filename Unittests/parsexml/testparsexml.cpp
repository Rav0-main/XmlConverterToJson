#include <iostream>
#include <string>
#include "..\\..\\parsexml.hpp"
#include "assertion.hpp"

#define DASH_LINE L"-------------------------------------------------------------"

typedef void (*Test) (void);

static void testValidXmlFileWith1Root(void);
static void testValidXmlFileWith3Roots(void);
static void testValidXmlFileWithChaosFormat(void);
static void testValidXmlFileWithDocTags(void);
static void testValidXmlFileWithTagAttributes(void);
static void testNotExistFile(void);
static void testWrongClosingTagNameWith1Root(void);
static void testWrongClosingTagNameWith4Roots(void);
static void testInTagNameStartingNewTagName(void);

static void outputTestname(const std::wstring& testname);
static void outputDashLine(void);

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
		testInTagNameStartingNewTagName
	};
	const size_t size = sizeof(tests) / sizeof(tests[0]);

	for (int i = 0; i < size - 1; ++i) {
		tests[i]();
		outputDashLine();
	}

	tests[size - 1]();

	std::cout << std::endl;
	return 0;
}

static void testValidXmlFileWith1Root(void) {
	const std::string filename = ".\\valid_test_1.xml";
	const std::wstring testname = L"Test with valid xml file with 1 root";

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
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if(assertEqualParsingStatus(ParsingStatus::Success, res))
		assertEqualRoots({ root }, roots);

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);
}

static void testValidXmlFileWith3Roots(void) {
	const std::string filename = ".\\valid_test_2.xml";
	const std::wstring testname = L"Test with valid xml file with 3 roots";

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
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if(assertEqualParsingStatus(ParsingStatus::Success, res))
		assertEqualRoots(answer, roots);

	for (auto& tag : answer)
		freeTag(tag, &tag);

	for (auto& tag : roots)
		freeTag(tag, &tag);
}

static void testValidXmlFileWithChaosFormat(void) {
	const std::string filename = ".\\valid_test_3.xml";
	const std::wstring testname = L"Test with valid xml file with chaos format";

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
	auto [res] = getXmlRootsOf(filename, roots);

	TagPtrSequence answer = { root1, root2 };

	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::Success, res))
		assertEqualRoots(answer, roots);

	for (auto& tag : answer)
		freeTag(tag, &tag);

	for (auto& tag : roots)
		freeTag(tag, &tag);
}

static void testValidXmlFileWithDocTags(void) {
	const std::string filename = ".\\with_doc_tags.xml";
	const std::wstring testname = L"Test xml-file with documentation tags";

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
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::Success, res))
		assertEqualRoots({ root }, roots);

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);
}

static void testValidXmlFileWithTagAttributes(void) {
	const std::string filename = ".\\tags_with_attrs.xml";
	const std::wstring testname = L"Test with tags which have attributes";

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
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::Success, res))
		assertEqualRoots({ root }, roots);

	freeTag(root, &root);

	for (auto& tag : roots)
		freeTag(tag, &tag);
}

static void testNotExistFile(void) {
	const std::string filename = ".\\not_exist_file.xml";
	const std::wstring testname = L"Test not exist file";

	TagPtrSequence roots;
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::FileNotExistsError, res))
		outputTrueVerdict();
}

static void testWrongClosingTagNameWith1Root(void) {
	const std::string filename = ".\\wrong_closing_tag_name_1_tree.xml";
	const std::wstring testname = L"Test with wrong closing tag name with 1 root";

	TagPtrSequence roots;
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::WrongClosingTagNameError, res))
		outputTrueVerdict();
}

static void testWrongClosingTagNameWith4Roots(void) {
	const std::string filename = ".\\wrong_closing_tag_name_4_trees.xml";
	const std::wstring testname = L"Test with wrong closing tag name with 4 roots";

	TagPtrSequence roots;
	auto [res] = getXmlRootsOf(filename, roots);

	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::WrongClosingTagNameError, res))
		outputTrueVerdict();
}

static void testInTagNameStartingNewTagName(void) {
	const std::string filename = ".\\wrong_tag_name.xml";
	const std::wstring testname = L"Test with wrong tag name, in him starting new tag";

	TagPtrSequence roots;
	auto [res] = getXmlRootsOf(filename, roots);
	outputTestname(testname);
	if (assertEqualParsingStatus(ParsingStatus::WrongTagNameError, res))
		outputTrueVerdict();
}

static void outputTestname(const std::wstring& testname) {
	std::wcout << L" * " << testname << L":" << std::endl;
}

static void outputDashLine(void) {
	std::wcout << std::endl << DASH_LINE << std::endl;
}