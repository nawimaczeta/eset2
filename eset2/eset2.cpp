// eset2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Evm/Evm.h"
#include "Evm/Application.h"


int main()
{
	const string EVM_FILE_NAME{ "in.evm" };

	try {
		auto evm = Evm::File::makeEvmFromFile(EVM_FILE_NAME);
		Evm::File::validateEvm(*evm);
		cout << *evm;
		Evm::Application app{ *evm };
	}
	catch (exception & e) {
		cout << e.what();
		getchar();
		return 1;
	}

	getchar();
    return 0;
}

