//! @file	ThreadContext.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	EVM thread abstraction
//! 
//! ThreadContext class represents an instance of evm thread. Each thread has an unique ID and
//! exclusively a set of registers, program counter, call stack. Threads contain pointer
//! to a parent - Application, therefore they are capable of obtaining access to shared
//! resouces - program and data mamories, locks, and Application API.
#pragma once

//#include "Operation.h"
#include "RuntimeError.h"

//! @namespace Eva
//!
//! Main namespace of Evm library
namespace Evm {

	struct Application;

	//! @brief Evm thread class
	//!
	//! The class provides basic functionality of a single evm thread. Evm 
	//! appliation has at least one thread
	struct ThreadContext {
		//! @brief Basic constructor
		//!
		//! Usually used to create the first thread.
		//! @parent appliation Pointer to a parent
		ThreadContext(Application *application);

		//! @brief Copy constructor
		//!
		//! It is used to create new evm threads that are based on
		//! caller thread
		//! @param caller Reference to caller thread
		//! @param address Program counter value for the new thread
		ThreadContext(const ThreadContext & caller, uint32_t address);

		//! @brief Get Thread ID
		//!
		//! The function returns the thread ID
		//! @return Thread IS
		uint32_t id() const;

		//! @brief Run the thread
		//!
		//! The function launches the current thread. Essentially it spowns a system thread.
		//! A task operates in loop where the evm instructions are captured, decoded and executed.
		//! The exectution lasts until terminate() function is called or and error occurs.
		//! @note Non-blocking function
		void run();

		//! @brief Join the thread
		//!
		//! The sunction suspends until the thread is in exetution.
		//! @note Blocking function
		void join();

		//! @brief Sleep
		//!
		//! Suspend execution of the thread for given time
		//! @param ms Time in milliseconds
		//! @note Blocking function
		void sleep(uint64_t ms);

		// @brief Set value to a register
		//!
		//! Set a value to a register with given index
		//! @param index Index of the register (0-15)
		//! @param value New value
		//! @throw BadRegisterRuntimeError
		void reg(uint8_t index, uint64_t value);

		// @brief Get value of a register
		//!
		//! Get a value of a register with given index
		//! @param index Index of the register (0-15)
		//! @return value Value of the register
		//! @throw BadRegisterRuntimeError
		uint64_t reg(uint8_t index) const;

		//! @brief Get value in program counter
		//!
		//! @retrun Current value in program counter
		uint32_t programCounter() const;

		//! @brief Set value to program counter
		//!
		//! It is good for jump and call instructions
		//! @param newValue New value for Program Counter
		void programCounter(uint32_t newValue);

		//! @brief Push value to a stack
		//!
		//! Save return address on a stack
		//! @param value Value to push to the stack
		void push(uint32_t value);

		//! @breif Pop value from a stack
		//!
		//! Load return address form a stack
		//! @return Value from the top of the stuck
		uint32_t pop();

		//! @brief Get pointer to application
		//!
		//! Return pointer to a parent - application
		//! It is good way to achieve access to application shared data like other threads,
		//! locks and memory
		//! @return Pointer to parent application
		Application *application();

		//! @biref Terminate
		//!
		//! Terminate a thread
		void terminate();
	private:
		uint32_t _id;		//!< Thread unique ID
		thread _thread;		//!< System thread
		Application * _parent;		//!< Pointer to parent - application
		uint32_t _programCounter;	//!< Program Counter
		array<uint64_t, 16> _registerList;	//!< Register list
		stack<uint32_t> _callStack;		//!< Call stack
		bool _isRunning = false;		//!< The thread execution loop is running until
										//!< this variable is true

		static uint32_t _currentThreadID;

		ofstream _traceFile;

		void _openTraceFile();
		void _closeTraceFile();
		void _logTrace(uint32_t instructionAddress, string & msg);
	};

	struct ThreadError : runtime_error {
		ThreadError(ThreadContext & thread, RuntimeError & runtimeError) :
			runtime_error{ _message(thread, runtimeError)}
		{}

	private:
		string _message(ThreadContext & thread, RuntimeError & runtimeError) {
			ostringstream oss;
			oss << "Thread " << thread.id() << 
				" error at " << thread.programCounter() << ": " << 
				runtimeError.what() << "\n";
			return oss.str();
		}
	};
}
