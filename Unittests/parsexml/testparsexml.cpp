#include <iostream>
#include <string>
#include <vector>
#include "..\\..\\parsexml.hpp"
#include "assertion.hpp"

#define DASH_LINE "-------------------------------------------------------------"

static void testValidXmlFileWith1Tree(const std::string& filename);
static void testValidXmlFileWith3Trees(const std::string& filename);
static void testValidXmlFileWithChaosFormat(const std::string& filename);

int main(void) {
	const std::string filenameWith1Tree = ".\\valid_test_1.xml";
	const std::string filenameWith3Tree = ".\\valid_test_2.xml";
	const std::string filenameWithChaosFormat = ".\\valid_test_3.xml";

	testValidXmlFileWith1Tree(filenameWith1Tree);
	std::cout << std::endl << DASH_LINE << std::endl;

	testValidXmlFileWith3Trees(filenameWith3Tree);
	std::cout << std::endl << DASH_LINE << std::endl;

	testValidXmlFileWithChaosFormat(filenameWithChaosFormat);

	std::cout << std::endl;
	return 0;
}

static void testValidXmlFileWith1Tree(const std::string& filename) {
	const std::string testname = "Test with valid xml file with 1 tree";

	Node* root = new Node;
	root->tagName = "m1";

	Node* empty1 = new Node;
	empty1->tagName = "empty1";

	Node* t1 = new Node;
	t1->tagName = "t1";

	Node* t1_1 = new Node;
	t1_1->tagName = "t1.1";
	t1_1->value = "tag 1.1";

	Node* t1_2 = new Node;
	t1_2->tagName = "t1.2";
	t1_2->value = "tag 1.2";

	Node* t1_3 = new Node;
	t1_3->tagName = "t1.3";
	t1_3->value = "tag 1.3";

	Node* empty2 = new Node;
	empty2->tagName = "empty2";

	t1->children = { t1_1, t1_2, t1_3, empty2 };

	Node* t2 = new Node;
	t2->tagName = "t2";

	Node* t2_1 = new Node;
	t2_1->tagName = "t2.1";
	
	Node* t2_1_1 = new Node;
	t2_1_1->tagName = "t2.1.1";

	Node* t2_1_1_1 = new Node;
	t2_1_1_1->tagName = "t2.1.1.1";
	t2_1_1_1->value = "tag 2.1.1.1";

	Node* empty3 = new Node;
	empty3->tagName = "empty3";

	t2_1_1->children = { t2_1_1_1, empty3 };

	Node* t2_1_2 = new Node;
	t2_1_2->tagName = "t2.1.2";
	t2_1_2->value = "tag 2.1.2";

	t2_1->children = { t2_1_1, t2_1_2 };

	Node* t2_2 = new Node;
	t2_2->tagName = "t2.2";
	t2_2->value = "tag 2.2";

	Node* t2_3 = new Node;
	t2_3->tagName = "t2.3";
	t2_3->value = "tag 2.3";

	t2->children = { t2_1, t2_2, t2_3 };

	Node* empty4 = new Node;
	empty4->tagName = "empty4";

	root->children = { empty1, t1, t2, empty4 };

	auto [tree, res] = getXmlTreesOf(filename);

	std::cout << " * " << testname << ":" << std::endl;
	if(assertEqualParsingResult(ParsingResult::Success, res))
		assertEqualTrees({ root }, tree);

	freeNode(root, &root);

	for (auto& node : tree)
		freeNode(node, &node);
}

static void testValidXmlFileWith3Trees(const std::string& filename) {
	const std::string testname = "Test with valid xml file with 3 trees";

	Node* root1 = new Node;
	root1->tagName = "m1";

	Node* m1t1 = new Node;
	m1t1->tagName = "t1";
	m1t1->value = "hello";

	Node* m1t2 = new Node;
	m1t2->tagName = "t2";

	Node* m1t2_1 = new Node;
	m1t2_1->tagName = "t2.1";
	m1t2_1->value = "world";

	Node* m1t2_2 = new Node;
	m1t2_2->tagName = "t2.2";
	m1t2_2->value = "guys";

	m1t2->children = { m1t2_1, m1t2_2 };

	root1->children = { m1t1, m1t2 };

	Node* root2 = new Node;
	root2->tagName = "m2";

	Node* empty = new Node;
	empty->tagName = "empty";

	Node* m2t1 = new Node;
	m2t1->tagName = "t1";
	m2t1->value = "hello world!!!";

	root2->children = { empty, m2t1 };

	Node* root3 = new Node;
	root3->tagName = "m3";

	Node* m3t1 = new Node;
	m3t1->tagName = "t1";
	m3t1->value = "test";

	Node* m3t2 = new Node;
	m3t2->tagName = "t2";
	m3t2->value = "stack";

	empty = new Node;
	empty->tagName = "empty";

	root3->children = { empty, m3t1, m3t2 };

	std::vector<Node*> answer = {
		root1, root2, root3
	};

	auto [trees, res] = getXmlTreesOf(filename);

	std::cout << " * " << testname << ":" << std::endl;
	if(assertEqualParsingResult(ParsingResult::Success, res))
		assertEqualTrees(answer, trees);

	for (auto& node : answer)
		freeNode(node, &node);

	for (auto& node : trees)
		freeNode(node, &node);
}

static void testValidXmlFileWithChaosFormat(const std::string& filename) {
	const std::string testname = "Test with valid xml file with chaos format";

	Node* root1 = new Node;
	root1->tagName = "m1";

	Node* t1 = new Node;
	t1->tagName = "t1";
	t1->value = "tag 1";

	Node* t2 = new Node;
	t2->tagName = "t2";

	Node* t2_1 = new Node;
	t2_1->tagName = "t2.1";
	t2_1->value = "tag 2.1";

	Node* t2_2 = new Node;
	t2_2->tagName = "t2.2";
	t2_2->value = "tag 2.2";

	t2->children = { t2_1, t2_2 };

	Node* t3 = new Node;
	t3->tagName = "t3";
	t3->value = "tag 3";

	root1->children = { t1, t2, t3 };

	Node* root2 = new Node;
	root2->tagName = "m2";

	Node* empty = new Node;
	empty->tagName = "empty";

	t1 = new Node;
	t1->tagName = "t1";
	t1->value = "tag 1";

	root2->children = { empty, t1 };

	auto [trees, res] = getXmlTreesOf(filename);

	std::vector<Node*> answer = { root1, root2 };

	std::cout << " * " << testname << ":" << std::endl;
	if (assertEqualParsingResult(ParsingResult::Success, res))
		assertEqualTrees(answer, trees);

	for (auto& node : answer)
		freeNode(node, &node);

	for (auto& node : trees)
		freeNode(node, &node);
}