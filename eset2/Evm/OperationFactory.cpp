#include "stdafx.h"
#include "OperationFactory.h"
#include "Argument.h"
#include "RuntimeError.h"

namespace Evm {
	namespace Operation {
		OperationPtr MovOperationFactory::build()
		{
			auto arg1 = Argument::getArgument(IOperationFactory::_programMemory, IOperationFactory::_offset);
			auto arg2 = Argument::getArgument(IOperationFactory::_programMemory, IOperationFactory::_offset);

			return make_unique<MovOperation>(arg1, arg2);
		}

		OperationPtr LoadConstOperationFactory::build()
		{
			auto constant = Argument::getConstant(IOperationFactory::_programMemory, IOperationFactory::_offset);
			auto arg1 = Argument::getArgument(IOperationFactory::_programMemory, IOperationFactory::_offset);

			return make_unique<LoadConstOperation>(constant, arg1);
		}

		MathOperationFactory::MathOperationFactory(const BitBuffer & bb, uint32_t & offset, function<int64_t(int64_t, int64_t)> function) :
			IOperationFactory{bb, offset },
			_function{ function }
		{}

		OperationPtr MathOperationFactory::build()
		{
			//return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a + b; });
			//	break;
			//case _SUB_OPCODE:
			//	return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a - b; });
			//	break;
			//case _DIV_OPCODE:
			//	return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a / b; });
			//	break;
			//case _MOD_OPCODE:
			//	return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a % b; });
			//	break;
			//case _MUL_OPCODE:
			//	return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a * b; });
			//	break;
			//}

			//// detect compare opcode. Compare operation can be implemented as MathOperation object
			//uint32_t compareMaskedOpcode = opcode & _COMPARE_OPCODE_MASK;
			//if (compareMaskedOpcode == _COMPARE_OPCODE) {
			//	return _makeMathExpression(bs, 5, [](int64_t a, int64_t b) {
			//		if (a < b) return -1;
			//		else if (a == b) return 0;
			//		else return 1;
			//	});
			//}

			auto arg1 = Argument::getArgument(IOperationFactory::_programMemory, IOperationFactory::_offset);
			auto arg2 = Argument::getArgument(IOperationFactory::_programMemory, IOperationFactory::_offset);
			auto arg3 = Argument::getArgument(IOperationFactory::_programMemory, IOperationFactory::_offset);

			return make_unique<MathOperation>(arg1, arg2, arg3, _function);
		}

		OperationPtr ConsoleWriteOperationFactory::build(uint8_t opcode, BitStream & bs)
		{
			uint32_t maskedOpcode = opcode & _MASK;
			if (maskedOpcode == _OPCODE) {
				auto offset = bs.position();
				bs.pop(5);

				auto arg1 = EvmArgument::getArgument(bs);

				auto operation = make_unique<ConsoleWriteOperation>(offset, arg1);
				return move(operation);
			}
			else {
				return IOperationFactory::build(opcode, bs);
			}
		}

		OperationPtr makeOperation(const BitBuffer & bb, uint32_t & offset)
		{
			OperationPtr operation = nullptr;
			uint32_t opcode;

			// detect 3-bit operations
			opcode = bb.getU32(offset, 3);
			switch (opcode) {
			case OPCODE_3BIT_MOV:
				return MovOperationFactory{ bb, offset }.build();
				break;
			case OPCODE_3BIT_LOAD_CONSTANT:
				return LoadConstOperationFactory{}.build(bb, offset);
				break;
			}

			// detect 4-bit operations
			opcode = bb.getU32(offset, 4);
			switch (opcode) {
			case OPCODE_4BIT_CALL:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_4BIT_RET:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_4BIT_LOCK:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_4BIT_UNLOCK:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			}

			// detect 5-bit operations
			opcode = bb.getU32(offset, 5);
			switch (opcode) {
			case OPCODE_5BIT_COMPARE:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_JUMP:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_JUMP_EQUAL:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_READ:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_WRITE:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_CONSOLE_READ:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_CONSOLE_WRITE:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_CREATE_THREAD:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_JOIN_THREAD:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_HLT:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_5BIT_SLEEP:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			}

			// detect 6-bit operations
			opcode = bb.getU32(offset, 6);
			switch (opcode) {
			case OPCODE_6BIT_ADD:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_6BIT_SUB:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_6BIT_DIV:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_6BIT_MOD:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			case OPCODE_6BIT_MUL:
				return UnsupportedOperationFactory{}.build(bb, offset);
				break;
			}

			// unknown operation
			return UnsupportedOperationFactory{}.build(bb, offset);
		}
}
}
