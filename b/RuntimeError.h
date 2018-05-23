#pragma once

#include "stdafx.h"

namespace Evm {

	struct RuntimeError : public runtime_error {
		RuntimeError(const string& _Message) :
			runtime_error(_Message)
		{}
	};

	struct NotImplementedOperationRuntimeError : RuntimeError {
		NotImplementedOperationRuntimeError() :
			RuntimeError{ "Opcode not implemented" }
		{}
	};

	struct UnknownOperationRuntimeError : RuntimeError {
		UnknownOperationRuntimeError() :
			RuntimeError{ "Unknown opcode" }
		{}
	};

	struct UnknownThreadIDRuntimeError : RuntimeError {
		UnknownThreadIDRuntimeError(uint64_t threadID) :
			RuntimeError{ "Thread " + to_string(threadID) + " doesn't exist" }
		{}
	};

	struct BadLockIDRuntimeError : RuntimeError {
		BadLockIDRuntimeError(uint64_t id) :
			RuntimeError{ "Lock with id " + to_string(id) + " doesn't exist" }
		{}
	};

	struct EvmFileParseRuntimeError : RuntimeError {
		EvmFileParseRuntimeError(const string & msg) :
			RuntimeError{ "Error while parsing evm file: " + msg }
		{}
	};

	struct CliConfigurationRuntimeError : RuntimeError {
		CliConfigurationRuntimeError(const string & msg) :
			RuntimeError{ "Error while parsing arguments from cli: " + msg }
		{}
	};

	struct InputFileRuntimeError : RuntimeError {
		InputFileRuntimeError(const string & filename, const string & msg) :
			RuntimeError{ "Input file " + filename + " error: " + msg }
		{}
		InputFileRuntimeError(const string & msg) :
			RuntimeError{ "Input file error: " + msg }
		{}
	};

	struct BadRegisterRuntimeError : RuntimeError {
		BadRegisterRuntimeError(uint8_t regIndex) :
			RuntimeError{ "Bad register index: " + to_string(regIndex) }
		{}
	};

	struct StackRuntimeError : RuntimeError {
		StackRuntimeError() :
			RuntimeError{ "Call stack error" }
		{}
	};
}
