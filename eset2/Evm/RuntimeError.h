#pragma once

#include "stdafx.h"

namespace Evm {

	class RuntimeError : public runtime_error {
		RuntimeError(const string& _Message) :
			runtime_error(_Message)
		{}
	};

	class InstructionRuntimeError : public RuntimeError {
		//InstructionRuntimeError()
	};
}
