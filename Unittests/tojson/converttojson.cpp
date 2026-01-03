#include <iostream>
#include "..\\..\\tojson.hpp"

#define DASH_LINE L"-------------------------------------------------------------"

typedef void (*Convert) (void);

static void convertEmptyRoot(void);
static void convert1Root(void);
static void convert2Roots(void);
static void convert3Roots(void);

static void outputTestname(const std::wstring& testname);
static void outputThatTestConverted(void);
static void outputDashLine(void);

int main(void) {
	const Convert converts[] = {
		convertEmptyRoot,
		convert1Root,
		convert2Roots,
		convert3Roots
	};

	const size_t size = sizeof(converts) / sizeof(converts[0]);

	for (int i = 0; i < size - 1; ++i) {
		converts[i]();
		outputDashLine();
	}

	converts[size - 1]();

	std::cout << std::endl;
	return 0;
}

void convertEmptyRoot(void) {
	const std::wstring testname = L"Test with empty tree";
	const std::string filename = ".\\empty_tree.json";

	/*
	XML-testcase:

	*/
	
	const NodePtrSequence root = {};

	convertToJson(root, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void convert1Root(void) {
	const std::wstring testname = L"Test with 1 tree";
	const std::string filename = ".\\1_tree.json";

	/*
	XML-testcase:
	<main>
		<t1>tag 1</t1>
		
		<t2>
			<t2.1>tag 2.1</t2.1>
			
			<t2.2>
				<t2.2.1>tag 2.2.1</t2.2.1>
				<empty></empty>
			</t2.2>

			<empty></empty>
		</t2>

		<NOT EMPTY>MEGA VALUE</NOT EMPTY>
	</main>
	*/

	Node* root = new Node;
	root->tagName = L"main";

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
	
	Node* t2_2_1 = new Node;
	t2_2_1->tagName = L"t2.2.1";
	t2_2_1->value = L"tag 2.2.1";

	Node* t2_2_2 = new Node;
	t2_2_2->tagName = L"empty";

	t2_2->children = { t2_2_1, t2_2_2 };

	Node* t2_3 = new Node;
	t2_3->tagName = L"empty";

	t2->children = { t2_1, t2_2, t2_3 };

	Node* t3 = new Node;
	t3->tagName = L"NOT EMPTY";
	t3->value = L"MEGA VAlUE";

	root->children = { t1, t2, t3 };

	const NodePtrSequence roots = {root};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void convert2Roots(void) {
	const std::wstring testname = L"Test with 2 tree";
	const std::string filename = ".\\2_trees.json";

	/*
	XML-testcase:
	<root1>
		<t1>tag 1</t1>

		<t2>
			<t2.1>tag 2.1</t2.1>
			<t2.2>tag 2.2</t2.2>
		</t2>
		
		<empty></empty>
	</root1>

	<root2>
		<empty></empty>
		<t1>tag 1</t1>
	</root2>
	*/

	Node* root1 = new Node;
	root1->tagName = L"root1";

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

	Node* empty = new Node;
	empty->tagName = L"empty";

	root1->children = { t1, t2, empty };

	Node* root2 = new Node;
	root2->tagName = L"root2";

	empty = new Node;
	empty->tagName = L"empty";

	t1 = new Node;
	t1->tagName = L"t1";
	t1->value = L"tag 1";

	root2->children = { empty, t1 };

	const NodePtrSequence roots = {root1, root2};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void convert3Roots(void) {
	const std::wstring testname = L"Test with 3 trees";
	const std::string filename = ".\\3_trees.json";

	/*
	XML-testcase:
	<root1>
	</root1>

	<root2>
	HELLO
	</root2>

	<root3>
		<t1>tag 1</t1>
		<t2>tag 2</t2>
	</root3>
	*/

	Node* root1 = new Node;
	root1->tagName = L"root1";

	Node* root2 = new Node;
	root2->tagName = L"HELLO";

	Node* root3 = new Node;
	root3->tagName = L"root3";

	Node* t1 = new Node;
	t1->tagName = L"t1";
	t1->value = L"tag 1";

	Node* t2 = new Node;
	t2->tagName = L"t2";
	t2->value = L"tag 2";

	root3->children = { t1, t2 };

	const NodePtrSequence roots = {root1, root2, root3};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void outputTestname(const std::wstring& testname) {
	std::wcout << L" * " << testname << L":" << std::endl;
}

static void outputThatTestConverted(void) {
	std::wcout << L"CONVERTED." << std::endl;
}

static void outputDashLine(void) {
	std::wcout << std::endl << DASH_LINE << std::endl;
}