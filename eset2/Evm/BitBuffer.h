#pragma once

#include "stdafx.h"

namespace Evm {
	struct BitBuffer {
		BitBuffer(const Bytes & data);
		BitBuffer(Bytes::const_iterator begin, Bytes::const_iterator end);

		uint64_t getU64(uint32_t bitAddress, uint32_t size, bool reversed = false) const;
		uint32_t getU32(uint32_t bitAddress, uint32_t size, bool reversed = false) const;
		uint16_t getU16(uint32_t bitAddress, uint32_t size, bool reversed = false) const;
		uint8_t getU8(uint32_t bitAddress, uint32_t size, bool reversed = false) const;
		uint32_t size() const;
	private:
		const Bytes _data;
	};
}