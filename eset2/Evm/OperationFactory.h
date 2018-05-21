#pragma once

#include "Operation.h"
#include "BitBuffer.h"

namespace Evm {
	namespace Operation {
		constexpr uint32_t OPCODE_3BIT_MOV				= 0x00000000;	// mov arg1, arg2					opcode 000
		constexpr uint32_t OPCODE_3BIT_LOAD_CONSTANT	= 0x00000001;	// loadConst constant, arg1			opcode 001
		constexpr uint32_t OPCODE_4BIT_CALL				= 0x0000000c;	// call address						opcode 1100
		constexpr uint32_t OPCODE_4BIT_RET				= 0x0000000d;	// ret								opcode 1101
		constexpr uint32_t OPCODE_4BIT_LOCK				= 0x0000000e;	// lock arg1						opcode 1110
		constexpr uint32_t OPCODE_4BIT_UNLOCK			= 0x0000000f;	// unlock arg1						opcode 1111
		constexpr uint32_t OPCODE_5BIT_COMPARE			= 0x0000000c;	// compare arg1, arg2, arg3			opcode 01100
		constexpr uint32_t OPCODE_5BIT_JUMP				= 0x0000000d;	// jump address						opcode 01101
		constexpr uint32_t OPCODE_5BIT_JUMP_EQUAL		= 0x0000000e;	// jumpEqual address, arg1, arg2	opcode 01110
		constexpr uint32_t OPCODE_5BIT_READ				= 0x00000010;	// read arg1, arg2, arg3, arg4		opcode 10000
		constexpr uint32_t OPCODE_5BIT_WRITE			= 0x00000011;	// write arg1, arg2, arg3			opcode 10001
		constexpr uint32_t OPCODE_5BIT_CONSOLE_READ		= 0x00000012;	// consoleRead arg1					opcode 10010
		constexpr uint32_t OPCODE_5BIT_CONSOLE_WRITE	= 0x00000013;	// consoleWrite arg1				opcode 10011
		constexpr uint32_t OPCODE_5BIT_CREATE_THREAD	= 0x00000014;	// createThread address, arg1		opcode 10100
		constexpr uint32_t OPCODE_5BIT_JOIN_THREAD		= 0x00000015;	// joinThread arg1					opcode 10101
		constexpr uint32_t OPCODE_5BIT_HLT				= 0x00000016;	// hlt								opcode 10110
		constexpr uint32_t OPCODE_5BIT_SLEEP			= 0x00000017;	// sleep arg1						opcode 10111
		constexpr uint32_t OPCODE_6BIT_ADD				= 0x00000011;	// add arg1, arg2, arg3				opcode 010001
		constexpr uint32_t OPCODE_6BIT_SUB				= 0x00000012;	// sub arg1, arg2, arg3				opcode 010010
		constexpr uint32_t OPCODE_6BIT_DIV				= 0x00000013;	// div arg1, arg2, arg3				opcode 010011
		constexpr uint32_t OPCODE_6BIT_MOD				= 0x00000014;	// mod arg1, arg2, arg3				opcode 010100
		constexpr uint32_t OPCODE_6BIT_MUL				= 0x00000015;	// mul arg1, arg2, arg3				opcode 010101
		

		/*
		Interface that creates IOperation objects.
		*/
		struct IOperationFactory {
			IOperationFactory(const BitBuffer & programMemory) :
				_programMemory{ programMemory }
			{}

			virtual ~IOperationFactory() = default;
			virtual OperationPtr build(uint32_t & offset) = 0;

		protected:
			const BitBuffer & _programMemory;
		};

		template <typename T>
		struct NoneArgOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				return make_unique<T>();
			}
		};

		template <typename T>
		struct Arg1OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto arg1 = Argument::getArgument(_programMemory, offset);

				return make_unique<T>(arg1);
			}
		};

		template <typename T>
		struct Arg1Arg2OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto arg1 = Argument::getArgument(_programMemory, offset);
				auto arg2 = Argument::getArgument(_programMemory, offset);

				return make_unique<T>(arg1, arg2);
			}
		};

		template <typename T>
		struct Arg1Arg2Arg3OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto arg1 = Argument::getArgument(_programMemory, offset);
				auto arg2 = Argument::getArgument(_programMemory, offset);
				auto arg3 = Argument::getArgument(_programMemory, offset);

				return make_unique<T>(arg1, arg2, arg3);
			}
		};

		template <typename T>
		struct Arg1Arg2Arg3Arg4OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto arg1 = Argument::getArgument(_programMemory, offset);
				auto arg2 = Argument::getArgument(_programMemory, offset);
				auto arg3 = Argument::getArgument(_programMemory, offset);
				auto arg4 = Argument::getArgument(_programMemory, offset);

				return make_unique<T>(arg1, arg2, arg3, arg4);
			}
		};

		template <typename T>
		struct AddressOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto address = Argument::getAddress(_programMemory, offset);

				return make_unique<T>(address);
			}
		};

		/*
		Last link of the responsibility chain. When cought, throw unknown operation exception
		*/
		struct UnsupportedOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				throw UnknownOperationRuntimeError{};
			}
		};

		/*
		Last link of the responsibility chain. When cought, throw unknown operation exception
		*/
		struct NotImplementedOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				throw NotImplementedOperationRuntimeError{};
			}
		};

		/*
		Factory that makes LoadConstOperation objects
		*/
		struct LoadConstOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset);
		};

		struct JumpEqualOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset);
		};

		struct CreateThreadOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset);
		};

		/*
		Factory that makes MathOperation objects. Detect math operation from opcode and create correct
		MathOperation class
		*/
		struct MathOperationFactory : IOperationFactory {
			MathOperationFactory(const BitBuffer & bb, function<int64_t(int64_t, int64_t)> function);
			OperationPtr build(uint32_t & offset);

		private:
			function<int64_t(int64_t, int64_t)> _function;
		};

		OperationPtr makeOperation(const BitBuffer & bb, uint32_t & offset);
	}
}
