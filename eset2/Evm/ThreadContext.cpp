//! @file	ThreadContext.cpp
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	EVM application
//! 
//! Definition of ThreadContext class
#include "stdafx.h"
#include "OperationFactory.h"
#include "ThreadContext.h"
#include "RuntimeError.h"
#include "Application.h"
#include "Operation.h"
//#include "Trace.h"

namespace Evm {
	uint32_t ThreadContext::_currentThreadID = 0;

	// Consdtuctor for the main thread
	ThreadContext::ThreadContext(Application *application) :
		_id{ _currentThreadID++ },
		_thread{},
		_parent{ application },
		_programCounter{ 0 },
		_trace{ _traceFileName(), _parent->configuartion().trace }
	{
		// clear register list 
		fill(begin(_registerList), end(_registerList), 0);
	}

	// Copy constructor for children threads
	ThreadContext::ThreadContext(const ThreadContext & caller, uint32_t address) :
		_id{ _currentThreadID++ },
		_thread{},
		_parent{ caller._parent},
		_programCounter{ address },
		_registerList{ caller._registerList },
		_trace{ _traceFileName(), _parent->configuartion().trace }
	{}

	void ThreadContext::run()
	{
		_isRunning = true;

		_thread = thread{ [&]() {
			while (_isRunning) {
				// Evm execution loop.
				// In each iteration the next instruction is being decided and executed.
				// Each instruction is converted to Operation class and executed.
				try {
					auto tmpProgramCounter = _programCounter;
					auto operation = Operation::makeOperation(_parent->programMemory(), _programCounter);
					if (_parent->configuartion().trace) {
						_trace.log(tmpProgramCounter, operation->trace(*this));
					}
					
					operation->execute(*this);
				}
				catch (RuntimeError & e) {
					cerr << "Thread " << id() << " error at: " << programCounter() << ": " << e.what() << "\n";
					return;
				}
			}
		} };
	}

	void ThreadContext::join()
	{
		if (_thread.joinable()) {
			_thread.join();
		}
	}

	void ThreadContext::sleep(uint64_t ms)
	{
		this_thread::sleep_for(chrono::milliseconds(ms));
	}

	void ThreadContext::reg(uint8_t index, uint64_t value)
	{
		if (index >= _registerList.size()) {
			throw BadRegisterRuntimeError{ index };
		}

		_registerList.at(index) = value;
	}

	uint64_t ThreadContext::reg(uint8_t index) const
	{
		if (index >= _registerList.size()) {
			throw BadRegisterRuntimeError{ index };
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
		try {
			uint32_t value = _callStack.top();
			_callStack.pop();
			return value;
		}
		catch (...) {
			throw StackRuntimeError{};
		}
	}

	Application * ThreadContext::application()
	{
		return _parent;
	}

	void ThreadContext::terminate()
	{
		_isRunning = false;
	}

	string ThreadContext::_traceFileName() const
	{
		return _parent->configuartion().evmFileName +
			"_thread" + to_string(id()) + ".trace";
	}
}