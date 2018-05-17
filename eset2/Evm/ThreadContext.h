#pragma once

//#include "Application.h"

namespace Evm {

	struct Application;

	struct ThreadContext {
		ThreadContext(Application *application);
		ThreadContext(const ThreadContext & tc, uint32_t address);
		void run();

		void reg(uint8_t index, uint64_t value);
		uint64_t reg(uint8_t index) const;
		Application *application();
	private:
		uint32_t _id;
		Application * _parent;
		uint32_t _programCounter;
		array<uint64_t, 16> _registerList;
		stack<uint32_t> _stack;
		bool _isRunning = false;

		static uint32_t _currentThreadID;
	};
}
