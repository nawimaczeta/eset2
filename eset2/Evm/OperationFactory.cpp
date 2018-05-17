#include "stdafx.h"
#include "OperationFactory.h"
#include "Argument.h"
#include "RuntimeError.h"

namespace Evm {
	namespace Operation {
		OperationPtr MovOperationFactory::build(uint8_t opcode, BitStream & bs)
		{
			uint32_t maskedOpcode = opcode & _MASK;
			if (maskedOpcode == _OPCODE) {
				auto offset = bs.position();
				bs.pop(3);

				auto arg1 = EvmArgument::getArgument(bs);
				auto arg2 = EvmArgument::getArgument(bs);

				auto operation = make_unique<MovOperation>(offset, arg1, arg2);
				return move(operation);
			}
			else {
				return IOperationFactory::build(opcode, bs);
			}
		}

		OperationPtr LoadConstOperationFactory::build(uint8_t opcode, BitStream & bs)
		{
			uint32_t maskedOpcode = opcode & _MASK;
			if (maskedOpcode == _OPCODE) {
				auto offset = bs.position();
				bs.pop(3);

				auto constant = EvmArgument::getConstant(bs);
				auto arg1 = EvmArgument::getArgument(bs);

				auto operation = make_unique<LoadConstOperation>(offset, constant, arg1);
				return move(operation);
			}
			else {
				return IOperationFactory::build(opcode, bs);
			}
		}

		OperationPtr MathOperationFactory::build(uint8_t opcode, BitStream & bs)
		{
			uint32_t maskedOpcode = opcode & _MATH_OPCODE_MASK;

			// detect math operation opcode
			switch (maskedOpcode) {
			case _ADD_OPCODE:
				return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a + b; });
				break;
			case _SUB_OPCODE:
				return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a - b; });
				break;
			case _DIV_OPCODE:
				return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a / b; });
				break;
			case _MOD_OPCODE:
				return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a % b; });
				break;
			case _MUL_OPCODE:
				return _makeMathExpression(bs, 6, [](int64_t a, int64_t b) {return a * b; });
				break;
			}

			// detect compare opcode. Compare operation can be implemented as MathOperation object
			uint32_t compareMaskedOpcode = opcode & _COMPARE_OPCODE_MASK;
			if (compareMaskedOpcode == _COMPARE_OPCODE) {
				return _makeMathExpression(bs, 5, [](int64_t a, int64_t b) {
					if (a < b) return -1;
					else if (a == b) return 0;
					else return 1;
				});
			}

			return IOperationFactory::build(opcode, bs);
		}

		OperationPtr MathOperationFactory::_makeMathExpression(BitStream & bs, uint32_t opcodeSize,
			function<int64_t(int64_t, int64_t)> mathOperation) const
		{
			auto offset = bs.position();
			bs.pop(opcodeSize);

			auto arg1 = EvmArgument::getArgument(bs);
			auto arg2 = EvmArgument::getArgument(bs);
			auto arg3 = EvmArgument::getArgument(bs);

			auto operation = make_unique<MathOperation>(offset, arg1, arg2, arg3, mathOperation);
			return move(operation);
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

		OperationPtr makeOperation(uint8_t opcode, BitStream & bs)
		{
			// build chain of responsibilities
			IOperationFactory::OperationFactoryPtr handler = nullptr;
			handler = make_unique<UnsupportedOperationFactory>(move(handler));
			handler = make_unique<ConsoleWriteOperationFactory>(move(handler));
			handler = make_unique<MathOperationFactory>(move(handler));
			handler = make_unique<LoadConstOperationFactory>(move(handler));
			handler = make_unique<MovOperationFactory>(move(handler));



			return handler->build(opcode, bs);
		}

		OperationPtr makeOperation(const BitBuffer & bb, uint32_t offset)
		{
			OperationPtr operation = nullptr;

			// detect 3-bit operations
			operation = makeOperation3Bit(bb, offset);
			if (operation) {
				return operation;
			}

			// detect 4-bit operations
			operation = makeOperation4Bit(bb, offset);
			if (operation) {
				return operation;
			}

			// detect 5-bit operations
			operation = makeOperation5Bit(bb, offset);
			if (operation) {
				return operation;
			}

			// detect 6-bit operations
			operation = makeOperation6Bit(bb, offset);
			if (operation) {
				return operation;
			}

			// unknown operation
			throw UnknownOperationRuntimeError{};
		}
}
}
