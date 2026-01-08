#include <iostream>
#include "..\\..\\tojson.hpp"

#define DASH_LINE L"-------------------------------------------------------------"

typedef void (*Convert) (void);

static void convertEmptyRoot(void);
static void convert1Root(void);
static void convert2Roots(void);
static void convert3Roots(void);

static void convertSimpleOneNamedArray(void);
static void convertObjectOneNamedArray(void);
static void convertTwoDifferentArraysInTag(void);
static void convertNestedDifferentArrays(void);

static void convertArrayWithNotAllSubarrays(void);

static void outputTestname(const std::wstring& testname);
static void outputThatTestConverted(void);
static void outputDashLine(void);

int main(void) {
	const Convert converts[] = {
		convertEmptyRoot,
		convert1Root,
		convert2Roots,
		convert3Roots,

		convertSimpleOneNamedArray,
		convertObjectOneNamedArray,
		convertTwoDifferentArraysInTag,
		convertNestedDifferentArrays,
		
		convertArrayWithNotAllSubarrays
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

	freeNode(root, &root);
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

	NodePtrSequence roots = {root1, root2};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
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

	NodePtrSequence roots = {root1, root2, root3};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
}

static void convertSimpleOneNamedArray(void) {
	const std::wstring testname = L"Test converting of simple one named array";
	const std::string filename = ".\\simple_one_named_array.json";

	/*
	XML-testcase:
	<main>
		<people>
			<human>Ivan</human>

			<human>Georgy</human>

			<human>John</human>

			<human>My friend</human>
		</people>
	</main>
	*/

	Node* root = new Node;
	root->tagName = L"main";
	
	Node* people = new Node;
	people->tagName = L"people";

	Node* human1 = new Node;
	human1->tagName = L"human";
	human1->value = L"Ivan";

	Node* human2 = new Node;
	human2->tagName = L"human";
	human2->value = L"Georgy";

	Node* human3 = new Node;
	human3->tagName = L"human";
	human3->value = L"John";

	Node* human4 = new Node;
	human4->tagName = L"human";
	human4->value = L"My friend";

	people->children = { human1, human2, human3, human4 };
	root->children = { people };

	NodePtrSequence roots = { root };

	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
}

static void convertObjectOneNamedArray(void) {
	const std::wstring testname = L"Test converting of object one named array";
	const std::string filename = ".\\object_one_named_array.json";

	/*
	XML-testcase:
	<main>
		<people>
			<human>
				<name>Ivan</name>
				<surname>Shweps</surname>
			</human>

			<human>
				<name>Georgy</name>
				<surname>First</surname>
			</human>
			
			<human>
				<name>Friend</name>
				<surname>Unknown</surname>
			</human>
		</people>
	</main>
	*/

	Node* root = new Node;
	root->tagName = L"main";

	Node* people = new Node;
	people->tagName = L"people";

	Node* human1 = new Node;
	human1->tagName = L"human";
	
	Node* human1name = new Node;
	human1name->tagName = L"name";
	human1name->value = L"Ivan";

	Node* human1surname = new Node;
	human1surname->tagName = L"surname";
	human1surname->value = L"Shweps";

	human1->children = { human1name, human1surname };

	Node* human2 = new Node;
	human2->tagName = L"human";
	
	Node* human2name = new Node;
	human2name->tagName = L"name";
	human2name->value = L"Georgy";

	Node* human2surname = new Node;
	human2surname->tagName = L"surname";
	human2surname->value = L"First";

	human2->children = { human2name, human2surname };

	Node* human3 = new Node;
	human3->tagName = L"human";

	Node* human3name = new Node;
	human3name->tagName = L"name";
	human3name->value = L"Friend";

	Node* human3surname = new Node;
	human3surname->tagName = L"surname";
	human3surname->value = L"Unknown";

	human3->children = { human3name, human3surname };

	people->children = { human1, human2, human3 };
	root->children = { people };

	NodePtrSequence roots = { root };
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
}

static void convertTwoDifferentArraysInTag(void) {
	const std::wstring testname = L"Test two different arrays in tag";
	const std::string filename = ".\\two_arrays_in_tag.json";

	/*
	XML-testcase:
	<main>
		<people>
			<human>
				<name>Ivan</name>
				<surname>Shewps</surname>
			</human>
			<human>
				<name>Georgy</name>
				<surname>First</surname>
			</human>
			<human>
				<name>Friend</name>
				<surname>Unknown</surname>
			</human>
			<only_name>
				Ivan
			</only_name>
			<only_name>
				Georgy
			</only_name>
			<only_name>
				John
			</only_name>
			<only_name>
				My friend
			</only_name>
		</people>
	</main>
	*/

	Node* root = new Node;
	root->tagName = L"main";

	Node* people = new Node;
	people->tagName = L"people";

	Node* human1 = new Node;
	human1->tagName = L"human";

	Node* human1name = new Node;
	human1name->tagName = L"name";
	human1name->value = L"Ivan";

	Node* human1surname = new Node;
	human1surname->tagName = L"surname";
	human1surname->value = L"Shweps";

	human1->children = { human1name, human1surname };

	Node* human2 = new Node;
	human2->tagName = L"human";

	Node* human2name = new Node;
	human2name->tagName = L"name";
	human2name->value = L"Georgy";

	Node* human2surname = new Node;
	human2surname->tagName = L"surname";
	human2surname->value = L"First";

	human2->children = { human2name, human2surname };

	Node* human3 = new Node;
	human3->tagName = L"human";

	Node* human3name = new Node;
	human3name->tagName = L"name";
	human3name->value = L"Friend";

	Node* human3surname = new Node;
	human3surname->tagName = L"surname";
	human3surname->value = L"Unknown";

	human3->children = { human3name, human3surname };

	Node* onlyName1 = new Node;
	onlyName1->tagName = L"only_name";
	onlyName1->value = L"Ivan";

	Node* onlyName2 = new Node;
	onlyName2->tagName = L"only_name";
	onlyName2->value = L"Georgy";

	Node* onlyName3 = new Node;
	onlyName3->tagName = L"only_name";
	onlyName3->value = L"John";

	Node* onlyName4 = new Node;
	onlyName4->tagName = L"only_name";
	onlyName4->value = L"My friend";

	people->children = { human1, human2, human3,
								   onlyName1, onlyName2, onlyName3, onlyName4 };

	root->children = { people };
	NodePtrSequence roots = { root };

	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
}

static void convertNestedDifferentArrays(void) {
	const std::wstring testname = L"Test nested different arrays";
	const std::string filename = ".\\nested_arrays.json";

	/*
	XML-testcase:
	<main>
		<people>
			<human>
				<name>Ivan</name>
				<surname>Shewps</surname>
			</human>
			<human>
				<name>Georgy</name>
				<surname>First</surname>
			</human>
			<human>
				<name>Friend</name>
				<surname>Unknown</surname>
			</human>
			<only_name>
				Ivan
			</only_name>
			<only_name>
				Georgy
			</only_name>
			<only_name>
				John
			</only_name>
			<only_name>
				My friend
			</only_name>
		</people>
		<people>
			<obj>w1</obj>
			<obj>w2</obj>
			<obj>w3</obj>
		</people>
		<people>tag value</people>
	</main>
	*/

	Node* root = new Node;
	root->tagName = L"main";

	Node* people1 = new Node;
	people1->tagName = L"people";

	Node* human1 = new Node;
	human1->tagName = L"human";

	Node* human1name = new Node;
	human1name->tagName = L"name";
	human1name->value = L"Ivan";

	Node* human1surname = new Node;
	human1surname->tagName = L"surname";
	human1surname->value = L"Shweps";

	human1->children = { human1name, human1surname };

	Node* human2 = new Node;
	human2->tagName = L"human";

	Node* human2name = new Node;
	human2name->tagName = L"name";
	human2name->value = L"Georgy";

	Node* human2surname = new Node;
	human2surname->tagName = L"surname";
	human2surname->value = L"First";

	human2->children = { human2name, human2surname };

	Node* human3 = new Node;
	human3->tagName = L"human";

	Node* human3name = new Node;
	human3name->tagName = L"name";
	human3name->value = L"Friend";

	Node* human3surname = new Node;
	human3surname->tagName = L"surname";
	human3surname->value = L"Unknown";

	human3->children = { human3name, human3surname };

	Node* onlyName1 = new Node;
	onlyName1->tagName = L"only_name";
	onlyName1->value = L"Ivan";

	Node* onlyName2 = new Node;
	onlyName2->tagName = L"only_name";
	onlyName2->value = L"Georgy";

	Node* onlyName3 = new Node;
	onlyName3->tagName = L"only_name";
	onlyName3->value = L"John";

	Node* onlyName4 = new Node;
	onlyName4->tagName = L"only_name";
	onlyName4->value = L"My friend";

	people1->children = { human1, human2, human3, 
									 onlyName1, onlyName2, onlyName3, onlyName4};

	Node* people2 = new Node;
	people2->tagName = L"people";

	Node* obj1 = new Node;
	obj1->tagName = L"obj";
	obj1->value = L"w1";

	Node* obj2 = new Node;
	obj2->tagName = L"obj";
	obj2->value = L"w2";

	Node* obj3 = new Node;
	obj3->tagName = L"obj";
	obj3->value = L"w3";

	people2->children = { obj1, obj2, obj3 };

	Node* people3 = new Node;
	people3->tagName = L"people";
	people3->value = L"tag value";

	root->children = { people1, people2, people3 };
	NodePtrSequence roots = { root };
	
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
}

static void convertArrayWithNotAllSubarrays(void) {
	const std::wstring testname = L"Test array with not all subarrays";
	const std::string filename = ".\\not_all_subarrays.json";

	/*
	XML-testcase:
	<main>
		<array>
			<first>1</first>
			<second>2</second>
			<third>
				<name>AMAZING</name>
				<surname>WOOOOW!</surname>
			</third>
			<first>4</first>
			<fourth>5</fourth>
		</array>
	</main>
	*/

	Node* root = new Node;
	root->tagName = L"main";
	
	Node* array = new Node;
	array->tagName = L"array";

	Node* first1 = new Node;
	first1->tagName = L"first";
	first1->value = L"1";

	Node* second = new Node;
	second->tagName = L"second";
	second->value = L"2";

	Node* third = new Node;
	third->tagName = L"third";
	
	Node* thirdName = new Node;
	thirdName->tagName = L"name";
	thirdName->value = L"AMAZING";

	Node* thirdSurname = new Node;
	thirdSurname->tagName = L"surname";
	thirdSurname->value = L"WOOOOW!";

	third->children = { thirdName, thirdSurname };

	Node* first2 = new Node;
	first2->tagName = L"first";
	first2->value = L"4";

	Node* fourth = new Node;
	fourth->tagName = L"fourth";
	fourth->value = L"5";

	array->children = { first1, second, third, first2, fourth };

	root->children = { array };
	NodePtrSequence roots = { root };

	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeNode(root, &root);
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