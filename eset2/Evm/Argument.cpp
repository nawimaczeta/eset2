#include "stdafx.h"
#include "Argument.h"
#include "Application.h"

namespace Evm {
	namespace Argument {
		/*
		Acquire argument form the bit stram
		*/
		IArgument * getArgument(const BitBuffer & bb, uint32_t & offset)
		{
			IArgument * arg = nullptr;

			auto argType = bb.getU8(offset, 1);
			if (argType == 0) {
				// Argument - index of a register
				auto regIndex = bb.getU8(offset + 1, 4, true);

				offset += 5;
				arg = new RegisterArgument{ regIndex };
			}
			else {
				// Memory argument
				auto memorySize = bb.getU8(offset + 1, 2);
				auto regIndex = bb.getU8(offset + 3, 4, true);
				offset += 7;

				switch (memorySize) {
				case 0:
					// byte access
					arg = new MemoryBYTEArgument(regIndex);
					break;
				default:
					// TODO
					arg = new TmpArgument(12);
				}
			}

			return arg;
		}

		/*
		Acquire constant form the bit stram
		*/
		uint64_t getConstant(const BitBuffer & bb, uint32_t & offset)
		{
			//if (offset + 64 > bb.size()) {
			//	throw runtime_error("Unexpected end of file while loading constant");
			//}

			auto res = bb.getU64(offset, 64, true);
			offset += 64;
			return res;
		}

		/*
		Acquire instruction address form the bit stream
		*/
		uint32_t getAddress(const BitBuffer & bb, uint32_t & offset)
		{
			//if (offset + 32 > bb.size()) {
			//	throw runtime_error("Unexpected end of file while loading address");
			//}

			auto res = bb.getU32(offset, 32, true);
			offset += 32;
			return res;
		}


		uint64_t RegisterArgument::getValue(ThreadContext & thread) const
		{
			return thread.reg(_regIndex);
		}

		void RegisterArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			thread.reg(_regIndex, value);
		}

		MemoryBYTEArgument::MemoryBYTEArgument(uint8_t regIndex) :
			_regIndex{ regIndex }
		{}

		uint64_t MemoryBYTEArgument::getValue(ThreadContext & thread) const
		{
			auto memory = thread.application()->dataMemory();
			uint64_t address = thread.reg(_regIndex);

			Bytes data = memory.read(address, 1);

			return static_cast<uint64_t>(data.at(0));
		}
		void MemoryBYTEArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			auto memory = thread.application()->dataMemory();
			uint64_t address = thread.reg(_regIndex);

			Byte byte = static_cast<Byte>(value);
			Bytes data{ byte };
			memory.write(address, data);
		}
	}
}
