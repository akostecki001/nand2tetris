#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include "Assembler.h"
#include "FileHandler.h"

using namespace std;

/**
	Gets the verbose and 'very verbose' flags from the command line arguments.

	@param argc Number of command line arguments.

	@param argv Array of arguments.

	@param verbose Verbose flag. Its value will be affected by the arguments.

	@param veryVerbose 'Very verbose' flag. Its value will be affected by the arguments.

	@return Returns false if an unidentified flag is present in the arguments.
	True otherwise.
*/
bool getFlags(int argc, char** argv, bool& verbose, bool& veryVerbose)
{
	char c;
	verbose = veryVerbose = false;

	while ((c = getopt(argc, argv, "vV")) != -1) {

		switch (c) {

			case 'V':
				veryVerbose = true;

			case 'v':
				verbose = true;
				break;

			case '?':
				return false;

		}
	}

	return true;
}

/**
	Checks if the input string is the name of a regular file with the extension ".asm".

	@return True if the input string is the name of .asm file. False otherwise.
*/
bool isAsmFile(string input)
{
	if (!FileHandler::isFile(input)) {
		cerr << "error: input \"" << input << "\"is not a file" << endl;
		return false;
	}

	if (!FileHandler::hasExtension(input, ".asm")) {
		cerr << "error: input file must have .asm extension" << endl;
		return false;
	}

	return true;
}

/**

*/
int main(int argc, char** argv)
{
	if (argc == 1) {
		cerr << "usage: " << "hass" << " [-v|-V] input-filename.asm" << endl;
		cerr << "       -v verbose" << endl;
		cerr << "       -V very verbose" << endl;
		return 1;
	}

	bool verbose, veryVerbose;

	if (!getFlags(argc, argv, verbose, veryVerbose))
		return 1;

	string inputName(argv[argc - 1]);

	if (!isAsmFile(inputName))
		return 1;

	ifstream inputFile(inputName);

	if (!inputFile.good()) {
		cerr << "error: unable to open input stream" << endl;
		return 1;
	}

	string outputName(FileHandler::changeExtension(inputName, ".hack"));
	ofstream outputFile(outputName);

	if (!outputFile.good()) {
		cerr << "error: unable to open output stream" << endl;
		return 1;
	}

	Assembler hass(inputFile, inputName, outputFile, outputName, verbose, veryVerbose);
	hass.assemble();

	if (inputFile.is_open())
		inputFile.close();

	if (outputFile.is_open())
		outputFile.close();

	return 0;
}
