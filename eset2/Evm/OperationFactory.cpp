//! @file	OperationFactory.cpp
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	OperationFactory class definition
#include "stdafx.h"
#include "OperationFactory.h"
#include "Argument.h"
#include "RuntimeError.h"

namespace Evm {
	namespace Operation {
		OperationPtr LoadConstOperationFactory::build(uint32_t & offset)
		{
			OperationPtr res = make_unique<LoadConstOperation>(_opcode);
			res->pushArgument(Argument::getConstantArgument(IOperationFactory::_programMemory, offset));
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			return res;
		}

		MathOperationFactory::MathOperationFactory(const string & opcode, const Utils::BitBuffer & programMemory, function<int64_t(int64_t, int64_t)> function) :
			IOperationFactory{ opcode, programMemory },
			_function{ function }
		{}

		OperationPtr MathOperationFactory::build(uint32_t & offset)
		{
			OperationPtr res = make_unique<MathOperation>(_opcode, _function);
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			return res;
		}

		OperationPtr JumpEqualOperationFactory::build(uint32_t & offset)
		{
			OperationPtr res = make_unique<JumpEqualOperation>(_opcode);
			res->pushArgument(Argument::getAddressArgument(_programMemory, offset));
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			return res;
		}

		OperationPtr CreateThreadOperationFactory::build(uint32_t & offset)
		{
			OperationPtr res = make_unique<CreateThreadOperation>(_opcode);
			res->pushArgument(Argument::getAddressArgument(_programMemory, offset));
			res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
			return res;
		}

		OperationPtr makeOperation(const Utils::BitBuffer & programMemory, uint32_t & offset)
		{
			unique_ptr<IOperationFactory> factory = nullptr;
			uint32_t tmpOffset;
			uint32_t opcode;

			// detect 3-bit operations
			opcode = programMemory.getU32(offset, 3);
			tmpOffset = offset + 3;
			switch (opcode) {
			case OPCODE_3BIT_MOV:
				factory = make_unique<Arg1Arg2OperationFactory<MovOperation>>("mov", programMemory);
				break;
			case OPCODE_3BIT_LOAD_CONSTANT:
				factory = make_unique<LoadConstOperationFactory>("loadConst", programMemory);
				break;
			}
			
			if (!factory) {
				// detect 4-bit operations
				tmpOffset = offset + 4;
				opcode = programMemory.getU32(offset, 4);
				switch (opcode) {
				case OPCODE_4BIT_CALL:
					factory = make_unique<AddressOperationFactory<CallOperation>>("call", programMemory);
					break;
				case OPCODE_4BIT_RET:
					factory = make_unique<NoneArgOperationFactory<RetOperation>>("ret", programMemory);
					break;
				case OPCODE_4BIT_LOCK:
					factory = make_unique<Arg1OperationFactory<LockOperation>>("lock", programMemory);
					break;
				case OPCODE_4BIT_UNLOCK:
					factory = make_unique<Arg1OperationFactory<UnlockOperation>>("unlock", programMemory);
					break;
				}
			}

			if (!factory) {
				// detect 5-bit operations
				tmpOffset = offset + 5;
				opcode = programMemory.getU32(offset, 5);
				switch (opcode) {
				case OPCODE_5BIT_COMPARE:
					factory = make_unique<MathOperationFactory>("compare", programMemory,
						[](int64_t a, int64_t b) {
								if (a < b) return -1;
								else if (a == b) return 0;
								else return 1;
							});
					break;
				case OPCODE_5BIT_JUMP:
					factory = make_unique<AddressOperationFactory<JumpOperation>>("jump", programMemory);
					break;
				case OPCODE_5BIT_JUMP_EQUAL:
					factory = make_unique<JumpEqualOperationFactory>("jumpEqual", programMemory);
					break;
				case OPCODE_5BIT_READ:
					factory = make_unique<Arg1Arg2Arg3Arg4OperationFactory<ReadOperation>>("read",programMemory);
					break;
				case OPCODE_5BIT_WRITE:
					factory = make_unique<Arg1Arg2Arg3OperationFactory<WriteOperation>>("write", programMemory);
					break;
				case OPCODE_5BIT_CONSOLE_READ:
					factory = make_unique<Arg1OperationFactory<ConsoleReadOperation>>("consoleRead", programMemory);
					break;
				case OPCODE_5BIT_CONSOLE_WRITE:
					factory = make_unique<Arg1OperationFactory<ConsoleWriteOperation>>("consoleWrite", programMemory);
					break;
				case OPCODE_5BIT_CREATE_THREAD:
					factory = make_unique<CreateThreadOperationFactory>("createThread", programMemory);
					break;
				case OPCODE_5BIT_JOIN_THREAD:
					factory = make_unique<Arg1OperationFactory<JoinOperation>>("joinThread", programMemory);
					break;
				case OPCODE_5BIT_HLT:
					factory = make_unique<NoneArgOperationFactory<HltOperation>>("hlt", programMemory);
					break;
				case OPCODE_5BIT_SLEEP:
					factory = make_unique<Arg1OperationFactory<SleepOperation>>("sleep", programMemory);
					break;
				}
			}

			if (!factory) {
				// detect 6-bit operations
				tmpOffset = offset + 6;
				opcode = programMemory.getU32(offset, 6);
				switch (opcode) {
				case OPCODE_6BIT_ADD:
					factory = make_unique<MathOperationFactory>("add", programMemory, 
						[](int64_t a, int64_t b) {return a + b; });
					break;
				case OPCODE_6BIT_SUB:
					factory = make_unique<MathOperationFactory>("sub", programMemory, 
						[](int64_t a, int64_t b) {return a - b; });
					break;
				case OPCODE_6BIT_DIV:
					factory = make_unique<MathOperationFactory>("div", programMemory, 
						[](int64_t a, int64_t b) {return a / b; });
					break;
				case OPCODE_6BIT_MOD:
					factory = make_unique<MathOperationFactory>("mod", programMemory, 
						[](int64_t a, int64_t b) {return a % b; });
					break;
				case OPCODE_6BIT_MUL:
					factory = make_unique<MathOperationFactory>("mul", programMemory, 
						[](int64_t a, int64_t b) {return a * b; });
					break;
				}
			}

			if (!factory) {
				// not supported opcode
				factory = make_unique<UnsupportedOperationFactory>("", programMemory);
			}

			offset = tmpOffset;
			return factory->build(offset);
		}
}
}
