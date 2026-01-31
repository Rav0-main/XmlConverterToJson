#include <iostream>
#include <cstring>
#include <string>
#include "tag.hpp"
#include "parsexml.hpp"
#include "tojson.hpp"
#include "help.hpp"
#include "config.hpp"

static const std::string HELP_COMMAND = "help";
static const std::string JSON = ".json";

inline static void outputStatusError(const ParsingStatus& status);
inline static void outputStatusError(const ConvertingStatus& status);
inline static void getFilenameWithExtension(
	const std::string& newExtension, const std::string& filename,
	std::string& newFilename
);

int main(const int argc, const char* argv[]) {
	if (argc == 1) {
		std::cout << "Choose xml-files to convert." << std::endl;
		std::cout << "For information, check: " UTILITY_NAME " " + HELP_COMMAND 
					  << std::endl;
	}

	else if (argc != 2 && !strcmp(argv[argc - 1], HELP_COMMAND.data())) {
		std::cout << "To use help-page need write one command - " + HELP_COMMAND + "."
					  << std::endl;
		std::cout << "For information, check: " UTILITY_NAME " " + HELP_COMMAND
			<< std::endl;
	}

	else if (!strcmp(argv[1], HELP_COMMAND.data())) {
		outputHelpPage();
	}

	else {
		std::string filename;
		std::string jsonFilename;
		TagPtrSequence roots;

		for (int i = 1; i < argc; ++i) {
			filename = std::string(argv[i]);
			std::cout << i << ") "
						  << "Converting \"" << filename << "\"..." << std::endl;

			const ParsingStatus parsingStatus = getXmlRootsOf(filename, roots);
			
			if (parsingStatus.code != ParsingStatusCode::Success) {
				std::cerr << "While parsing file \"" << filename << "\" throwed error: " << std::endl;
				outputStatusError(parsingStatus);
			}
			else {
				getFilenameWithExtension(JSON, filename, jsonFilename);
				
				const ConvertingStatus convertingStatus = convertToJson(roots, jsonFilename);
				if (convertingStatus.code != ConvertingStatusCode::Success)
					outputStatusError(convertingStatus);

				for (Tag* root : roots)
					freeTag(root, &root);

				roots.clear();

				std::cout << '\"' << filename << "\" converted to \"" << jsonFilename << "\"." << std::endl;
			}
			if(i+1 != argc)
				std::cout << std::endl;
		}
	}

	return 0;
}

static void outputStatusError(const ParsingStatus& status) {
	if (status.code != ParsingStatusCode::UnknownError)
		std::wcerr << status.msg << std::endl;
	else
		std::wcerr << L"Unknown error." << std::endl;
}

static void outputStatusError(const ConvertingStatus& status) {
	if (status.code != ConvertingStatusCode::UnknownError)
		std::cerr << status.msg << std::endl;
	else
		std::cerr << "Unknown error." << std::endl;
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