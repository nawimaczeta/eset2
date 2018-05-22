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
			virtual string label() const = 0;
			virtual string printValue(ThreadContext & thread) const = 0;
		};

		using ArgumentPtr = unique_ptr<Argument::IArgument>;

		struct IRegisterBaseArgument : IArgument {
			IRegisterBaseArgument(uint8_t registerIndex) :
				_regIndex{ registerIndex }
			{}
		protected:
			uint8_t _regIndex;
		};

		struct RegisterArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		struct MemoryBYTEArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		struct MemoryWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		struct MemoryDWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		struct MemoryQWORDArgument : IRegisterBaseArgument {
			using IRegisterBaseArgument::IRegisterBaseArgument;
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		};

		struct ConstArgument : IArgument {
			ConstArgument(uint64_t constValue) :
				_constValue{ constValue }
			{}
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		private:
			const uint64_t _constValue;
		};

		struct AddressArgument : IArgument {
			AddressArgument(uint32_t value) :
				_value{ value }
			{}
			virtual uint64_t getValue(ThreadContext & thread) const override;
			virtual void setValue(ThreadContext & thread, uint64_t value) override;
			virtual string label() const override;
			virtual string printValue(ThreadContext & thread) const override;
		private:
			const uint32_t _value;
		};

		ArgumentPtr getRegisterArgument(const Utils::BitBuffer & bb, uint32_t & offset);
		ArgumentPtr getConstantArgument(const Utils::BitBuffer & bb, uint32_t & offset);
		ArgumentPtr getAddressArgument(const Utils::BitBuffer & bb, uint32_t & offset);
	}
}