// eset2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Evm/Evm.h"
#include "Evm/Application.h"


int main()
{
	//const string EVM_FILE_NAME{ "in.evm" };
	//const string EVM_FILE_NAME{ "math.evm" };
	//const string EVM_FILE_NAME{ "memory.evm" };
	//const string EVM_FILE_NAME{ "xor.evm" };
	const string EVM_FILE_NAME{ "xor-with-stack-frame.evm" };

	try {
		auto evm = Evm::File::makeEvmFromFile(EVM_FILE_NAME);
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

