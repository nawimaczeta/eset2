//! @file	RuntimeError.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	EVM exceptions
#pragma once

#include "stdafx.h"

namespace Evm {
	//! @brief Base class for Evm exceptions
	struct RuntimeError : public runtime_error {
		RuntimeError(const string& _Message) :
			runtime_error(_Message)
		{}
	};

	//! @brief Operation is not implemented yet
	struct NotImplementedOperationRuntimeError : RuntimeError {
		NotImplementedOperationRuntimeError() :
			RuntimeError{ "Opcode not implemented" }
		{}
	};

	//! @brief Operation is not supported
	struct UnknownOperationRuntimeError : RuntimeError {
		UnknownOperationRuntimeError() :
			RuntimeError{ "Unknown opcode" }
		{}
	};

	//! @brief Thread with given id doesn't exist
	struct UnknownThreadRuntimeError : RuntimeError {
		UnknownThreadRuntimeError(uint64_t threadID) :
			RuntimeError{ "Thread " + to_string(threadID) + " doesn't exist" }
		{}
	};

	//! @brief Lock with given id doesn't exist
	struct BadLockIDRuntimeError : RuntimeError {
		BadLockIDRuntimeError(uint64_t id) :
			RuntimeError{ "Lock with id " + to_string(id) + " doesn't exist" }
		{}
	};

	//! @brief Evm file parsing error
	struct EvmFileParseRuntimeError : RuntimeError {
		EvmFileParseRuntimeError(const string & msg) :
			RuntimeError{ "Error while parsing evm file: " + msg }
		{}
	};

	//! @brief Capture configuration from command line error
	struct CliConfigurationRuntimeError : RuntimeError {
		CliConfigurationRuntimeError(const string & msg) :
			RuntimeError{ "Error while parsing arguments from cli: " + msg }
		{}
	};

	//! @brief Error while accessing input file
	struct InputFileRuntimeError : RuntimeError {
		InputFileRuntimeError(const string & filename, const string & msg) :
			RuntimeError{ "Input file " + filename + " error: " + msg }
		{}
		InputFileRuntimeError(const string & msg) :
			RuntimeError{ "Input file error: " + msg }
		{}
	};

	//! @brief Bef register index
	struct BadRegisterRuntimeError : RuntimeError {
		BadRegisterRuntimeError(uint8_t regIndex) :
			RuntimeError{ "Bad register index: " + to_string(regIndex) }
		{}
	};

	//! @brief Call stack error
	struct StackRuntimeError : RuntimeError {
		StackRuntimeError() :
			RuntimeError{ "Call stack error" }
		{}
	};

	//! @brief Program memory exceeds 32 bit size
	struct ProgramMemoryExceedBusWidthRuntimeError : RuntimeError {
		ProgramMemoryExceedBusWidthRuntimeError() :
			RuntimeError{ "Program memory out of range. Instruction address bus is 32-bit, code size is greater." }
		{}
	};

	//! @brief Trying to access data out of program memory
	struct ProgramMemoryOutOfRangeRuntimeError : RuntimeError {
		ProgramMemoryOutOfRangeRuntimeError() :
			RuntimeError{ "Trying to access data out of program memory" }
		{}
	};

	//! @brief Trying to access data out of data memory.
	struct DataMemoryOutOfRangeRuntimeError : RuntimeError {
		DataMemoryOutOfRangeRuntimeError(string & msg) :
			RuntimeError{ "Trying to access data out of data memory. " + msg }
		{}
	};

	//! @brief Trying to write to read only argument
	struct WriteToConstRuntimeError : RuntimeError {
		WriteToConstRuntimeError() :
			RuntimeError{ "Trying to write to constant argument (address or constant)" }
		{}
	};
}
