#pragma once

#include "stdafx.h"

namespace Evm {

	struct RuntimeError : public runtime_error {
		RuntimeError(const string& _Message) :
			runtime_error(_Message)
		{}
	};

	struct UnknownOperationRuntimeError : RuntimeError {
		UnknownOperationRuntimeError() :
			RuntimeError{ "Unknown operation" }
		{}
	};

	struct InstructionRuntimeError : RuntimeError {
		//InstructionRuntimeError()
	};
}
