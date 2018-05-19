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

	struct UnknownThreadRuntimeError : RuntimeError {
		UnknownThreadRuntimeError(uint64_t threadID) :
			RuntimeError{ "Thread " + to_string(threadID) + " doesn't exist" }
		{}
	};

	struct BadLockIDRuntimeError : RuntimeError {
		BadLockIDRuntimeError(uint64_t id) :
			RuntimeError{ "Lock with id " + to_string(id) + " doesn't exist" }
		{}
	};
}
