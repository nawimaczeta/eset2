#pragma once

#include "stdafx.h"
#include "Memory.h"
#include "BitBuffer.h"
#include "Evm.h"
#include "ThreadContext.h"

namespace Evm {

	struct Application {
		using ThreadList = vector<ThreadContext>;

		Application(Evm & evm);

		void run();
	private:
		const BitBuffer _programMemory;
		Memory _dataMemory;
		ThreadList _threadList;

		BitBuffer _getProgramMemory(Evm & evm);
	};
}
