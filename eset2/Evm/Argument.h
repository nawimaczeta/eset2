//! @file	Argument.h
//! @author	Lukasz Iwanecki
//! @date	05.2018
//! @brief	Argument interface
//! 
//! IArgument interface represents Evm instruction argument.
//! Arguments are passes to IOperation class.
//! Evm sepcification intriduces a few argument types: address, constant,
//! register, BYTE, WORDm DWORD and QWORD. Each of them can be implemeted
//! as IArgument. Argument values are always 64bit unsigned numbers.
//! It is usually true but there is one exception - address argument is 32-bit.
//! It is up caller class to cast that value to 32-bit, but fortunatly the caller
//! is awared of argument types it uses.
//! The IArgument interface has two classes responsible of printing the arguemnt.
//! label() function prints readable form of the argument.
//! printValue() returns label as well as value of the parameter in runtime
#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "BitBuffer.h"

namespace Evm {
	namespace Argument {
		struct IArgument {
			//! @brief Virtual destructor
			virtual ~IArgument() = default;

			//! @brief Get value
			//!
			//! Return value of the argument
			//! @param thread Thread context within this argument is used
			//! @return Value of the parameter
			virtual uint64_t getValue(ThreadContext & thread) const = 0;

			//! @brief Set value
			//!
			//! Set value of the argument.
			//! @warning For read-only arguments this throws an exception
			//! @param thread Thread context within this argument is used
			//! @param value New value of the parameter
			virtual void setValue(ThreadContext & thread, uint64_t value) = 0;

			//! @brief Get printable representation of the argument
			//!
			//! @return string with argument label
			virtual string label() const = 0;

			//! @brief Get printable value of the parameter
			//!
			//! @param thread Thread context within this argument is used
			//! @return string with argument value
			virtual string printValue(ThreadContext & thread) const = 0;
		};

		using ArgumentPtr = unique_ptr<Argument::IArgument>;

		//! @brief Subinterface for register based arguments
		//!
		//! Interface for register based arguments. The arguments involves 
		//! register. Arguments in this group are: rX, BYTE[rX], WORD[rX], DWORD[rX],
		//! QDWORD[rX]
		struct IRegisterBaseArgument : IArgument {
			//! @brief Constructor
			//!
			//! @param Register index
			IRegisterBaseArgument(uint8_t registerIndex) :
				_regIndex{ registerIndex }
			{}
		protected:
			uint8_t _regIndex;	//!< Register index
		};

		//! @brief Register argument (rX)
		struct RegisterArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		//! @brief Memory BYTE[rX] argument
		struct MemoryBYTEArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		//! @brief Memory WORD[rX] argument
		struct MemoryWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		//! @brief Memory DWORD[rX] argument
		struct MemoryDWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		//! @brief Memory QWORD[rX] argument
		struct MemoryQWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		//! @brief Const argument
		//!
		//! @warning This is read only argument, writing will cause @ref WriteToConstRuntimeError
		struct ConstArgument : IArgument {
			//! @brief Constructor
			//!
			//! @param constValue Const value of the argument
			ConstArgument(uint64_t constValue) :
				_constValue{ constValue }
			{}
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		private:
			const uint64_t _constValue;		//!< const value
		};

		//! @brief Address argument
		//!
		//! @note Address is really 32-bit value. It up to caller to cast it to 32-bit
		//! @warning This is read only argument, writing will cause @ref WriteToConstRuntimeError
		struct AddressArgument : IArgument {
			AddressArgument(uint32_t value) :
				_value{ value }
			{}
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		private:
			const uint32_t _value;		//! address
		};

		//! @brief Make Register Argument
		//!
		//! Factory method. Make register argument from program memory, under given offset
		//! @note offset is input/output parameter. As input it contains offset in
		//!		program memory. It is updated by the function to and contains value 
		//!		bits of the argument
		//! @param programMemory Reference to program memory
		//! @param offset As input - offset to first bit of the argument. 
		//!		As output - offset to a bit after last bit of the argument
		//! @throw RuntimeError
		ArgumentPtr getRegisterArgument(const Utils::BitBuffer & programMemory, uint32_t & offset);

		//! @brief Make Constant Argument
		//!
		//! Factory method. Make constant argument from program memory, under given offset
		//! @note offset is input/output parameter. As input it contains offset in
		//!		program memory. It is updated by the function to and contains value 
		//!		bits of the argument
		//! @param programMemory Reference to program memory
		//! @param offset As input - offset to first bit of the argument. 
		//!		As output - offset to a bit after last bit of the argument
		//! @throw RuntimeError
		ArgumentPtr getConstantArgument(const Utils::BitBuffer & programMemory, uint32_t & offset);

		//! @brief Make Address Argument
		//!
		//! Factory method. Make address argument from program memory, under given offset
		//! @note offset is input/output parameter. As input it contains offset in
		//!		program memory. It is updated by the function to and contains value 
		//!		bits of the argument
		//! @param programMemory Reference to program memory
		//! @param offset As input - offset to first bit of the argument. 
		//!		As output - offset to a bit after last bit of the argument
		//! @throw RuntimeError
		ArgumentPtr getAddressArgument(const Utils::BitBuffer & programMemory, uint32_t & offset);
	}
}