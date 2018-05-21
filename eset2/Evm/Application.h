#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Memory.h"
#include "BitBuffer.h"
#include "Evm.h"

namespace Evm {

	struct CliConfiguration {
		string evmFileName;
		string inputFileName;
		bool inputFileIsGiven;
	};

	struct Application {
		using ThreadContexPtr = unique_ptr<ThreadContext>;
		using ThreadList = vector<ThreadContexPtr>;
		using LockList = map<uint64_t, mutex>;

		Application(CliConfiguration & config);

		void run();
		void wait();
		uint64_t runNewThread(ThreadContext & caller, uint32_t address);
		void joinThread(uint64_t threadId);
		void lock(uint64_t lockID);
		void unlock(uint64_t lockID);
		Memory & dataMemory();
		const BitBuffer & programMemory() const;

	private:
		unique_ptr<Evm> _evm;
		const BitBuffer _programMemory;
		Memory _dataMemory;
		ThreadList _threadList;
		LockList _lockList;

		unique_ptr<Evm> _parseEvmFile(const CliConfiguration & config) const;
		BitBuffer _extractProgramMemory(const Evm & evm) const;
	};

	void getCliConfiguration(int argc, char** argv, CliConfiguration & cliConfig);
	void getCliConfigurationHardcoded(CliConfiguration & cliConfig);
}
