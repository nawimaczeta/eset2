#include "stdafx.h"
#include "Application.h"
#include "OperationFactory.h"

namespace Evm {

	ThreadContext::ThreadContext(Application *application) :
		_id{ _currentThreadID++ },
		_parent{ application },
		_programCounter{ 0 }
	{
		fill(begin(_registerList), end(_registerList), 0);
	}

	ThreadContext::ThreadContext(const ThreadContext & tc, uint32_t address) :
		_id{ _currentThreadID++ },
		_parent{tc._parent},
		_programCounter{ address },
		_registerList{ tc._registerList }
	{
	}

	void ThreadContext::run()
	{
		_isRunning = true;

		while (_isRunning) {
			auto operation = Operation::makeOperation(_parent->programMemory(), _programCounter);
		}
	}

	void ThreadContext::reg(uint8_t index, uint64_t value)
	{
		if (index > _registerList.size()) {
			throw runtime_error{ "Bad register index " + to_string(index) };
		}

		_registerList.at(index) = value;
	}

	uint64_t ThreadContext::reg(uint8_t index) const
	{
		if (index > _registerList.size()) {
			throw runtime_error{ "Bad register index " + to_string(index) };
		}

		return _registerList.at(index);
	}

	Application * ThreadContext::application()
	{
		return _parent;
	}

}