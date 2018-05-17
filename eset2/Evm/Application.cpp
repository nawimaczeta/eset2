
#include "stdafx.h"
#include "Application.h"

namespace Evm {
	Application::Application(Evm & evm) :
		_programMemory{ _getProgramMemory(evm) },
		_dataMemory{ evm.header.dataSize }
	{
		// Copy initialized data to beginning of data memory
		auto initDataIts = File::extractInitializedData(evm);
		_dataMemory.write(0, initDataIts.first, initDataIts.second);
	}

	void Application::run()
	{

	}

	BitBuffer Application::_getProgramMemory(Evm & evm)
	{
		auto its = File::extractCode(evm);
		return{ its.first, its.second };
	}

}