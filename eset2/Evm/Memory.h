#pragma once

#include "stdafx.h"

namespace Evm {
	struct Memory {
		Memory(uint64_t size) :
			_memory(size)
		{}

		void write(uint64_t address, const Bytes & data);
		void write(uint64_t address, Bytes::const_iterator beg, Bytes::const_iterator end);
		Bytes read(uint64_t address, uint64_t size) const;

	private:
		Bytes _memory;
		bool _outOfMemory(uint64_t address, uint64_t size) const;
	};
}