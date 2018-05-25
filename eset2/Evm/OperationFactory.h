//! @file	OperationFactory.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	Operation - abstraction which represent Evm instruction
#pragma once

#include "Operation.h"
#include "RuntimeError.h"
#include "BitBuffer.h"

namespace Evm {
	namespace Operation {
		constexpr uint32_t OPCODE_3BIT_MOV				= 0x00000000;	//!< mov arg1, arg2					opcode 000
		constexpr uint32_t OPCODE_3BIT_LOAD_CONSTANT	= 0x00000001;	//!< loadConst constant, arg1			opcode 001
		constexpr uint32_t OPCODE_4BIT_CALL				= 0x0000000c;	//!< call address						opcode 1100
		constexpr uint32_t OPCODE_4BIT_RET				= 0x0000000d;	//!< ret								opcode 1101
		constexpr uint32_t OPCODE_4BIT_LOCK				= 0x0000000e;	//!< lock arg1						opcode 1110
		constexpr uint32_t OPCODE_4BIT_UNLOCK			= 0x0000000f;	//!< unlock arg1						opcode 1111
		constexpr uint32_t OPCODE_5BIT_COMPARE			= 0x0000000c;	//!< compare arg1, arg2, arg3			opcode 01100
		constexpr uint32_t OPCODE_5BIT_JUMP				= 0x0000000d;	//!< jump address						opcode 01101
		constexpr uint32_t OPCODE_5BIT_JUMP_EQUAL		= 0x0000000e;	//!< jumpEqual address, arg1, arg2	opcode 01110
		constexpr uint32_t OPCODE_5BIT_READ				= 0x00000010;	//!< read arg1, arg2, arg3, arg4		opcode 10000
		constexpr uint32_t OPCODE_5BIT_WRITE			= 0x00000011;	//!< write arg1, arg2, arg3			opcode 10001
		constexpr uint32_t OPCODE_5BIT_CONSOLE_READ		= 0x00000012;	//!< consoleRead arg1					opcode 10010
		constexpr uint32_t OPCODE_5BIT_CONSOLE_WRITE	= 0x00000013;	//!< consoleWrite arg1				opcode 10011
		constexpr uint32_t OPCODE_5BIT_CREATE_THREAD	= 0x00000014;	//!< createThread address, arg1		opcode 10100
		constexpr uint32_t OPCODE_5BIT_JOIN_THREAD		= 0x00000015;	//!< joinThread arg1					opcode 10101
		constexpr uint32_t OPCODE_5BIT_HLT				= 0x00000016;	//!< hlt								opcode 10110
		constexpr uint32_t OPCODE_5BIT_SLEEP			= 0x00000017;	//!< sleep arg1						opcode 10111
		constexpr uint32_t OPCODE_6BIT_ADD				= 0x00000011;	//!< add arg1, arg2, arg3				opcode 010001
		constexpr uint32_t OPCODE_6BIT_SUB				= 0x00000012;	//!< sub arg1, arg2, arg3				opcode 010010
		constexpr uint32_t OPCODE_6BIT_DIV				= 0x00000013;	//!< div arg1, arg2, arg3				opcode 010011
		constexpr uint32_t OPCODE_6BIT_MOD				= 0x00000014;	//!< mod arg1, arg2, arg3				opcode 010100
		constexpr uint32_t OPCODE_6BIT_MUL				= 0x00000015;	//!< mul arg1, arg2, arg3				opcode 010101
		
		//! @brief IOperation factory intefrace
		//!
		//! The interface is capable of making IOperation objects
		//! It is used in opcode decoder. The interface provides @ref build() function
		//! that makes a prtticular IOperation object
		struct IOperationFactory {
			//! @brief Constructor
			//!
			//! Get printable label of an instruction and reference to program memory
			//! @param opcode Printable label of the instruction
			//! @param programMemory Reference to program memory
			IOperationFactory(const string & opcode, const Utils::BitBuffer & programMemory) :
				_opcode{ opcode },
				_programMemory{ programMemory }
			{}

			//! @brief Virtual destructor
			virtual ~IOperationFactory() = default;

			//! @brief Build IOperation object
			//!
			//! Offset is set to first byte of the first argument.
			//! @note Offset is input/output reference. After execution it points to the next bit
			//!		after argument set, that has been required by the current instruction
			//! @param offset Input/output reference. As input, offset to first bit of first argument
			//!		for current instruction. As output - offset to first bit after the last argument
			//!		required by the instruction
			//! @return Pointer to IOperation object
			virtual OperationPtr build(uint32_t & offset) = 0;

		protected:
			string _opcode;		//!< printable label of tha instruction that is built
			const Utils::BitBuffer & _programMemory;	//!< Reference to program memory
		};

		//! @brief A factory that makes operation without arguments
		template <typename T>
		struct NoneArgOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				(void)offset;
				return make_unique<T>(_opcode);
			}
		};

		//! @brief A factory that makes operation with one "register" argument
		template <typename T>
		struct Arg1OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto res = make_unique<T>(_opcode);
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				return res;
			}
		};

		//! @brief A factory that makes operation with two "register" arguments
		template <typename T>
		struct Arg1Arg2OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto res = make_unique<T>(_opcode);
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				return res;
			}
		};

		//! @brief A factory that makes operation with three "register" arguments
		template <typename T>
		struct Arg1Arg2Arg3OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				auto res = make_unique<T>(_opcode);
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				return res;
			}
		};

		//! @brief A factory that makes operation with four "register" arguments
		template <typename T>
		struct Arg1Arg2Arg3Arg4OperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				OperationPtr res = make_unique<T>(_opcode);
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				res->pushArgument(Argument::getRegisterArgument(_programMemory, offset));
				return res;
			}
		};

		//! @brief A factory that makes operation with address argument
		template <typename T>
		struct AddressOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				OperationPtr res = make_unique<T>(_opcode);
				res->pushArgument(Argument::getAddressArgument(_programMemory, offset));
				return res;
			}
		};

		//! @brief A factory for unsupported operation
		//!
		//! Building will cause throwing @ref UnknownOperationRuntimeError
		struct UnsupportedOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				(void)offset;
				throw UnknownOperationRuntimeError{};
			}
		};

		//! @brief A factory for not implemented operation
		//!
		//! Building will cause throwing @ref NotImplementedOperationRuntimeError
		struct NotImplementedOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset) {
				(void)offset;
				throw NotImplementedOperationRuntimeError{};
			}
		};

		//! @brief A factory that makes loadConst operation
		struct LoadConstOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset);
		};

		//! @brief A factory that makes jumpEqual operation
		struct JumpEqualOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset);
		};

		//! @brief A factory that makes createThread operation
		struct CreateThreadOperationFactory : IOperationFactory {
			using IOperationFactory::IOperationFactory;
			virtual OperationPtr build(uint32_t & offset);
		};

		//! @brief A factory that makes MathOperation. 
		struct MathOperationFactory : IOperationFactory {
			//! @brief Constructor
			//!
			//! @param opcode Printable label of the instruction
			//! @param programMemory Reference to program memory
			//! @param function Lambda with math operation
			MathOperationFactory(const string & opcode, const Utils::BitBuffer & bb, function<int64_t(int64_t, int64_t)> function);
			OperationPtr build(uint32_t & offset);
		private:
			function<int64_t(int64_t, int64_t)> _function;	//!< Lambda with math operation
		};

		//! @brief Opcode decoder
		//!
		//! Parse program memory from given offset and determine operation from opcode.
		//! @note Offset is input/output reference. After execution it points to the next bit
		//!		after argument set, that has been required by the current instruction
		//! @param programMemory Reference to program memory
		//! @param offset Input/output reference. As input, offset to first bit of first argument
		//!		for current instruction. As output - offset to first bit after the last argument
		//!		required by the instruction
		//! @return Object of IOperation interface
		OperationPtr makeOperation(const Utils::BitBuffer & programMemory, uint32_t & offset);
	}
}
