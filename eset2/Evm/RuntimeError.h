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
}
