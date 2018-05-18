#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Argument.h"

namespace Evm {
	namespace Operation {
		struct IOperation {
			virtual ~IOperation() = default;
			virtual void execute(ThreadContext & thread) = 0;
		};

		using OperationPtr = unique_ptr<IOperation>;

		struct MovOperation : IOperation {
			MovOperation(Argument::IArgument *arg1, Argument::IArgument *arg2) :
				_arg1{ arg1 }, _arg2{ arg2 }
			{}

			~MovOperation() {
				delete _arg1, _arg2;
			}

			virtual void execute(ThreadContext & thread) {
				auto arg1Value = _arg1->getValue(thread);
				_arg2->setValue(thread, arg1Value);
			}

		private:
			Argument::IArgument *_arg1;
			Argument::IArgument *_arg2;
		};

		struct LoadConstOperation : IOperation {
			LoadConstOperation(uint64_t constant, Argument::IArgument *arg1) :
				_constant{ constant }, _arg1{ arg1 }
			{}

			~LoadConstOperation() {
				delete _arg1;
			}

			virtual void execute(ThreadContext & thread) {
				_arg1->setValue(thread, _constant);
			}

		private:
			uint64_t _constant;
			Argument::IArgument * _arg1;
		};

		struct MathOperation : IOperation {
			MathOperation(Argument::IArgument *arg1,
				Argument::IArgument *arg2, Argument::IArgument *arg3,
				function<int64_t(int64_t, int64_t)> mathOperation) :
				_arg1{ arg1 }, _arg2{ arg2 }, _arg3{ arg3 }, _mathOperation{ mathOperation }
			{}

			~MathOperation() {
				delete _arg1, _arg2, _arg3;
			}

			virtual void execute(ThreadContext & thread) {
				// Get arguments, convert them to signed numbers
				uint64_t arg1Value = _arg1->getValue(thread);
				uint64_t arg2Value = _arg2->getValue(thread);
				int64_t arg1Signed = *reinterpret_cast<int64_t *>(&arg1Value);
				int64_t arg2Signed = *reinterpret_cast<int64_t *>(&arg2Value);

				// compute math operation, convert back to usigned
				int64_t result = _mathOperation(arg1Signed, arg2Signed);
				uint64_t resultUnsigned = *reinterpret_cast<uint64_t *>(&result);
				_arg3->setValue(thread, resultUnsigned);
			}

		private:
			Argument::IArgument *_arg1;
			Argument::IArgument *_arg2;
			Argument::IArgument *_arg3;
			function<int64_t(int64_t, int64_t)> _mathOperation;
		};

		struct ConsoleWriteOperation : IOperation {
			ConsoleWriteOperation(Argument::IArgument *arg1) :
				_arg1{ arg1 }
			{}

			~ConsoleWriteOperation() {
				delete _arg1;
			}

			virtual void execute(ThreadContext & thread) {
				uint64_t value = _arg1->getValue(thread);
				ios_base::fmtflags flags{ cout.flags() };
				cout << "0x" << setfill('0') << setw(16) << hex << value << "\n";
				cout.flags(flags);
			}

		private:
			Argument::IArgument * _arg1;
		};

		struct ConsoleReadOperation : IOperation {
			ConsoleReadOperation(Argument::IArgument *arg1) :
				_arg1{ arg1 }
			{}

			~ConsoleReadOperation() {
				delete _arg1;
			}

			virtual void execute(ThreadContext & thread) {
				uint16_t input;
				ios_base::fmtflags flags{ cin.flags() };
				cin >> hex >> input;
				cin.flags(flags);
				_arg1->setValue(thread, input);
			}

		private:
			Argument::IArgument * _arg1;
		};

		struct CallOperation : IOperation {
			CallOperation(uint32_t address) :
				_address{ address }
			{}

			virtual void execute(ThreadContext & thread) {
				thread.push(thread.programCounter());
				thread.programCounter(_address);
			}

		private:
			uint32_t _address;
		};

		struct JumpOperation : IOperation {
			JumpOperation(uint32_t address) :
				_address{ address }
			{}

			virtual void execute(ThreadContext & thread) {
				thread.programCounter(_address);
			}

		private:
			uint32_t _address;
		};

		struct HltOperation : IOperation {
			virtual void execute(ThreadContext & thread) {
				thread.terminate();
			}
		};

		struct RetOperation : IOperation {
			virtual void execute(ThreadContext & thread) {
				thread.programCounter(thread.pop());
			}
		};

		struct JumpEqualOperation : IOperation {
			JumpEqualOperation(uint32_t address, 
				Argument::IArgument *arg1, Argument::IArgument *arg2) :
				_address{ address }, _arg1{ arg1 }, _arg2{ arg2 }
			{}

			~JumpEqualOperation() {
				delete _arg1, _arg2;
			}

			virtual void execute(ThreadContext & thread) {
				auto arg1Value = _arg1->getValue(thread);
				auto arg2Value = _arg2->getValue(thread);
				if (arg1Value == arg2Value) {
					thread.programCounter(_address);
				}
			}

		private:
			uint32_t _address;
			Argument::IArgument *_arg1;
			Argument::IArgument *_arg2;
		};
	}
}