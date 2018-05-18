#include "stdafx.h"
#include "Application.h"
#include "OperationFactory.h"

namespace Evm {
	uint32_t ThreadContext::_currentThreadID = 0;

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
			try {
				auto tmpProgramCounter = _programCounter;
				auto operation = Operation::makeOperation(_parent->programMemory(), tmpProgramCounter);
				operation->execute(*this);
				_programCounter = tmpProgramCounter;
			}
			catch (RuntimeError & e) {
				throw ThreadError{ *this, e };
			}
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

	uint32_t ThreadContext::id() const
	{
		return _id;
	}

	uint32_t ThreadContext::programCounter() const
	{
		return _programCounter;
	}

	Application * ThreadContext::application()
	{
		return _parent;
	}

	void ThreadContext::terminate()
	{
		_isRunning = false;
	}

}