#pragma once

#include "stdafx.h"

struct BitBuffer {
	BitBuffer(Bytes & data);
	BitBuffer(Bytes::iterator begin, Bytes::iterator end);

	uint64_t get(uint32_t bitAddress, uint32_t size, bool reversed = false) const;
private:
	const Bytes _data;
};
