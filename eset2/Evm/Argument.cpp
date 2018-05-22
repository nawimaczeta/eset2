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
		ArgumentPtr getRegisterArgument(const Utils::BitBuffer & bb, uint32_t & offset)
		{
			ArgumentPtr arg = nullptr;

			try {
				auto argType = bb.getU8(offset, 1);
				if (argType == 0) {
					// Argument - index of a register
					auto regIndex = bb.getU8(offset + 1, 4, true);

					offset += 5;
					arg = make_unique<RegisterArgument>(regIndex);
				}
				else {
					// Memory argument
					auto memorySize = bb.getU8(offset + 1, 2, true);
					auto regIndex = bb.getU8(offset + 3, 4, true);
					offset += 7;

					switch (memorySize) {
					case 0:
						// byte access
						arg = make_unique<MemoryBYTEArgument>(regIndex);
						break;
					case 1:
						// word access
						arg = make_unique<MemoryWORDArgument>(regIndex);
						break;
					case 2:
						// dword access
						arg = make_unique<MemoryDWORDArgument>(regIndex);
						break;
					case 3:
						// qword access
						arg = make_unique<MemoryQWORDArgument>(regIndex);
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
		ArgumentPtr getConstantArgument(const Utils::BitBuffer & bb, uint32_t & offset)
		{
			try {
				auto value = bb.getU64(offset, 64, true);
				offset += 64;
				return make_unique<ConstArgument>(value);
			}
			catch (out_of_range & e) {
				(void)e;
				throw ProgramMemoryOutOfRangeRuntimeError{};
			}
		}

		/*
		Acquire instruction address form the bit stream
		*/
		ArgumentPtr getAddressArgument(const Utils::BitBuffer & bb, uint32_t & offset)
		{
			try {
				auto value = bb.getU32(offset, 32, true);
				offset += 32;
				return make_unique<AddressArgument>(value);
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

		string RegisterArgument::label() const
		{
			return "reg" + to_string(_regIndex);
		}

		string RegisterArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "reg" << to_string(_regIndex) << ":0x" << setfill('0') << setw(16) << hex << getValue(thread);
			return oss.str();
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

		string MemoryBYTEArgument::label() const
		{
			return "BYTE[reg" + to_string(_regIndex) + "]";
		}

		string MemoryBYTEArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "BYTE:0x" << setfill('0') << setw(2) << hex << getValue(thread);
			return oss.str();
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

		string MemoryWORDArgument::label() const
		{
			return "WORD[reg" + to_string(_regIndex) + "]";
		}

		string MemoryWORDArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "WORD:0x" << setfill('0') << setw(4) << hex << getValue(thread);
			return oss.str();
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

		string MemoryDWORDArgument::label() const
		{
			return "DWORD[reg" + to_string(_regIndex) + "]";
		}

		string MemoryDWORDArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "DWORD:0x" << setfill('0') << setw(8) << hex << getValue(thread);
			return oss.str();
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

		string MemoryQWORDArgument::label() const
		{
			return "QWORD[reg" + to_string(_regIndex) + "]";
		}

		string MemoryQWORDArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "QWORD:0x" << setfill('0') << setw(16) << hex << getValue(thread);
			return oss.str();
		}

		uint64_t ConstArgument::getValue(ThreadContext & thread) const
		{
			return _constValue;
		}

		void ConstArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			throw WriteToConstRuntimeError{};
		}

		string ConstArgument::label() const
		{
			return "const";
		}

		string ConstArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "const:0x" << setfill('0') << setw(16) << hex << getValue(thread);
			return oss.str();
		}

		uint64_t AddressArgument::getValue(ThreadContext & thread) const
		{
			return static_cast<uint64_t>(_value);
		}

		void AddressArgument::setValue(ThreadContext & thread, uint64_t value)
		{
			throw WriteToConstRuntimeError{};
		}
		string AddressArgument::label() const
		{
			return "address";
		}
		string AddressArgument::printValue(ThreadContext & thread) const
		{
			ostringstream oss;
			oss << "add:0x" << setfill('0') << setw(8) << hex << getValue(thread);
			return oss.str();
		}
}
}
