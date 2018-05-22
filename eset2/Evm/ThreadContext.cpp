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

namespace Evm {
	uint32_t ThreadContext::_currentThreadID = 0;

	// Consdtuctor for the main thread
	ThreadContext::ThreadContext(Application *application) :
		_id{ _currentThreadID++ },
		_thread{},
		_parent{ application },
		_programCounter{ 0 }
	{
		fill(begin(_registerList), end(_registerList), 0);
		_openTraceFile();
	}

	// Copy constructor for children threads
	ThreadContext::ThreadContext(const ThreadContext & caller, uint32_t address) :
		_id{ _currentThreadID++ },
		_thread{},
		_parent{ caller._parent},
		_programCounter{ address },
		_registerList{ caller._registerList }
	{
		_closeTraceFile();
	}

	void ThreadContext::run()
	{
		_isRunning = true;

		_thread = thread{ [&]() {
			//cout << "Thread " << _id << " is launched\n";
			while (_isRunning) {
				// Evm execution loop.
				// In each iteration the next instruction is being decided and executed.
				// Each instruction is converted to Operation class and executed.
				try {
					auto tmpProgramCounter = _programCounter;
					auto operation = Operation::makeOperation(_parent->programMemory(), _programCounter);
					operation->execute(*this);
					//string str = operation->trace(*this);
					//cout << str << "\n";
					//cout << *operation << "\n";
					_logTrace(tmpProgramCounter, operation->trace(*this));
				}
				catch (RuntimeError & e) {
					//throw ThreadError{ *this, e };
					cerr << "Thread " << id() << " error at: " << programCounter() << ": " << e.what() << "\n";
					return;
				}
			}
			//cout << "Thread " << _id << " is about to terminate\n";
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

	void ThreadContext::_openTraceFile()
	{
		bool trace = application()->configuartion().trace;
		if (trace) {
			string filename = application()->configuartion().evmFileName + "_thread_" + to_string(id()) + ".trace";
			_traceFile.open(filename, fstream::trunc);
			_traceFile << "Thread " << id() << " spawned\n";
		}
	}

	void ThreadContext::_closeTraceFile()
	{
		bool trace = application()->configuartion().trace;
		if (trace) {
			_traceFile.close();
			_traceFile << "Thread " << id() << " closed\n";
		}
	}

	void ThreadContext::_logTrace(uint32_t instructionAddress, string & msg)
	{
		bool trace = application()->configuartion().trace;
		if (trace) {
			ostringstream oss;
			oss << ":0x" << setfill('0') << setw(8) << hex << instructionAddress << ": ";
			_traceFile << oss.str() << msg << "\n";
		}
	}

}