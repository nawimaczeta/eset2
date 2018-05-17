#pragma once

#include "stdafx.h"
#include "BitBuffer.h"
#include "Evm.h"

namespace Evm {

	struct Application {
		Application(Evm & evm) :
			_instructions{ _getInstructions(evm) }
		{}
	private:
		const BitBuffer _instructions;

		BitBuffer _getInstructions(Evm & evm);
	};
}
