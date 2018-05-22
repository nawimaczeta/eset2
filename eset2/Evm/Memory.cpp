#include "stdafx.h"
#include "Memory.h"

namespace Evm {
	namespace Utils {
		void Memory::write(uint64_t address, const Bytes & data)
		{
			if (_outOfMemory(address, data.size())) {
				throw out_of_range("Writing beyound memory. Memory size :" +
					to_string(_memory.size()) + " address: " + to_string(address) +
					" size: " + to_string(data.size()));
			}

			copy(begin(data), end(data), begin(_memory) + address);
		}

		void Memory::write(uint64_t address, Bytes::const_iterator beg, Bytes::const_iterator end)
		{
			auto dataSize = distance(beg, end);
			if ((dataSize + address) > _memory.size()) {
				throw out_of_range("Writing beyound memory. Memory size :" +
					to_string(_memory.size()) + " address: " + to_string(address) +
					" size: " + to_string(dataSize));
			}

			copy(beg, end, begin(_memory) + address);
		}

		Bytes Memory::read(uint64_t address, uint64_t size) const
		{
			if (_outOfMemory(address, size)) {
				throw out_of_range("Reading beyound memory. Memory size :" +
					to_string(_memory.size()) + " address: " + to_string(address) +
					" size: " + to_string(size));
			}

			Bytes res(begin(_memory) + address, begin(_memory) + address + size);
			return res;
		}

		bool Memory::_outOfMemory(uint64_t address, uint64_t size) const
		{
			if ((address + size) > _memory.size()) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}