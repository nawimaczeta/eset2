
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
		// create first thread
		_threadList.emplace_back(this);
		_threadList.at(0).run();
	}

	Memory & Application::dataMemory()
	{
		return _dataMemory;
	}

	const BitBuffer & Application::programMemory() const
	{
		return _programMemory;
	}

	BitBuffer Application::_getProgramMemory(Evm & evm)
	{
		auto its = File::extractCode(evm);
		return{ its.first, its.second };
	}

}