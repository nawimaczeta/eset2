//! @file	main.cpp
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	EVM entry point

#include "stdafx.h"
#include "Evm/Evm.h"

//! @brief Entry point
//!
//! Application entry point
int main(int argc, char** argv)
{
	try {
		// get configuration from console
		Evm::CliConfiguration cliConfig;
#ifdef _DEBUG
		getCliConfiguration(cliConfig);		// hardcoded configuration - for tests
#else
		getCliConfiguration(argc, argv, cliConfig);		// get configuration from cli
#endif // _DEBUG

		// Run application, wait for execution
		Evm::Application app{ cliConfig };
		app.run();
		app.wait();
	}
	catch (Evm::RuntimeError & e) {
		cout << e.what();
	}

#ifdef _DEBUG
	getchar();
	getchar();
#endif // _DEBUG
    return 0;
}
