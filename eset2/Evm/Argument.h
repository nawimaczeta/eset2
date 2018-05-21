#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "BitBuffer.h"

namespace Evm {
	namespace Argument {
		struct IArgument {
			IArgument(uint8_t registerIndex) :
				_regIndex{ registerIndex }
			{}

			virtual ~IArgument() = default;
			virtual uint64_t getValue(ThreadContext & thread) const = 0;
			virtual void setValue(ThreadContext & thread, uint64_t value) = 0;
		protected:
			uint8_t _regIndex;
		};

		struct RegisterArgument : IArgument {
			using IArgument::IArgument;

			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);
		};

		struct MemoryBYTEArgument : IArgument {
			using IArgument::IArgument;
			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);
		};

		struct MemoryWORDArgument : IArgument {
			using IArgument::IArgument;
			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);
		};

		struct MemoryDWORDArgument : IArgument {
			using IArgument::IArgument;
			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);
		};

		struct MemoryQWORDArgument : IArgument {
			using IArgument::IArgument;
			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);
		};

		IArgument * getArgument(const BitBuffer & bb, uint32_t & offset);
		uint64_t getConstant(const BitBuffer & bb, uint32_t & offset);
		uint32_t getAddress(const BitBuffer & bb, uint32_t & offset);
	}
}