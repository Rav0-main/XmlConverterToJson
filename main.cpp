#include <iostream>
#include <cstring>
#include <string>
#include "node.hpp"
#include "parsexml.hpp"
#include "tojson.hpp"
#include "help.hpp"
#include "config.hpp"

#define HELP_COMMAND "help"
#define JSON_EXTENSION ".json"

static void outputXmlParsingError(const ParsingResult& result);
static void getFilenameWithExtension(
	const std::string& newExtension, const std::string& filename,
	std::string& newFilename
);

int main(const int argc, const char* argv[]) {
	if (argc == 1) {
		std::cout << "Choose xml-files to convert." << std::endl;
		std::cout << "For information, check: " UTILITY_NAME " " HELP_COMMAND 
					  << std::endl;
	}
	else if (argc != 2 && !strcmp(argv[argc - 1], HELP_COMMAND)) {
		std::cout << "To use help-page need write one command - " HELP_COMMAND "."
					  << std::endl;
		std::cout << "For information, check: " UTILITY_NAME " " HELP_COMMAND
			<< std::endl;
	}
	else if (!strcmp(argv[1], HELP_COMMAND)) {
		outputHelpPage();
	}
	else {
		std::string filename;
		std::string jsonFilename;
		for (int i = 1; i < argc; ++i) {
			filename = std::string(argv[i]);
			auto [trees, result] = getXmlTreesOf(filename);
			
			std::cout << i << ") ";
			if (int(result)) {
				std::cerr << "While parsing file \"" << filename << "\" throwed error: " << std::endl;
				outputXmlParsingError(result);
			}
			else {
				getFilenameWithExtension(JSON_EXTENSION, filename, jsonFilename);
				convertToJson(trees, jsonFilename);
				for (Node* root : trees)
					freeNode(root, &root);

				std::cout << '\"' << filename << "\" converted to \"" << jsonFilename << "\"." << std::endl;
			}
			if(i+1 != argc)
				std::cout << std::endl;
		}
	}
	return 0;
}

static void outputXmlParsingError(const ParsingResult& result) {
	switch (int(result)) {
	case 1:
		std::cerr << "File not found." << std::endl;
		break;
	case 2:
		std::cerr << "Closing tag name not equal with opening." << std::endl;
		break;
	case 3:
		std::cerr << "Tag name syntax is incorrect." << std::endl;
		break;
	case 4:
		std::cerr << "Found not closed tag name." << std::endl;
		break;
	default:
		std::cerr << "Unknown error." << std::endl;
	}
}

static void getFilenameWithExtension(
	const std::string& newExtension, const std::string& filename,
	std::string& newFilename
) {
	size_t dotLastIndex = filename.length()-1;

	for (; dotLastIndex > 0; --dotLastIndex)
		if (filename[dotLastIndex] == '.')
			break;

	if (dotLastIndex == 0 && filename[0] != '.')
		newFilename = filename + newExtension;

	else if (dotLastIndex == 0)
		newFilename = newExtension;

	else
		newFilename = filename.substr(0, dotLastIndex) + newExtension;
}