//! @file	BitBuffer.cpp
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! 
//! Definition of BitBuffer classes
#include "stdafx.h"
#include "BitBuffer.h"
#include "RuntimeError.h"

namespace Evm {
	namespace Utils {
		BitBuffer::BitBuffer(const Bytes & data) :
			_data(data)
		{}

		BitBuffer::BitBuffer(Bytes::const_iterator begin, Bytes::const_iterator end) :
			_data(begin, end)
		{}

		uint64_t BitBuffer::getU64(uint32_t bitAddress, uint32_t size, bool reversed) const
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

		uint32_t BitBuffer::getU32(uint32_t bitAddress, uint32_t size, bool reversed) const
		{
			uint32_t currentByte = bitAddress / 8;
			uint32_t currentBit = bitAddress % 8;
			uint32_t res = 0;

			while (size > 0) {
				if (currentByte > _data.size()) {
					throw out_of_range{ "" };
				}

				Byte byte = _data.at(currentByte);
				uint8_t bitMask = 1 << (7 - currentBit);
				uint32_t bit;

				if (reversed) {
					bit = ((byte & bitMask) == 0) ? 0 : 0x80000000u;
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

		uint16_t BitBuffer::getU16(uint32_t bitAddress, uint32_t size, bool reversed) const
		{
			uint32_t currentByte = bitAddress / 8;
			uint32_t currentBit = bitAddress % 8;
			uint16_t res = 0;

			while (size > 0) {
				if (currentByte > _data.size()) {
					throw out_of_range{ "" };
				}

				Byte byte = _data.at(currentByte);
				uint8_t bitMask = 1 << (7 - currentBit);
				uint16_t bit;

				if (reversed) {
					bit = ((byte & bitMask) == 0) ? 0 : 0x8000u;
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

		uint8_t BitBuffer::getU8(uint32_t bitAddress, uint32_t size, bool reversed) const
		{
			uint32_t currentByte = bitAddress / 8;
			uint32_t currentBit = bitAddress % 8;
			uint8_t res = 0;
			uint32_t tmpSize = size;

			while (tmpSize > 0) {
				if (currentByte > _data.size()) {
					throw out_of_range{ "" };
				}

				Byte byte = _data.at(currentByte);
				uint8_t bitMask = 1 << (7 - currentBit);
				uint8_t bit;

				if (reversed) {
					bit = ((byte & bitMask) == 0) ? 0 : 0x80u;
					res >>= 1;
				}
				else {
					bit = ((byte & bitMask) == 0) ? 0 : 1;
					res <<= 1;
				}

				res |= bit;

				tmpSize--;
				currentBit++;
				if (currentBit > 7) {
					currentBit = 0;
					currentByte++;
				}
			}

			if (reversed) {
				res >>= (8 - size);
			}
			return res;
		}

		uint32_t BitBuffer::size() const
		{
			uint64_t bitSize = 8 * _data.size();
			if (bitSize > numeric_limits<uint32_t>::max()) {
				throw ProgramMemoryExceedBusWidthRuntimeError{};
			}
			return static_cast<uint32_t>(bitSize);
		}
	}
}