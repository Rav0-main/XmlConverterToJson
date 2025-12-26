#include <iostream>
#include "parsexml.hpp"

#define FILENAME "D:\\Temporary\\test.xml"

int main(void) {
	auto [roots, res] = getXmlTreesOf(FILENAME);
	if (!int(res))
		outputRoots(roots);
	else
		std::cout << "Error of code: " << int(res);
	
	return 0;
}