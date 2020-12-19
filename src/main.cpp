#include <stdio.h>

#include "HuffmanArchiver.h"
#include "CLI.h"

int main(int argc, char* argv[]) {
	char *reading = new char[BUFSIZ];
	char *writing = new char[BUFSIZ];
	try
	{
		CLI parser;
		HuffmanArchiver archiver;
		FileSizeTracker track;
		parser.parse(argc, argv);
		std::ifstream in;
		std::ofstream out;
		in.rdbuf()->pubsetbuf(reading, sizeof(reading));
		out.rdbuf()->pubsetbuf(writing, sizeof(writing));
		in.open(parser.get_input());
		out.open(parser.get_output());
		if (parser.get_flag() == 1)
		{
			track = archiver.compress(in, out);
 			std::cout << track.getOriginalSize() << std::endl << track.getCompressedSize() << std::endl << track.getExtraSize() << std::endl;
		}
		else
		{
			track = archiver.extract(in, out);
 			std::cout << track.getOriginalSize() << std::endl << track.getCompressedSize() << std::endl << track.getExtraSize() << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
	delete [] reading;
	delete [] writing;
}
