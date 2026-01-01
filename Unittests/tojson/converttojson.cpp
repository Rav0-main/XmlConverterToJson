#include <iostream>
#include "..\\..\\tojson.hpp"

#define DASH_LINE "-------------------------------------------------------------"

typedef void (*Convert) (void);

static void convertEmptyTree(void);
static void convert1Tree(void);
static void convert2Trees(void);
static void convert3Trees(void);

static void outputTestname(const std::string& testname);
static void outputThatTestConverted(void);
static void outputDashLine(void);

int main(void) {
	const Convert converts[] = {
		convertEmptyTree,
		convert1Tree,
		convert2Trees,
		convert3Trees
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

void convertEmptyTree(void) {
	const std::string testname = "Test with empty tree";
	const std::string filename = ".\\empty_tree.json";

	/*
	XML-testcase:

	*/
	
	convertToJson({}, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void convert1Tree(void) {
	const std::string testname = "Test with 1 tree";
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
	root->tagName = "main";

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
	
	Node* t2_2_1 = new Node;
	t2_2_1->tagName = "t2.2.1";
	t2_2_1->value = "tag 2.2.1";

	Node* t2_2_2 = new Node;
	t2_2_2->tagName = "empty";

	t2_2->children = { t2_2_1, t2_2_2 };

	Node* t2_3 = new Node;
	t2_3->tagName = "empty";

	t2->children = { t2_1, t2_2, t2_3 };

	Node* t3 = new Node;
	t3->tagName = "NOT EMPTY";
	t3->value = "MEGA VAlUE";

	root->children = { t1, t2, t3 };

	convertToJson({ root }, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void convert2Trees(void) {
	const std::string testname = "Test with 2 tree";
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
	root1->tagName = "root1";

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

	Node* empty = new Node;
	empty->tagName = "empty";

	root1->children = { t1, t2, empty };

	Node* root2 = new Node;
	root2->tagName = "root2";

	empty = new Node;
	empty->tagName = "empty";

	t1 = new Node;
	t1->tagName = "t1";
	t1->value = "tag 1";

	root2->children = { empty, t1 };

	convertToJson({ root1, root2 }, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void convert3Trees(void) {
	const std::string testname = "Test with 3 trees";
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
	root1->tagName = "root1";

	Node* root2 = new Node;
	root2->tagName = "HELLO";

	Node* root3 = new Node;
	root3->tagName = "root3";

	Node* t1 = new Node;
	t1->tagName = "t1";
	t1->value = "tag 1";

	Node* t2 = new Node;
	t2->tagName = "t2";
	t2->value = "tag 2";

	root3->children = { t1, t2 };

	convertToJson({ root1, root2, root3 }, filename);

	outputTestname(testname);
	outputThatTestConverted();
}

static void outputTestname(const std::string& testname) {
	std::cout << " * " << testname << ":" << std::endl;
}

static void outputThatTestConverted(void) {
	std::cout << "CONVERTED." << std::endl;
}

static void outputDashLine(void) {
	std::cout << std::endl << DASH_LINE << std::endl;
}