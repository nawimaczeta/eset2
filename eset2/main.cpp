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
	//	TCLAP::ValueArg<std::string> filenameArg("i", "input_file", "Input file", false, "in", "filename");
	//	cmd.add(evmFilenameArg);
	//	cmd.add(filenameArg);

	//	cmd.parse(argc, argv);

	//	evmFilename = evmFilenameArg.getValue();
	//	filename = filenameArg.getValue();
	//}
	//catch (TCLAP::ArgException &e)  // catch any exceptions
	//{
	//	std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	//}

	//const string EVM_FILE_NAME{ "input/math.evm" };
	//const string EVM_FILE_NAME{ "input/memory.evm" };
	//const string EVM_FILE_NAME{ "input/xor.evm" };
	//const string EVM_FILE_NAME{ "input/xor-with-stack-frame.evm" };
	//const string EVM_FILE_NAME{ "input/fibonacci_loop.evm" };
	const string EVM_FILE_NAME{ "input/threadingBase.evm" };

	try {
		auto evm = Evm::File::makeEvmFromFile(EVM_FILE_NAME);
		//auto evm = Evm::File::makeEvmFromFile(evmFilename);
		Evm::File::validateEvm(*evm);
		cout << *evm;
		Evm::Application app{ *evm };
		app.run();
		app.join();
	}
	catch (exception & e) {
		cout << e.what();
	}

	getchar();
	getchar();
    return 0;
}

