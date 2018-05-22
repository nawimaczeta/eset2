#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "BitBuffer.h"

namespace Evm {
	namespace Argument {
		struct IArgument {
			virtual ~IArgument() = default;
			virtual uint64_t getValue(ThreadContext & thread) const = 0;
			virtual void setValue(ThreadContext & thread, uint64_t value) = 0;
			
			enum class ValueType {
				UINT64,
				UINT32
			};
			virtual ValueType type() const = 0;
		};

		using ArgumentPtr = unique_ptr<Argument::IArgument>;

		struct IRegisterBaseArgument : IArgument {
			IRegisterBaseArgument(uint8_t registerIndex) :
				_regIndex{ registerIndex }
			{}
			virtual ValueType type() const override {
				return ValueType::UINT64;
			}
		protected:
			uint8_t _regIndex;
		};

		struct RegisterArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
		};

		struct MemoryBYTEArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
		};

		struct MemoryWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
		};

		struct MemoryDWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
		};

		struct MemoryQWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
		};

		struct ConstArgument : IArgument {
			ConstArgument(uint64_t constValue) :
				_constValue{ constValue }
			{}
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual ValueType type() const override {
				return ValueType::UINT64;
			}
		private:
			const uint64_t _constValue;
		};

		struct AddressArgument : IArgument {
			AddressArgument(uint32_t value) :
				_value{ value }
			{}
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual ValueType type() const override {
				return ValueType::UINT32;
			}
		private:
			const uint32_t _value;
		};

		ArgumentPtr getRegisterArgument(const Utils::BitBuffer & bb, uint32_t & offset);
		ArgumentPtr getConstant(const Utils::BitBuffer & bb, uint32_t & offset);
		ArgumentPtr getAddress(const Utils::BitBuffer & bb, uint32_t & offset);
	}
}