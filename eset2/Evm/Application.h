#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Memory.h"
#include "BitBuffer.h"
#include "Evm.h"

namespace Evm {

	struct Application {
		using ThreadContexPtr = unique_ptr<ThreadContext>;
		using ThreadList = vector<ThreadContexPtr>;

		Application(Evm & evm);

		void run();
		void join();
		uint64_t runNewThread(ThreadContext & caller, uint32_t address);
		void joinThread(uint64_t threadId);
		Memory & dataMemory();
		const BitBuffer & programMemory() const;

	private:
		const BitBuffer _programMemory;
		Memory _dataMemory;
		ThreadList _threadList;

		BitBuffer _extractProgramMemory(Evm & evm) const;
	};
}
