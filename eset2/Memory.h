#pragma once

#include "stdafx.h"

struct Memory {
	Memory(uint64_t size) :
		_memory(size)
	{}

	void write(uint64_t address, Bytes & data);
	void write(uint64_t address, Bytes::iterator beg, Bytes::iterator end);
	Bytes read(uint64_t address, uint64_t size) const;

private:
	Bytes _memory;
	bool _outOfMemory(uint64_t address, uint64_t size) const;
};
