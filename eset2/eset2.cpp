// eset2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

namespace Evm {

	class RuntimeError : public runtime_error {
		RuntimeError(const string& _Message) :
			runtime_error(_Message)
		{}
	};

	class InstructionRuntimeError : public RuntimeError {
		//InstructionRuntimeError()
	};

	struct BitBuffer {
		BitBuffer(Bytes & data);
		BitBuffer(Bytes::iterator begin, Bytes::iterator end);

		uint64_t get(uint32_t bitAddress, uint32_t size, bool reversed = false) const;
	private:
		const Bytes _data;	// TODO const?
	};

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

	struct Application {

	private:
		BitBuffer _instructions;
	};

}

int main()
{
    return 0;
}

