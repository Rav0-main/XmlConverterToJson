#include <iostream>
#include "parsexml.hpp"

#define FILENAME "D:\\StudyProgrammingData\\CppProjects" \
								"\\FromXmlToJson\\test.xml"

int main(void) {
	auto [roots, res] = getXmlTreesOf(FILENAME);
	if (!int(res))
		outputRoots(roots, false);
	else
		std::cout << "Error of code: " << int(res);
	
	for (Node* root : roots)
		freeNode(root, &root);

	return 0;
}