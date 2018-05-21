// eset2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Evm/Application.h"
#include "Evm/RuntimeError.h"

int main(int argc, char** argv)
{
	try {
		Evm::CliConfiguration cliConfig;				
		//getCliConfiguration(argc, argv, cliconfig);	// get configuration from cli
		getCliConfigurationHardcoded(cliConfig);			// hardcoded configuration - for tests

		Evm::Application app{ cliConfig };
		app.run();
		app.wait();
	}
	catch (Evm::RuntimeError & e) {
		cout << e.what();
	}

	getchar();
	getchar();
    return 0;
}
