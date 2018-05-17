
#include "stdafx.h"
#include "BitBuffer.h"

BitBuffer::BitBuffer(Bytes & data) :
	_data(data)
{}

BitBuffer::BitBuffer(Bytes::iterator begin, Bytes::iterator end) :
	_data(begin, end)
{}

uint64_t BitBuffer::get(uint32_t bitAddress, uint32_t size, bool reversed) const
{
	uint32_t currentByte = bitAddress / 8;
	uint32_t currentBit = bitAddress % 8;
	uint64_t res = 0;

	while (size > 0) {
		if (currentByte > _data.size()) {
			throw out_of_range{ "" };
		}

		Byte byte = _data.at(currentByte);
		uint8_t bitMask = 1 << (7 - currentBit);
		uint64_t bit;

		if (reversed) {
			bit = ((byte & bitMask) == 0) ? 0 : 0x8000000000000000u;
			res >>= 1;
		}
		else {
			bit = ((byte & bitMask) == 0) ? 0 : 1;
			res <<= 1;
		}

		res |= bit;

		size--;
		currentBit++;
		if (currentBit > 7) {
			currentBit = 0;
			currentByte++;
		}
	}
	return res;
}