// eset2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Evm/Evm.h"
#include "Evm/Application.h"
#include "tclap/CmdLine.h"

int main(int argc, char** argv)
{
	//string evmFilename;
	//string inputFilename;
	//string outputFilename;

	//try {
	//	TCLAP::CmdLine cmd("Command description message", ' ', "0.9");
	//	TCLAP::UnlabeledValueArg<string> evmFilenameArg("evm", "evm file name", true, "", "filename");
	//	TCLAP::ValueArg<std::string> inputFilenameArg("i", "input_file", "Input file", false, "in", "filename");
	//	TCLAP::ValueArg<std::string> outputFilenameArg("o", "output_file", "Output file", false, "out", "filename");
	//	cmd.add(evmFilenameArg);
	//	cmd.add(inputFilenameArg);
	//	cmd.add(outputFilenameArg);

	//	cmd.parse(argc, argv);

	//	evmFilename = evmFilenameArg.getValue();
	//	inputFilename = inputFilenameArg.getValue();
	//	outputFilename = outputFilenameArg.getValue();
	//}
	//catch (TCLAP::ArgException &e)  // catch any exceptions
	//{
	//	std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	//}

	//const string EVM_FILE_NAME{ "input/math.evm" };
	//const string EVM_FILE_NAME{ "input/memory.evm" };
	//const string EVM_FILE_NAME{ "input/xor.evm" };
	//const string EVM_FILE_NAME{ "input/xor-with-stack-frame.evm" };
	const string EVM_FILE_NAME{ "input/fibonacci_loop.evm" };

	try {
		auto evm = Evm::File::makeEvmFromFile(EVM_FILE_NAME);
		//auto evm = Evm::File::makeEvmFromFile(evmFilename);
		Evm::File::validateEvm(*evm);
		cout << *evm;
		Evm::Application app{ *evm };
		app.run();
	}
	catch (exception & e) {
		cout << e.what();
	}

	getchar();
	getchar();
    return 0;
}

