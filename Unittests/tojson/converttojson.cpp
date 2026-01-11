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
	
	const TagPtrSequence root = {};

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

	Tag* root = new Tag;
	root->name = L"main";

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
	
	Tag* t2_2_1 = new Tag;
	t2_2_1->name = L"t2.2.1";
	t2_2_1->value = L"tag 2.2.1";

	Tag* t2_2_2 = new Tag;
	t2_2_2->name = L"empty";

	t2_2->children = { t2_2_1, t2_2_2 };

	Tag* t2_3 = new Tag;
	t2_3->name = L"empty";

	t2->children = { t2_1, t2_2, t2_3 };

	Tag* t3 = new Tag;
	t3->name = L"NOT EMPTY";
	t3->value = L"MEGA VAlUE";

	root->children = { t1, t2, t3 };

	const TagPtrSequence roots = {root};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	freeTag(root, &root);
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

	Tag* root1 = new Tag;
	root1->name = L"root1";

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

	Tag* empty = new Tag;
	empty->name = L"empty";

	root1->children = { t1, t2, empty };

	Tag* root2 = new Tag;
	root2->name = L"root2";

	empty = new Tag;
	empty->name = L"empty";

	t1 = new Tag;
	t1->name = L"t1";
	t1->value = L"tag 1";

	root2->children = { empty, t1 };

	TagPtrSequence roots = {root1, root2};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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

	Tag* root1 = new Tag;
	root1->name = L"root1";

	Tag* root2 = new Tag;
	root2->name = L"HELLO";

	Tag* root3 = new Tag;
	root3->name = L"root3";

	Tag* t1 = new Tag;
	t1->name = L"t1";
	t1->value = L"tag 1";

	Tag* t2 = new Tag;
	t2->name = L"t2";
	t2->value = L"tag 2";

	root3->children = { t1, t2 };

	TagPtrSequence roots = {root1, root2, root3};
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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

	Tag* root = new Tag;
	root->name = L"main";
	
	Tag* people = new Tag;
	people->name = L"people";

	Tag* human1 = new Tag;
	human1->name = L"human";
	human1->value = L"Ivan";

	Tag* human2 = new Tag;
	human2->name = L"human";
	human2->value = L"Georgy";

	Tag* human3 = new Tag;
	human3->name = L"human";
	human3->value = L"John";

	Tag* human4 = new Tag;
	human4->name = L"human";
	human4->value = L"My friend";

	people->children = { human1, human2, human3, human4 };
	root->children = { people };

	TagPtrSequence roots = { root };

	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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

	Tag* root = new Tag;
	root->name = L"main";

	Tag* people = new Tag;
	people->name = L"people";

	Tag* human1 = new Tag;
	human1->name = L"human";
	
	Tag* human1name = new Tag;
	human1name->name = L"name";
	human1name->value = L"Ivan";

	Tag* human1surname = new Tag;
	human1surname->name = L"surname";
	human1surname->value = L"Shweps";

	human1->children = { human1name, human1surname };

	Tag* human2 = new Tag;
	human2->name = L"human";
	
	Tag* human2name = new Tag;
	human2name->name = L"name";
	human2name->value = L"Georgy";

	Tag* human2surname = new Tag;
	human2surname->name = L"surname";
	human2surname->value = L"First";

	human2->children = { human2name, human2surname };

	Tag* human3 = new Tag;
	human3->name = L"human";

	Tag* human3name = new Tag;
	human3name->name = L"name";
	human3name->value = L"Friend";

	Tag* human3surname = new Tag;
	human3surname->name = L"surname";
	human3surname->value = L"Unknown";

	human3->children = { human3name, human3surname };

	people->children = { human1, human2, human3 };
	root->children = { people };

	TagPtrSequence roots = { root };
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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

	Tag* root = new Tag;
	root->name = L"main";

	Tag* people = new Tag;
	people->name = L"people";

	Tag* human1 = new Tag;
	human1->name = L"human";

	Tag* human1name = new Tag;
	human1name->name = L"name";
	human1name->value = L"Ivan";

	Tag* human1surname = new Tag;
	human1surname->name = L"surname";
	human1surname->value = L"Shweps";

	human1->children = { human1name, human1surname };

	Tag* human2 = new Tag;
	human2->name = L"human";

	Tag* human2name = new Tag;
	human2name->name = L"name";
	human2name->value = L"Georgy";

	Tag* human2surname = new Tag;
	human2surname->name = L"surname";
	human2surname->value = L"First";

	human2->children = { human2name, human2surname };

	Tag* human3 = new Tag;
	human3->name = L"human";

	Tag* human3name = new Tag;
	human3name->name = L"name";
	human3name->value = L"Friend";

	Tag* human3surname = new Tag;
	human3surname->name = L"surname";
	human3surname->value = L"Unknown";

	human3->children = { human3name, human3surname };

	Tag* onlyName1 = new Tag;
	onlyName1->name = L"only_name";
	onlyName1->value = L"Ivan";

	Tag* onlyName2 = new Tag;
	onlyName2->name = L"only_name";
	onlyName2->value = L"Georgy";

	Tag* onlyName3 = new Tag;
	onlyName3->name = L"only_name";
	onlyName3->value = L"John";

	Tag* onlyName4 = new Tag;
	onlyName4->name = L"only_name";
	onlyName4->value = L"My friend";

	people->children = { human1, human2, human3,
								   onlyName1, onlyName2, onlyName3, onlyName4 };

	root->children = { people };
	TagPtrSequence roots = { root };

	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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

	Tag* root = new Tag;
	root->name = L"main";

	Tag* people1 = new Tag;
	people1->name = L"people";

	Tag* human1 = new Tag;
	human1->name = L"human";

	Tag* human1name = new Tag;
	human1name->name = L"name";
	human1name->value = L"Ivan";

	Tag* human1surname = new Tag;
	human1surname->name = L"surname";
	human1surname->value = L"Shweps";

	human1->children = { human1name, human1surname };

	Tag* human2 = new Tag;
	human2->name = L"human";

	Tag* human2name = new Tag;
	human2name->name = L"name";
	human2name->value = L"Georgy";

	Tag* human2surname = new Tag;
	human2surname->name = L"surname";
	human2surname->value = L"First";

	human2->children = { human2name, human2surname };

	Tag* human3 = new Tag;
	human3->name = L"human";

	Tag* human3name = new Tag;
	human3name->name = L"name";
	human3name->value = L"Friend";

	Tag* human3surname = new Tag;
	human3surname->name = L"surname";
	human3surname->value = L"Unknown";

	human3->children = { human3name, human3surname };

	Tag* onlyName1 = new Tag;
	onlyName1->name = L"only_name";
	onlyName1->value = L"Ivan";

	Tag* onlyName2 = new Tag;
	onlyName2->name = L"only_name";
	onlyName2->value = L"Georgy";

	Tag* onlyName3 = new Tag;
	onlyName3->name = L"only_name";
	onlyName3->value = L"John";

	Tag* onlyName4 = new Tag;
	onlyName4->name = L"only_name";
	onlyName4->value = L"My friend";

	people1->children = { human1, human2, human3, 
									 onlyName1, onlyName2, onlyName3, onlyName4};

	Tag* people2 = new Tag;
	people2->name = L"people";

	Tag* obj1 = new Tag;
	obj1->name = L"obj";
	obj1->value = L"w1";

	Tag* obj2 = new Tag;
	obj2->name = L"obj";
	obj2->value = L"w2";

	Tag* obj3 = new Tag;
	obj3->name = L"obj";
	obj3->value = L"w3";

	people2->children = { obj1, obj2, obj3 };

	Tag* people3 = new Tag;
	people3->name = L"people";
	people3->value = L"tag value";

	root->children = { people1, people2, people3 };
	TagPtrSequence roots = { root };
	
	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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

	Tag* root = new Tag;
	root->name = L"main";
	
	Tag* array = new Tag;
	array->name = L"array";

	Tag* first1 = new Tag;
	first1->name = L"first";
	first1->value = L"1";

	Tag* second = new Tag;
	second->name = L"second";
	second->value = L"2";

	Tag* third = new Tag;
	third->name = L"third";
	
	Tag* thirdName = new Tag;
	thirdName->name = L"name";
	thirdName->value = L"AMAZING";

	Tag* thirdSurname = new Tag;
	thirdSurname->name = L"surname";
	thirdSurname->value = L"WOOOOW!";

	third->children = { thirdName, thirdSurname };

	Tag* first2 = new Tag;
	first2->name = L"first";
	first2->value = L"4";

	Tag* fourth = new Tag;
	fourth->name = L"fourth";
	fourth->value = L"5";

	array->children = { first1, second, third, first2, fourth };

	root->children = { array };
	TagPtrSequence roots = { root };

	convertToJson(roots, filename);

	outputTestname(testname);
	outputThatTestConverted();

	for (auto& root : roots)
		freeTag(root, &root);
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