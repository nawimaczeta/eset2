#pragma once

#include "stdafx.h"
#include "Memory.h"
#include "BitBuffer.h"
#include "Evm.h"

namespace Evm {

	struct Application {
		Application(Evm & evm);

		void run();
	private:
		const BitBuffer _programMemory;
		Memory _dataMemory;

		BitBuffer _getProgramMemory(Evm & evm);
	};
}
