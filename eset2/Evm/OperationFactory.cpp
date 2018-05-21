#include "stdafx.h"
#include "OperationFactory.h"
#include "Argument.h"
#include "RuntimeError.h"

namespace Evm {
	namespace Operation {
		OperationPtr LoadConstOperationFactory::build(uint32_t & offset)
		{
			auto constant = Argument::getConstant(IOperationFactory::_programMemory, offset);
			auto arg1 = Argument::getArgument(IOperationFactory::_programMemory, offset);

			return make_unique<LoadConstOperation>(constant, arg1);
		}

		MathOperationFactory::MathOperationFactory(const BitBuffer & bb, function<int64_t(int64_t, int64_t)> function) :
			IOperationFactory{ bb },
			_function{ function }
		{}

		OperationPtr MathOperationFactory::build(uint32_t & offset)
		{
			auto arg1 = Argument::getArgument(IOperationFactory::_programMemory, offset);
			auto arg2 = Argument::getArgument(IOperationFactory::_programMemory, offset);
			auto arg3 = Argument::getArgument(IOperationFactory::_programMemory, offset);

			return make_unique<MathOperation>(arg1, arg2, arg3, _function);
		}

		OperationPtr JumpEqualOperationFactory::build(uint32_t & offset)
		{
			uint32_t address = Argument::getAddress(_programMemory, offset);
			auto arg1 = Argument::getArgument(_programMemory, offset);
			auto arg2 = Argument::getArgument(_programMemory, offset);

			return make_unique<JumpEqualOperation>(address, arg1, arg2);
		}

		OperationPtr CreateThreadOperationFactory::build(uint32_t & offset)
		{
			uint32_t address = Argument::getAddress(_programMemory, offset);
			auto arg1 = Argument::getArgument(_programMemory, offset);

			return make_unique<CreateThreadOperation>(address, arg1);
		}

		OperationPtr makeOperation(const BitBuffer & bb, uint32_t & offset)
		{
			unique_ptr<IOperationFactory> factory = nullptr;
			uint32_t tmpOffset;
			uint32_t opcode;

			// detect 3-bit operations
			opcode = bb.getU32(offset, 3);
			tmpOffset = offset + 3;
			switch (opcode) {
			case OPCODE_3BIT_MOV:
				factory = make_unique<Arg1Arg2OperationFactory<MovOperation>>(bb);
				break;
			case OPCODE_3BIT_LOAD_CONSTANT:
				factory = make_unique<LoadConstOperationFactory>(bb);
				break;
			}
			
			if (!factory) {
				// detect 4-bit operations
				tmpOffset = offset + 4;
				opcode = bb.getU32(offset, 4);
				switch (opcode) {
				case OPCODE_4BIT_CALL:
					factory = make_unique<AddressOperationFactory<CallOperation>>(bb);
					break;
				case OPCODE_4BIT_RET:
					factory = make_unique<NoneArgOperationFactory<RetOperation>>(bb);
					break;
				case OPCODE_4BIT_LOCK:
					factory = make_unique<Arg1OperationFactory<LockOperation>>(bb);
					break;
				case OPCODE_4BIT_UNLOCK:
					factory = make_unique<Arg1OperationFactory<UnlockOperation>>(bb);
					break;
				}
			}

			if (!factory) {
				// detect 5-bit operations
				tmpOffset = offset + 5;
				opcode = bb.getU32(offset, 5);
				switch (opcode) {
				case OPCODE_5BIT_COMPARE:
					factory = make_unique<MathOperationFactory>(bb,
						[](int64_t a, int64_t b) {
								if (a < b) return -1;
								else if (a == b) return 0;
								else return 1;
							});
					break;
				case OPCODE_5BIT_JUMP:
					factory = make_unique<AddressOperationFactory<JumpOperation>>(bb);
					break;
				case OPCODE_5BIT_JUMP_EQUAL:
					factory = make_unique<JumpEqualOperationFactory>(bb);
					break;
				case OPCODE_5BIT_READ:
					factory = make_unique<NotImplementedOperationFactory>(bb);
					break;
				case OPCODE_5BIT_WRITE:
					factory = make_unique<NotImplementedOperationFactory>(bb);
					break;
				case OPCODE_5BIT_CONSOLE_READ:
					factory = make_unique<Arg1OperationFactory<ConsoleReadOperation>>(bb);
					break;
				case OPCODE_5BIT_CONSOLE_WRITE:
					factory = make_unique<Arg1OperationFactory<ConsoleWriteOperation>>(bb);
					break;
				case OPCODE_5BIT_CREATE_THREAD:
					factory = make_unique<CreateThreadOperationFactory>(bb);
					break;
				case OPCODE_5BIT_JOIN_THREAD:
					factory = make_unique<Arg1OperationFactory<JoinOperation>>(bb);
					break;
				case OPCODE_5BIT_HLT:
					factory = make_unique<NoneArgOperationFactory<HltOperation>>(bb);
					break;
				case OPCODE_5BIT_SLEEP:
					factory = make_unique<Arg1OperationFactory<SleepOperation>>(bb);
					break;
				}
			}

			if (!factory) {
				// detect 6-bit operations
				tmpOffset = offset + 6;
				opcode = bb.getU32(offset, 6);
				switch (opcode) {
				case OPCODE_6BIT_ADD:
					factory = make_unique<MathOperationFactory>(bb, 
						[](int64_t a, int64_t b) {return a + b; });
					break;
				case OPCODE_6BIT_SUB:
					factory = make_unique<MathOperationFactory>(bb, 
						[](int64_t a, int64_t b) {return a - b; });
					break;
				case OPCODE_6BIT_DIV:
					factory = make_unique<MathOperationFactory>(bb, 
						[](int64_t a, int64_t b) {return a / b; });
					break;
				case OPCODE_6BIT_MOD:
					factory = make_unique<MathOperationFactory>(bb, 
						[](int64_t a, int64_t b) {return a % b; });
					break;
				case OPCODE_6BIT_MUL:
					factory = make_unique<MathOperationFactory>(bb, 
						[](int64_t a, int64_t b) {return a * b; });
					break;
				}
			}

			if (!factory) {
				// not supported opcode
				factory = make_unique<UnsupportedOperationFactory>(bb);
			}

			offset = tmpOffset;
			return factory->build(offset);
		}
}
}
