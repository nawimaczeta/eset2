#pragma once

#include "stdafx.h"
#include "ThreadContext.h"
#include "Argument.h"
#include "Application.h"

namespace Evm {
	namespace Operation {
		struct IOperation {
			virtual ~IOperation() = default;
			virtual void execute(ThreadContext & thread) = 0;
		};

		struct Arg1Operation : IOperation {
			Arg1Operation(Argument::IArgument *arg1) :
				_arg1{ arg1 }
			{}

			~Arg1Operation() {
				delete _arg1;
			}
		protected:
			Argument::IArgument *_arg1;
		};

		struct Arg1Arg2Operation : IOperation {
			Arg1Arg2Operation(Argument::IArgument *arg1, Argument::IArgument *arg2) :
				_arg1{ arg1 }, _arg2{ arg2 }
			{}

			~Arg1Arg2Operation() {
				delete _arg1;
				delete _arg2;

			}
		protected:
			Argument::IArgument *_arg1;
			Argument::IArgument *_arg2;
		};

		struct Arg1Arg2Arg3Operation : IOperation {
			Arg1Arg2Arg3Operation(Argument::IArgument *arg1, Argument::IArgument *arg2, Argument::IArgument *arg3) :
				_arg1{ arg1 }, _arg2{ arg2 }, _arg3{ arg3 }
			{}

			~Arg1Arg2Arg3Operation() {
				delete _arg1;
				delete _arg2;
				delete _arg3;
			}
		protected:
			Argument::IArgument *_arg1;
			Argument::IArgument *_arg2;
			Argument::IArgument *_arg3;
		};

		struct Arg1Arg2Arg3Arg4Operation : IOperation {
			Arg1Arg2Arg3Arg4Operation(Argument::IArgument *arg1, Argument::IArgument *arg2, 
				Argument::IArgument *arg3, Argument::IArgument *arg4) :
				_arg1{ arg1 }, _arg2{ arg2 }, _arg3{ arg3 }, _arg4{ arg4 }
			{}

			~Arg1Arg2Arg3Arg4Operation() {
				delete _arg1;
				delete _arg2;
				delete _arg3;
				delete _arg4;
			}
		protected:
			Argument::IArgument *_arg1;
			Argument::IArgument *_arg2;
			Argument::IArgument *_arg3;
			Argument::IArgument *_arg4;
		};

		using OperationPtr = unique_ptr<IOperation>;

		struct MovOperation : Arg1Arg2Operation {
			Arg1Arg2Operation::Arg1Arg2Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct LoadConstOperation : Arg1Operation {
			LoadConstOperation(uint64_t constant, Argument::IArgument *arg1) :
				_constant{ constant }, Arg1Operation(arg1)
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			uint64_t _constant;
		};

		struct MathOperation : Arg1Arg2Arg3Operation {
			MathOperation(Argument::IArgument *arg1,
				Argument::IArgument *arg2, Argument::IArgument *arg3,
				function<int64_t(int64_t, int64_t)> mathOperation) :
				Arg1Arg2Arg3Operation{ arg1, arg2, arg3 }, _mathOperation{ mathOperation }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			function<int64_t(int64_t, int64_t)> _mathOperation;
		};

		struct ConsoleWriteOperation : Arg1Operation {
			using Arg1Operation::Arg1Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct ConsoleReadOperation : Arg1Operation {
			using Arg1Operation::Arg1Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct CallOperation : IOperation {
			CallOperation(uint32_t address) :
				_address{ address }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			uint32_t _address;
		};

		struct JumpOperation : IOperation {
			JumpOperation(uint32_t address) :
				_address{ address }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			uint32_t _address;
		};

		struct HltOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct RetOperation : IOperation {
			virtual void execute(ThreadContext & thread) override;
		};

		struct JumpEqualOperation : Arg1Arg2Operation {
			JumpEqualOperation(uint32_t address, 
				Argument::IArgument *arg1, Argument::IArgument *arg2) :
				_address{ address }, Arg1Arg2Operation{ arg1, arg2 }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			uint32_t _address;
		};

		struct CreateThreadOperation : Arg1Operation {
			CreateThreadOperation(uint32_t address,
				Argument::IArgument *arg1) :
				_address{ address }, Arg1Operation{ arg1 }
			{}
			virtual void execute(ThreadContext & thread) override;
		private:
			uint32_t _address;
		};

		struct JoinOperation : Arg1Operation {
			using Arg1Operation::Arg1Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct SleepOperation : Arg1Operation {
			using Arg1Operation::Arg1Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct LockOperation : Arg1Operation {
			using Arg1Operation::Arg1Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct UnlockOperation : Arg1Operation {
			using Arg1Operation::Arg1Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct WriteOperation : Arg1Arg2Arg3Operation {
			using Arg1Arg2Arg3Operation::Arg1Arg2Arg3Operation;
			virtual void execute(ThreadContext & thread) override;
		};

		struct ReadOperation : Arg1Arg2Arg3Arg4Operation {
			using Arg1Arg2Arg3Arg4Operation::Arg1Arg2Arg3Arg4Operation;
			virtual void execute(ThreadContext & thread) override;
		};
	}
}