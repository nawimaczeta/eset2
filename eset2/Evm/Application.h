#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Memory.h"
#include "BitBuffer.h"
#include "Evm.h"

namespace Evm {

	struct Application {
		using ThreadList = vector<ThreadContext>;

		Application(Evm & evm);

		void run();
		Memory & dataMemory();
		const BitBuffer & programMemory() const;

	private:
		const BitBuffer _programMemory;
		Memory _dataMemory;
		ThreadList _threadList;

		BitBuffer _getProgramMemory(Evm & evm) const;
	};
}
