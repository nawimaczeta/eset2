#include "stdafx.h"
#include "Argument.h"
#include "Application.h"

namespace Evm {
	namespace Argument {
		union ConvertUnion {
			uint64_t u64;
			uint32_t u32[2];
			uint16_t u16[4];
			uint8_t u8[8];
		};

		/*
		Acquire argument form the bit stram
		*/
		IArgument * getArgument(const Utils::BitBuffer & bb, uint32_t & offset)
		{
			IArgument * arg = nullptr;

			try {
				auto argType = bb.getU8(offset, 1);
				if (argType == 0) {
					// Argument - index of a register
					auto regIndex = bb.getU8(offset + 1, 4, true);

					offset += 5;
					arg = new RegisterArgument{ regIndex };
				}
				else {
					// Memory argument
					auto memorySize = bb.getU8(offset + 1, 2, true);
					auto regIndex = bb.getU8(offset + 3, 4, true);
					offset += 7;

					switch (memorySize) {
					case 0:
						// byte access
						arg = new MemoryBYTEArgument(regIndex);
						break;
					case 1:
						// word access
						arg = new MemoryWORDArgument(regIndex);
						break;
					case 2:
						// dword access
						arg = new MemoryDWORDArgument(regIndex);
						break;
					case 3:
						// qword access
						arg = new MemoryQWORDArgument(regIndex);
						break;
					}
				}
			}
			catch (out_of_range & e) {
				(void)e;
				throw ProgramMemoryOutOfRangeRuntimeError{};
			}
			catch (RuntimeError & e) {
				throw e;
			}

			return arg;
		}

		/*
		Acquire constant form the bit stram
		*/
		uint64_t getConstant(const Utils::BitBuffer & bb, uint32_t & offset)
		{
			try {
				auto res = bb.getU64(offset, 64, true);
				offset += 64;
				return res;
			}
			catch (out_of_range & e) {
				(void)e;
				throw ProgramMemoryOutOfRangeRuntimeError{};
			}
		}

		/*
		Acquire instruction address form the bit stream
		*/
		uint32_t getAddress(const Utils::BitBuffer & bb, uint32_t & offset)
		{
			try {
				auto res = bb.getU32(offset, 32, true);
				offset += 32;
				return res;
			}
			catch (out_of_range & e) {
				(void)e;
				throw ProgramMemoryOutOfRangeRuntimeError{};
			}
		}


		uint64_t RegisterArgument::getValue(ThreadContext & thread) const
		{
			return thread.reg(_regIndex);
		}

		void RegisterArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			thread.reg(_regIndex, value);
		}

		uint64_t MemoryBYTEArgument::getValue(ThreadContext & thread) const
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				Bytes data = memory.read(address, 1);

				return static_cast<uint64_t>(data.at(0));
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{e.what()} };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}
		void MemoryBYTEArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				Byte byte = static_cast<Byte>(value);
				Bytes data{ byte };
				memory.write(address, data);
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}

		uint64_t MemoryWORDArgument::getValue(ThreadContext & thread) const
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				Bytes data = memory.read(address, 2);

				ConvertUnion cu;
				cu.u8[0] = data.at(0);
				cu.u8[1] = data.at(1);

				return static_cast<uint64_t>(cu.u16[0]);
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}

		void MemoryWORDArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				ConvertUnion cu;
				cu.u64 = value;
				Byte byte0 = static_cast<Byte>(cu.u8[0]);
				Byte byte1 = static_cast<Byte>(cu.u8[1]);

				Bytes data{ byte0, byte1 };
				memory.write(address, data);
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			} 
			catch (RuntimeError & e) {
				throw e;
			}
		}

		uint64_t MemoryDWORDArgument::getValue(ThreadContext & thread) const
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				Bytes data = memory.read(address, 4);

				ConvertUnion cu;
				cu.u8[0] = data.at(0);
				cu.u8[1] = data.at(1);
				cu.u8[2] = data.at(2);
				cu.u8[3] = data.at(3);

				return static_cast<uint64_t>(cu.u32[0]);
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}

		void MemoryDWORDArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				ConvertUnion cu;
				cu.u64 = value;
				Byte byte0 = static_cast<Byte>(cu.u8[0]);
				Byte byte1 = static_cast<Byte>(cu.u8[1]);
				Byte byte2 = static_cast<Byte>(cu.u8[2]);
				Byte byte3 = static_cast<Byte>(cu.u8[3]);

				Bytes data{ byte0, byte1, byte2, byte3 };
				memory.write(address, data);
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}
		uint64_t MemoryQWORDArgument::getValue(ThreadContext & thread) const
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				Bytes data = memory.read(address, 8);

				ConvertUnion cu;
				cu.u8[0] = data.at(0);
				cu.u8[1] = data.at(1);
				cu.u8[2] = data.at(2);
				cu.u8[3] = data.at(3);
				cu.u8[4] = data.at(4);
				cu.u8[5] = data.at(5);
				cu.u8[6] = data.at(6);
				cu.u8[7] = data.at(7);

				return cu.u64;
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}
		void MemoryQWORDArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			try {
				auto & memory = thread.application()->dataMemory();
				uint64_t address = thread.reg(_regIndex);

				ConvertUnion cu;
				cu.u64 = value;
				Byte byte0 = static_cast<Byte>(cu.u8[0]);
				Byte byte1 = static_cast<Byte>(cu.u8[1]);
				Byte byte2 = static_cast<Byte>(cu.u8[2]);
				Byte byte3 = static_cast<Byte>(cu.u8[3]);
				Byte byte4 = static_cast<Byte>(cu.u8[4]);
				Byte byte5 = static_cast<Byte>(cu.u8[5]);
				Byte byte6 = static_cast<Byte>(cu.u8[6]);
				Byte byte7 = static_cast<Byte>(cu.u8[7]);

				Bytes data{ byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 };
				memory.write(address, data);
			}
			catch (out_of_range & e) {
				throw DataMemoryOutOfRangeRuntimeError{ string{ e.what() } };
			}
			catch (RuntimeError & e) {
				throw e;
			}
		}
	}
}
