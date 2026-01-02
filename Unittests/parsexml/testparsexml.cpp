#include <iostream>
#include <string>
#include <vector>
#include "..\\..\\parsexml.hpp"
#include "assertion.hpp"

#define DASH_LINE L"-------------------------------------------------------------"

typedef void (*Test) (void);

static void testValidXmlFileWith1Tree(void);
static void testValidXmlFileWith3Trees(void);
static void testValidXmlFileWithChaosFormat(void);
static void testNotExistFile(void);
static void testWrongClosingTagNameWith1Tree(void);
static void testWrongClosingTagNameWith4Trees(void);
static void testInTagNameStartingNewTagName(void);

static void outputTestname(const std::wstring& testname);
static void outputDashLine(void);

int main(void) {
	const Test tests[] = {
		testValidXmlFileWith1Tree,
		testValidXmlFileWith3Trees,
		testValidXmlFileWithChaosFormat,
		testNotExistFile,
		testWrongClosingTagNameWith1Tree,
		testWrongClosingTagNameWith4Trees,
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

static void testValidXmlFileWith1Tree(void) {
	const std::string filename = ".\\valid_test_1.xml";
	const std::wstring testname = L"Test with valid xml file with 1 tree";

	Node* root = new Node;
	root->tagName = L"m1";

	Node* empty1 = new Node;
	empty1->tagName = L"empty1";

	Node* t1 = new Node;
	t1->tagName = L"t1";

	Node* t1_1 = new Node;
	t1_1->tagName = L"t1.1";
	t1_1->value = L"tag 1.1";

	Node* t1_2 = new Node;
	t1_2->tagName = L"t1.2";
	t1_2->value = L"tag 1.2";

	Node* t1_3 = new Node;
	t1_3->tagName = L"t1.3";
	t1_3->value = L"tag 1.3";

	Node* empty2 = new Node;
	empty2->tagName = L"empty2";

	t1->children = { t1_1, t1_2, t1_3, empty2 };

	Node* t2 = new Node;
	t2->tagName = L"t2";

	Node* t2_1 = new Node;
	t2_1->tagName = L"t2.1";
	
	Node* t2_1_1 = new Node;
	t2_1_1->tagName = L"t2.1.1";

	Node* t2_1_1_1 = new Node;
	t2_1_1_1->tagName = L"t2.1.1.1";
	t2_1_1_1->value = L"tag 2.1.1.1";

	Node* empty3 = new Node;
	empty3->tagName = L"empty3";

	t2_1_1->children = { t2_1_1_1, empty3 };

	Node* t2_1_2 = new Node;
	t2_1_2->tagName = L"t2.1.2";
	t2_1_2->value = L"tag 2.1.2";

	t2_1->children = { t2_1_1, t2_1_2 };

	Node* t2_2 = new Node;
	t2_2->tagName = L"t2.2";
	t2_2->value = L"tag 2.2";

	Node* t2_3 = new Node;
	t2_3->tagName = L"t2.3";
	t2_3->value = L"tag 2.3";

	t2->children = { t2_1, t2_2, t2_3 };

	Node* empty4 = new Node;
	empty4->tagName = L"empty4";

	root->children = { empty1, t1, t2, empty4 };

	auto [tree, res] = getXmlTreesOf(filename);

	outputTestname(testname);
	if(assertEqualParsingResult(ParsingResult::Success, res))
		assertEqualTrees({ root }, tree);

	freeNode(root, &root);

	for (auto& node : tree)
		freeNode(node, &node);
}

static void testValidXmlFileWith3Trees(void) {
	const std::string filename = ".\\valid_test_2.xml";
	const std::wstring testname = L"Test with valid xml file with 3 trees";

	Node* root1 = new Node;
	root1->tagName = L"m1";

	Node* m1t1 = new Node;
	m1t1->tagName = L"t1";
	m1t1->value = L"hello";

	Node* m1t2 = new Node;
	m1t2->tagName = L"t2";

	Node* m1t2_1 = new Node;
	m1t2_1->tagName = L"t2.1";
	m1t2_1->value = L"world";

	Node* m1t2_2 = new Node;
	m1t2_2->tagName = L"t2.2";
	m1t2_2->value = L"guys";

	m1t2->children = { m1t2_1, m1t2_2 };

	root1->children = { m1t1, m1t2 };

	Node* root2 = new Node;
	root2->tagName = L"m2";

	Node* empty = new Node;
	empty->tagName = L"empty";

	Node* m2t1 = new Node;
	m2t1->tagName = L"t1";
	m2t1->value = L"hello world!!!";

	root2->children = { empty, m2t1 };

	Node* root3 = new Node;
	root3->tagName = L"m3";

	Node* m3t1 = new Node;
	m3t1->tagName = L"t1";
	m3t1->value = L"test";

	Node* m3t2 = new Node;
	m3t2->tagName = L"t2";
	m3t2->value = L"stack";

	empty = new Node;
	empty->tagName = L"empty";

	root3->children = { empty, m3t1, m3t2 };

	std::vector<Node*> answer = {
		root1, root2, root3
	};

	auto [trees, res] = getXmlTreesOf(filename);

	outputTestname(testname);
	if(assertEqualParsingResult(ParsingResult::Success, res))
		assertEqualTrees(answer, trees);

	for (auto& node : answer)
		freeNode(node, &node);

	for (auto& node : trees)
		freeNode(node, &node);
}

static void testValidXmlFileWithChaosFormat(void) {
	const std::string filename = ".\\valid_test_3.xml";
	const std::wstring testname = L"Test with valid xml file with chaos format";

	Node* root1 = new Node;
	root1->tagName = L"m1";

	Node* t1 = new Node;
	t1->tagName = L"t1";
	t1->value = L"tag 1";

	Node* t2 = new Node;
	t2->tagName = L"t2";

	Node* t2_1 = new Node;
	t2_1->tagName = L"t2.1";
	t2_1->value = L"tag 2.1";

	Node* t2_2 = new Node;
	t2_2->tagName = L"t2.2";
	t2_2->value = L"tag 2.2";

	t2->children = { t2_1, t2_2 };

	Node* t3 = new Node;
	t3->tagName = L"t3";
	t3->value = L"tag 3";

	root1->children = { t1, t2, t3 };

	Node* root2 = new Node;
	root2->tagName = L"m2";

	Node* empty = new Node;
	empty->tagName = L"empty";

	t1 = new Node;
	t1->tagName = L"t1";
	t1->value = L"tag 1";

	root2->children = { empty, t1 };

	auto [trees, res] = getXmlTreesOf(filename);

	std::vector<Node*> answer = { root1, root2 };

	outputTestname(testname);
	if (assertEqualParsingResult(ParsingResult::Success, res))
		assertEqualTrees(answer, trees);

	for (auto& node : answer)
		freeNode(node, &node);

	for (auto& node : trees)
		freeNode(node, &node);
}

static void testNotExistFile(void) {
	const std::string filename = ".\\not_exist_file.xml";
	const std::wstring testname = L"Test with not exist file";

	auto [trees, res] = getXmlTreesOf(filename);

	outputTestname(testname);
	if (assertEqualParsingResult(ParsingResult::FileNotExistsError, res))
		outputTrueVerdict();
}

static void testWrongClosingTagNameWith1Tree(void) {
	const std::string filename = ".\\wrong_closing_tag_name_1_tree.xml";
	const std::wstring testname = L"Test with wrong closing tag name with 1 tree";

	auto [trees, res] = getXmlTreesOf(filename);

	outputTestname(testname);
	if (assertEqualParsingResult(ParsingResult::WrongClosingTagNameError, res))
		outputTrueVerdict();
}

static void testWrongClosingTagNameWith4Trees(void) {
	const std::string filename = ".\\wrong_closing_tag_name_4_trees.xml";
	const std::wstring testname = L"Test with wrong closing tag name with 4 trees";

	auto [trees, res] = getXmlTreesOf(filename);

	outputTestname(testname);
	if (assertEqualParsingResult(ParsingResult::WrongClosingTagNameError, res))
		outputTrueVerdict();
}

static void testInTagNameStartingNewTagName(void) {
	const std::string filename = ".\\wrong_tag_name.xml";
	const std::wstring testname = L"Test with wrong tag name, in him starting new tag";

	auto [trees, res] = getXmlTreesOf(filename);
	outputTestname(testname);
	if (assertEqualParsingResult(ParsingResult::WrongTagNameError, res))
		outputTrueVerdict();
}

static void outputTestname(const std::wstring& testname) {
	std::wcout << L" * " << testname << L":" << std::endl;
}

static void outputDashLine(void) {
	std::wcout << std::endl << DASH_LINE << std::endl;
}