#include <iostream>
#include <vector>
#include "help.hpp"
#include "config.hpp"

#define DASH_LINE "-------------------------------------------------------------"

static void outputLinesWithOffset(
	const char offsetChar, const unsigned count,
	const std::vector<const char*> lines
);

void outputHelpPage(void) {
	std::cout << "HELP-PAGE: " << std::endl;
	std::cout << DASH_LINE << std::endl;

	std::cout << "* What?" << std::endl;
	outputLinesWithOffset(
		' ', 2u,
		{
			"The utility is needed to convert xml files to json format.",
			"",
			"EXAMPLE:",
			"Source xml-file:",
			"",
			"<main>",
			"\t<tag1>value 1</tag1>",
			"\t<tag2>",
			"\t\t<tag2.1>value 2.1</tag2.1>",
			"\t\t<tag2.2>value 2.2<tag2.2>",
			"\t<tag2>",
			"</main>",
			"",
			"Output json-file:",
			"{",
			"\t\"main\" : {",
			"\t\t\"tag1\" : \"value 1\",",
			"\t\t\"tag2\" : {",
			"\t\t\t\"tag2.1\" : \"value 2.1\",",
			"\t\t\t\"tag2.2\" : \"value 2.2\"",
			"\t\t}",
			"\t}",
			"}"
		}
	);
	std::cout << std::endl << DASH_LINE << std::endl;

	std::cout << "* How to use?" << std::endl;
	outputLinesWithOffset(
		' ', 2,
		{
			"* For converting:"
		}
	);
	outputLinesWithOffset(
		' ', 4,
		{
			UTILITY_NAME " \"filenames\"...",
			"For each of \"filenames\" convert xml-format to json-format.",
			"",
			"EXAMPLE:",
			"Need convert two xml-files in CWD: f1.xml and f2.xml.",
			"To use, need execute: " UTILITY_NAME " f1.xml f2.xml.",
			"After, if f1.xml and f2.xml - valid xml-files,",
			"will create new json-files in CWD: f1.json and f2.json."
		}
	);
	outputLinesWithOffset(
		' ', 2,
		{
			"* For documentation: "
		}
	);
	outputLinesWithOffset(
		' ', 4,
		{
			UTILITY_NAME " help"
		}
	);
	std::cout << std::endl << DASH_LINE << std::endl;

	std::cout << "* Thanks for use." << std::endl;
}

static void outputLinesWithOffset(
	const char offsetChar, const unsigned count,
	const std::vector<const char*> lines
) {
	for (const char* line : lines) {
		for (unsigned j = 0; j < count; ++j)
			std::cout << offsetChar;

		std::cout << line << std::endl;
	}
}