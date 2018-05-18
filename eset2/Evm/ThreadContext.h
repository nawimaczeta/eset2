#pragma once

#include "RuntimeError.h"

namespace Evm {

	struct Application;

	struct ThreadContext {
		ThreadContext(Application *application);
		ThreadContext(const ThreadContext & tc, uint32_t address);

		void run();
		void reg(uint8_t index, uint64_t value);
		uint64_t reg(uint8_t index) const;
		uint32_t id() const;
		uint32_t programCounter() const;
		Application *application();
		void terminate();
	private:
		uint32_t _id;
		Application * _parent;
		uint32_t _programCounter;
		array<uint64_t, 16> _registerList;
		stack<uint32_t> _stack;
		bool _isRunning = false;

		static uint32_t _currentThreadID;
	};

	struct ThreadError : runtime_error {
		ThreadError(ThreadContext & thread, RuntimeError & runtimeError) :
			runtime_error{ _message(thread, runtimeError)}
		{}

	private:
		string _message(ThreadContext & thread, RuntimeError & runtimeError) {
			ostringstream oss;
			oss << "Thread " << thread.id() << 
				" error at " << thread.programCounter() << ": " << 
				runtimeError.what() << "\n";
			return oss.str();
		}
	};
}
