#include "stdafx.h"
#include "Application.h"
#include "OperationFactory.h"

namespace Evm {
	uint32_t ThreadContext::_currentThreadID = 0;

	ThreadContext::ThreadContext(Application *application) :
		_id{ _currentThreadID++ },
		_thread{},
		_parent{ application },
		_programCounter{ 0 }
	{
		fill(begin(_registerList), end(_registerList), 0);
	}

	ThreadContext::ThreadContext(const ThreadContext & tc, uint32_t address) :
		_id{ _currentThreadID++ },
		_thread{},
		_parent{tc._parent},
		_programCounter{ address },
		_registerList{ tc._registerList }
	{
	}

	void ThreadContext::run()
	{
		_isRunning = true;

		_thread = thread{ [&]() {
			cout << "Thread " << _id << " is launched\n";
			while (_isRunning) {
				try {
					auto tmpProgramCounter = _programCounter;
					auto operation = Operation::makeOperation(_parent->programMemory(), tmpProgramCounter);
					_programCounter = tmpProgramCounter;
					operation->execute(*this);
				}
				catch (RuntimeError & e) {
					throw ThreadError{ *this, e };
				}
			}
			cout << "Thread " << _id << " is about to terminate\n";
		} };
	}

	void ThreadContext::join()
	{
		_thread.join();
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

	void ThreadContext::programCounter(uint32_t newValue)
	{
		_programCounter = newValue;
	}

	void ThreadContext::push(uint32_t value)
	{
		_callStack.push(value);
	}

	uint32_t ThreadContext::pop()
	{
		uint32_t value = _callStack.top();
		_callStack.pop();
		return value;
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