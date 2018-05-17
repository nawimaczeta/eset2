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
		};

		struct RegisterArgument : IArgument {
			RegisterArgument(uint8_t regIndex) :
				_regIndex{ regIndex }
			{}

			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);

		private:
			uint8_t _regIndex;
		};

		struct MemoryBYTEArgument : IArgument {
			MemoryBYTEArgument(uint8_t regIndex);
			virtual uint64_t getValue(ThreadContext & thread) const;
			virtual void setValue(ThreadContext & thread, uint64_t value);

		private:
			uint8_t _regIndex;
		};

		struct TmpArgument : IArgument {
			TmpArgument(uint64_t value) :
				_value(value)
			{}

			virtual uint64_t getValue(ThreadContext & thread) const {
				(void)thread;
				return _value;
			}

			virtual void setValue(ThreadContext & thread, uint64_t value) {
				(void)thread;
				_value = value;
			}

		private:
			uint64_t _value;
		};

		pair<IArgument *, uint32_t> getArgument(BitBuffer & bb, uint32_t offset);
		uint64_t getConstant(BitBuffer & bb, uint32_t offset);
		uint32_t getAddress(BitBuffer & bb, uint32_t offset);
	}
}