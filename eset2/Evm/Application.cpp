
#include "stdafx.h"
#include "Application.h"

namespace Evm {
	Application::Application(Evm & evm) :
		_programMemory{ _extractProgramMemory(evm) },
		_dataMemory{ evm.header.dataSize }
	{
		// Copy initialized data to beginning of data memory
		auto initDataIts = File::extractInitializedData(evm);
		_dataMemory.write(0, initDataIts.first, initDataIts.second);
	}

	void Application::run()
	{
		cout << "Application: spawn main thread\n";
		// create first thread
		_threadList.emplace_back(make_unique<ThreadContext>(this));
		_threadList.at(0)->run();
		cout << "Application: main thread is alive!\n";
	}

	void Application::join()
	{
		// join main thread
		cout << "Application: joining for main thread\n";
		_threadList.at(0)->join();
		cout << "Application: main thread is over\n";
	}

	uint64_t Application::runNewThread(ThreadContext & caller, uint32_t address)
	{
		// create new Thread from the caller
 		_threadList.emplace_back(make_unique<ThreadContext>(caller, address));
		uint64_t newThreadId = _threadList.size() - 1;
		_threadList.at(newThreadId)->run();
		return newThreadId;
	}

	void Application::joinThread(uint64_t threadId)
	{
		try {
			ThreadContext & thread = *_threadList.at(threadId);
			thread.join();
		}
		catch (out_of_range & e) {
			(void)e;
			throw UnknownThreadRuntimeError(threadId);
		}
	}

	Memory & Application::dataMemory()
	{
		return _dataMemory;
	}

	const BitBuffer & Application::programMemory() const
	{
		return _programMemory;
	}

	BitBuffer Application::_extractProgramMemory(Evm & evm) const
	{
		auto its = File::extractCode(evm);
		return{ its.first, its.second };
	}

}